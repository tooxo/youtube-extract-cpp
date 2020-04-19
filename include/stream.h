//
// Created by till on 12.04.20.
//

#pragma once

#include "extract.h"
#include "itags.h"
#include <string>

namespace youtube {
    static const int FILTER_URL = 1;
    static const int FILTER_ITAG = 2;
    static const int FILTER_MIME = 3;
    static const int FILTER_TYPE = 4;
    static const int FILTER_SUBTYPE = 5;
    static const int FILTER_VIDEOCODEC = 6;
    static const int FILTER_AUDIOCODEC = 7;
    static const int FILTER_OTF = 8;
    static const int FILTER_BITRATE = 9;
    static const int FILTER_CODECS = 10;
    static const int FILTER_DASH = 11;
    static const int FILTER_ABR = 12;
    static const int FILTER_FPS = 13;
    static const int FILTER_RESOLUTION = 14;
    static const int FILTER_3D = 15;
    static const int FILTER_HDR = 16;
    static const int FILTER_LIVE = 17;

    class Stream {
    public:
        Stream(nlohmann::json &stream, nlohmann::json &player_config_args) {
            this->url = stream["url"];
            this->itag = stream["itag"];

            std::string _type = stream["type"];
            std::pair<std::string, std::vector<std::string>> mc = extract::mime_type_codec(_type);
            this->mime_type = mc.first;
            this->codecs = mc.second;

            int pos = this->mime_type.find_first_of('/');
            this->type = this->mime_type.substr(0, pos);
            this->subtype = this->mime_type.substr(pos + 1, this->mime_type.size());

            std::pair<std::string, std::string> parsed_codecs = this->parse_codecs();
            this->video_codec = parsed_codecs.first;
            this->audio_codec = parsed_codecs.second;

            this->is_otf = stream["is_otf"];
            this->bitrate = stream["bitrate"];

            std::unordered_map<std::string, std::string> itag_profile = itags::get_format_profile(this->itag);
            // Extract the itag_profile and convert them back from string variables into their respective types
            this->is_dash = itag_profile["is_dash"] == "true";
            this->abr = itag_profile["abr"];
            std::string raw_fps = itag_profile["fps"];
            this->fps = std::stoi(raw_fps);
            this->resolution = itag_profile["resolution"];
            this->is_3d = itag_profile["is_3d"] == "true";
            this->is_hdr = itag_profile["is_hdr"] == "true";
            this->is_live = itag_profile["is_live"] == "true";

            this->player_config_args = player_config_args;
        }

        std::string url;
        int itag;

        std::string mime_type;
        std::vector<std::string> codecs;

        std::string type;
        std::string subtype;

        std::string video_codec;
        std::string audio_codec;

        bool is_otf;

        int bitrate;

        bool is_dash;
        std::string abr;
        int fps;
        std::string resolution;
        bool is_3d;
        bool is_hdr;
        bool is_live;

        __attribute__((unused)) nlohmann::json player_config_args;

        // Property Functions
        bool is_adaptive() const;

        bool is_progressive() const;

        bool includes_audio_track() const;

        bool includes_video_track() const;

        int expiration() const;

    private:
        std::pair<std::string, std::string> parse_codecs();
    };

    std::pair<std::string, std::string> Stream::parse_codecs() {
        std::string video;
        std::string audio;

        if (not this->is_adaptive()) {
            video = this->codecs[0];
            audio = this->codecs[1];
        } else if (this->includes_video_track()) {
            video = this->codecs[0];
        } else if (this->includes_audio_track()) {
            audio = this->codecs[0];
        }
        return std::pair<std::string, std::string>(video, audio);
    }

    bool Stream::is_progressive() const {
        return not this->is_adaptive();
    }

    bool Stream::is_adaptive() const {
        return (this->codecs.size() % 2) == 1;
    }

    bool Stream::includes_audio_track() const {
        return this->is_progressive() or this->type == "audio";
    }

    bool Stream::includes_video_track() const {
        return this->is_progressive() or this->type == "video";
    }

    __attribute__((unused)) int Stream::expiration() const {
        /**
         * Shows the unix timestamp where the stream expires.
         */
        return stoi(helpers::mapUrlParams(this->url.substr(this->url.find_first_of('?')))["expire"]);
    }
}