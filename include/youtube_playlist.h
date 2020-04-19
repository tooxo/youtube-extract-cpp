//
// Created by till on 17.04.20.
//

#include <string>

#include "helpers.h"
#include "exceptions.h"
#include "request.h"

#include <curl/curl.h>
#include "json.hpp"

namespace youtube {
    class Playlist {
    public:
        explicit Playlist(std::string &url, bool full_url = false) {
            this->url = url;
            this->id = extract_id(url);
            this->full_url = full_url;
            this->fill_video_urls();
        }

        std::vector<std::pair<std::string, std::string>> videos;
        int skipped_songs = 0;

    private:
        std::string url;
        std::string id;
        bool full_url;

        static std::string extract_id(const std::string &url) {
            try {
                return helpers::mapUrlParams(url)["list"];
            }
            catch (exceptions::RegexMatchError &e) {
                return url;
            }
        }

        static std::string fetch_content(std::string &url);

        void fill_video_urls();

    };

    void Playlist::fill_video_urls() {
        std::string raw_url = url + "&pbj=1";
        nlohmann::json data_store;
        std::string raw = fetch_content(raw_url);
        nlohmann::json js = nlohmann::json::parse(raw);
        try {
            data_store = js[1]["response"]["contents"]["twoColumnBrowseResultsRenderer"]["tabs"][0]["tabRenderer"]
            ["content"]["sectionListRenderer"]["contents"][0]["itemSectionRenderer"]["contents"][0]
            ["playlistVideoListRenderer"];
        } catch (nlohmann::json::exception &e) {
            std::cout << e.id << " " << e.what() << std::endl;
            throw exceptions::ExtractError();
        }
        for (nlohmann::json song : data_store["contents"]) {
            try {
                if (this->full_url) {
                    this->videos.emplace_back(
                            std::pair<std::string, std::string>{song["playlistVideoRenderer"]["title"]["simpleText"],
                                                                std::string("https://youtube.com/watch?v=").append(
                                                                        song["playlistVideoRenderer"]["videoId"])});
                } else {
                    this->videos.emplace_back(
                            std::pair<std::string, std::string>{song["playlistVideoRenderer"]["title"]["simpleText"],
                                                                song["playlistVideoRenderer"]["videoId"]});
                }
            } catch (nlohmann::json::exception &e) {
                skipped_songs++;
            }
        }

        std::string continuation_url;
        std::string continuation_raw;
        nlohmann::json continuation_json;

        while (data_store.contains("continuations")) {
            try {
                std::string cont = data_store["continuations"][0]["nextContinuationData"]["continuation"];
                std::string itct = data_store["continuations"][0]["nextContinuationData"]["clickTrackingParams"];
                continuation_url =
                        std::string("https://youtube.com/browse_ajax?ctoken=").append(cont).append(
                                "&continuation=").append(cont).append("&itct=").append(itct);
                continuation_raw = fetch_content(continuation_url);
                continuation_json = nlohmann::json::parse(continuation_raw);
                data_store = continuation_json[1]["response"]["continuationContents"]["playlistVideoListContinuation"];

                for (nlohmann::json song : data_store["contents"]) {
                    try {
                        if (this->full_url) {
                            this->videos.emplace_back(
                                    std::pair<std::string, std::string>{
                                            song["playlistVideoRenderer"]["title"]["simpleText"],
                                            std::string("https://youtube.com/watch?v=").append(
                                                    song["playlistVideoRenderer"]["videoId"])});
                        } else {
                            this->videos.emplace_back(
                                    std::pair<std::string, std::string>{
                                            song["playlistVideoRenderer"]["title"]["simpleText"],
                                            song["playlistVideoRenderer"]["videoId"]});
                        }
                    } catch (nlohmann::json::exception &e) {
                        skipped_songs++;
                    }
                }
            } catch (nlohmann::json::exception &e) {
                std::cout << e.id << " " << e.what() << std::endl;
                throw exceptions::ExtractError();
            }
        }
    }

    std::string Playlist::fetch_content(std::string &url) {
        auto curl = curl_easy_init();
        if (curl) {
            struct curl_slist *chunk = nullptr;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            chunk = curl_slist_append(chunk, "accept-language: en-US,en");
            chunk = curl_slist_append(chunk, "x-youtube-client-name:1");
            chunk = curl_slist_append(chunk, "x-youtube-client-version:2.20200312.05.00");

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, request::writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cout << curl_easy_strerror(res);
            }
            curl_easy_cleanup(curl);
            free(chunk);
            return response;
        }
        return "";
    }


}