//
// Created by till on 09.04.20.
//

#include "string"
#include "list"
#include "request.h"
#include "json.hpp"
#include "helpers.h"
#include "extract.h"
#include "stream.h"
#include "query.h"

namespace youtube {
    class YouTube {
    public:
        explicit YouTube(const std::string &url) {
            this->video_url = url;
            try {
                this->video_id = extract::video_id(this->video_url);
            } catch (exceptions::RegexMatchError &e) {
                throw exceptions::VideoUnavailable();
            }
            this->embed_url = "https://www.youtube.com/embed/" + video_id;
            this->watch_url = "https://youtube.com/watch?v=" + video_id;
            this->prefetch();
            this->descramble();
        }

        std::vector<Stream> fmt_streams;

        std::string thumbnail_url();

        std::string title();

        std::string description();

        float rating();

        int length();

        int duration();

        int views();

        std::string author();

        std::string uploader();

        StreamQuery streams() const;
    private:
        std::string video_url = "";
        std::string js = "";
        std::string js_url = "";

        std::string vid_info_url = "";
        std::string video_info_raw = "";

        nlohmann::json vid_info;

        std::string watch_html = "";
        std::string embed_html = "";

        nlohmann::json player_config_args;

        nlohmann::json player_response;

        bool age_restricted = false;

        std::string video_id;

        std::string watch_url;
        std::string embed_url;

        void descramble();

        void prefetch();

        void initialize_stream_formats(const std::string &fmt);

    };

    void YouTube::prefetch() {
        /*
         * Download Data and extract age_restriction and publicity settings
         */
        this->watch_html = request::http_get(this->watch_url);
        if (this->watch_html.empty()) {
            throw std::exception();
        }
        this->age_restricted = extract::is_age_restricted(this->watch_html);

        if (not this->age_restricted && this->watch_html.find("This video is private") != std::string::npos) {
            throw exceptions::VideoUnavailable("This video is private.");
        }

        if (this->age_restricted) {
            if (not this->embed_html.empty()) {
                this->embed_html = request::http_get(this->embed_url);
            }
            this->vid_info_url = extract::video_info_url_age_restricted(
                    this->video_id, this->watch_url
            );

        } else {
            this->vid_info_url = extract::video_info_url(this->video_id, this->watch_url);
        }

        this->video_info_raw = request::http_get(this->vid_info_url);

        if (not this->age_restricted) {
            this->js_url = extract::js_url(this->watch_html);
            this->js = request::http_get(this->js_url);
        }
    }

    void YouTube::descramble() {
        /*
         * Make sense of the loaded data and extract stream urls and further details
         */
        this->vid_info = nlohmann::json(helpers::mapUrlParams(this->video_info_raw));
        if (this->age_restricted) {
            this->player_config_args = this->vid_info;
        } else {
            if (this->watch_html.empty()) {
                throw std::exception();
            }
            this->player_config_args = extract::get_ytplayer_config(this->watch_html)["args"];
        }
        if (not player_config_args.contains("title")) {
            unsigned long i_start = this->watch_html.find("<title>") + 7;
            unsigned long i_end = this->watch_html.find("</title>");

            std::string title = this->watch_html.substr(i_start, i_end - i_start);
            unsigned long index = title.find(" - YouTube");
            if (index > 0 and index != std::string::npos) {
                title = title.erase(index, 10);
            }

            this->player_config_args["title"] = title;
        }

        std::list<std::string> stream_maps = std::list<std::string>();
        stream_maps.emplace_back("url_encoded_fmt_stream_map");
        if (this->player_config_args.count("adaptive_fmts") > 0) {
            stream_maps.emplace_back("adaptive_fmts");
        }

        // unscramble the progressive and adaptive stream manifests.

        for (std::string &format : stream_maps) {
            if ((not this->age_restricted) and (this->vid_info.count(format) > 0)) {
                extract::apply_descrambler(this->vid_info, format);
            }
            extract::apply_descrambler(this->player_config_args, format);

            if (this->js.empty()) {
                if (this->embed_html.empty()) {
                    this->embed_html = request::http_get(this->embed_url);
                }
                this->js_url = extract::js_url(this->embed_html);
                this->js = request::http_get(this->js_url);
            }

            extract::apply_signature(this->player_config_args, format, this->js);
            this->initialize_stream_formats(format);

        }

        // load the player_response object
        std::string _temp = this->player_config_args["player_response"];
        this->player_response = nlohmann::json::parse(helpers::urlDecode(_temp));
        this->player_config_args.erase("player_response");
    }

    void YouTube::initialize_stream_formats(const std::string &fmt) {
        /*
         * Initialize Stream objects
         */
        nlohmann::json stream_manifest = this->player_config_args[fmt];
        for (nlohmann::json stream : stream_manifest) {
            Stream new_stream(
                    stream,
                    this->player_config_args
            );
            this->fmt_streams.emplace_back(new_stream);
        }
    }

    std::string YouTube::thumbnail_url() {
        /**
         * Get the highest resolution thumbnail url for the respective video.
         */
        nlohmann::json thumbnail_details = this->player_response["videoDetails"]["thumbnail"]["thumbnails"];

        if (!thumbnail_details.empty()) {
            return thumbnail_details[thumbnail_details.size() - 1]["url"];
        }
        return "https://img.youtube.com/vi/" + this->video_id + "/maxresdefault.jpg";
    }

    std::string YouTube::title() {
        /**
         * Get the title defined by the uploader
         */
        if (!this->player_config_args["title"].empty()) {
            return this->player_config_args["title"];
        }
        if (!this->player_response["videoDetails"]["title"].empty()) {
            return this->player_response["videoDetails"]["title"];
        }
        return "";
    }

    std::string YouTube::description() {
        /**
         * Get the description defined by the uploader
         */
        return this->player_response["videoDetails"]["shortDescription"];
        // TODO: implement _get_video_descr out of the extract namespace
    }

    float YouTube::rating() {
        /**
         * Get the rating for the video
         */
        return this->player_response["videoDetails"]["averageRating"];
    }

    int YouTube::length() {
        /**
         * Get the duration in seconds of the video
         */
        if (!this->player_config_args["length_seconds"].empty()) {
            return this->player_config_args["length_seconds"];
        }
        if (!this->player_response["videoDetails"]["lengthSeconds"].empty()) {
            return std::stoi((std::string) this->player_response["videoDetails"]["lengthSeconds"]);
        }
        return 0;
    }

    int YouTube::duration() {
        /**
         * Get the duration in seconds of the video
         */
        return this->length();
    }

    int YouTube::views() {
        /**
         * Get the viewcount of the video
         */
        return std::stoi((std::string) this->player_response["videoDetails"]["viewCount"]);
    }

    std::string YouTube::author() {
        /*
         * Get the uploader of the video
         */
        if (!this->player_response["videoDetails"]["author"].empty()) {
            return this->player_response["videoDetails"]["author"];
        }
        return "unknown";
    }

    std::string YouTube::uploader() {
        /**
         * Get the uploader of the video
         */
        return this->author();
    }

    StreamQuery YouTube::streams() const {
        return StreamQuery(this->fmt_streams);
    }

}