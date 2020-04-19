//
// Created by till on 09.04.20.
//

#ifndef YOUTUBEDL_CPP_EXTRACT_H
#define YOUTUBEDL_CPP_EXTRACT_H


#endif //YOUTUBEDL_CPP_EXTRACT_H
#pragma once

#include "string"
#include "iostream"
#include "list"
#include "json.hpp"
#include "helpers.h"
#include "cipher.h"


namespace youtube {
    namespace extract {
        bool is_age_restricted(std::string &watch_html) {
            std::string pattern = "og:restrictions:age";
            return watch_html.find(pattern) != std::string::npos;
        }

        std::string video_id(std::string &url) {
            std::string pattern = R"((?:v=|\/)([0-9A-Za-z_-]{11}).*)";
            return helpers::regex_search(pattern, url);
        }

        std::string video_info_url(std::string &video_id, std::string &watch_url) {
            std::string s = "https://youtube.com/get_video_info?video_id=" + video_id;
            return s;
        }

        std::string video_info_url_age_restricted(std::string &video_id, std::string &embed_html) {
            std::string sts;
            std::string pattern = R"("sts"\s*:\s*(\d+))";
            try {
                sts = helpers::regex_search(pattern, embed_html);
            } catch (youtube::exceptions::RegexMatchError &e) {
                sts = "";
            }
            std::string yt_eurl = "https://youtube.googleapis.com/v/" + video_id;
            std::string unencoded = "video_id=" + video_id + "&eurl=" + helpers::urlEncode(yt_eurl) + "&sts=" + sts;
            return "https://youtube.com/get_video_info?" + unencoded;
        }

        std::pair<std::string, std::vector<std::string>> mime_type_codec(std::string &mime_type_codec) {
            std::regex pattern(R"((\w+\/\w+)\;\scodecs=\"([a-zA-Z-0-9.,\s]*)\")");
            std::smatch matcher;
            if (std::regex_search(mime_type_codec, matcher, pattern)) {
                std::string mime_type = matcher.str(1);
                std::string codec_string = matcher.str(2);

                std::vector<std::string> codecs;
                int pos;
                while ((pos = codec_string.find(',')) != std::string::npos) {
                    codecs.emplace_back(
                            codec_string.substr(0, pos)
                    );
                    codec_string.erase(0, pos + 1);
                }
                codecs.emplace_back(
                        (std::string) helpers::strstrip(const_cast<char *>(codec_string.c_str()))
                );

                return std::pair<std::string, std::vector<std::string>>(mime_type, codecs);
            }
            throw exceptions::RegexMatchError();
        }

        nlohmann::json get_ytplayer_config(std::string &html) {
            std::vector<std::string> config_patterns(
                    {
                            R"(;ytplayer\.config\s*=\s*({.*?});)",
                            R"(;yt\.setConfig\(\{'PLAYER_CONFIG':\s*({.*})}\);)",
                            R"(;yt\.setConfig\(\{'PLAYER_CONFIG':\s*({.*})(,'EXPERIMENT_FLAGS'|;))"
                    }
            );

            for (const std::string &pattern : config_patterns) {
                try {
                    std::string config = helpers::regex_search(pattern, html);
                    return nlohmann::json::parse(config);
                } catch (exceptions::RegexMatchError &ignored) {}
            }
            throw exceptions::RegexMatchError();
        }

        std::string js_url(std::string &html) {
            std::string base_js = get_ytplayer_config(html)["assets"]["js"];
            return "https://youtube.com" + base_js;
        }

        void apply_signature(nlohmann::json &config_args, std::string &fmt, std::string &js) {
            cipher::Cipher cipher(js);

            nlohmann::json &stream_manifest = config_args[fmt];
            for (nlohmann::json &stream : stream_manifest) {
                std::string url;

                url = stream["url"];
                if (url.find("signature") != std::string::npos or (
                        not stream.contains("s") and (
                                url.find("&sig=") != std::string::npos or url.find("&lsig=") != std::string::npos)
                )
                        ) {
                    // Signature was already there.
                    continue;
                }

                if (url.empty()) {
                    continue;
                }

                std::string stream_secret = stream["s"];

                std::string signature = cipher.get_signature(stream_secret);

                url += "&sig=" + signature;
                stream["url"] = url;
            }

        }

        void apply_descrambler(nlohmann::json &stream_data, std::string &key) {
            std::string otf_type = "FORMAT_STREAM_TYPE_OTF";
            std::string url_enc = "url_encoded_fmt_stream_map";

            if (key == url_enc and stream_data.count(url_enc) == 0) {
                nlohmann::json player_response = nlohmann::json::parse(
                        helpers::urlDecode(stream_data["player_response"]));
                nlohmann::json formats = player_response["streamingData"]["formats"];
                for (const nlohmann::json &format : player_response["streamingData"]["adaptiveFormats"]) {
                    formats.emplace_back(format);
                }

                bool cipher = false;
                for (nlohmann::json format_item : formats) {
                    nlohmann::json j;
                    if (format_item["url"].empty()) {
                        // shitty solution, but works
                        cipher = true;
                        break;
                    }

                    if (format_item["type"] == otf_type) {
                        // For now, OTF urls seems to be unsupported by pytube aswell as youtube-dl
                        // Also there is no visible way to extract them from the response
                        continue;
                    }

                    j["url"] = format_item["url"];
                    j["type"] = format_item["mimeType"];
                    j["quality"] = format_item["quality"];
                    j["itag"] = format_item["itag"];
                    j["bitrate"] = format_item["bitrate"];
                    j["is_otf"] = format_item["type"] == otf_type;

                    stream_data[key].emplace_back(j);
                }
                if (cipher) {
                    for (nlohmann::json format : formats) {
                        std::map<std::string, std::string> cipher_url = helpers::mapUrlParams(format["cipher"], true);
                        nlohmann::json j;

                        std::string url = cipher_url["url"];
                        for (std::pair<std::string, std::string> param : cipher_url) {
                            if (param.first == "url") { continue; }
                            url += "&" + param.first + "=" + param.second;
                        }

                        j["url"] = url;
                        j["s"] = cipher_url["s"];
                        j["type"] = format["mimeType"];
                        j["quality"] = format["quality"];
                        j["itag"] = format["itag"];
                        j["bitrate"] = format["bitrate"];
                        j["is_otf"] = (format["type"] == otf_type);
                        stream_data[key].emplace_back(j);
                    }
                }
            } else {
                std::cout << "Not yet implemented." << std::endl;
                throw exceptions::VideoUnavailable();
                /**
                 * Note: There a no current urls that fall into this case, so I think this will be
                 * unimplemented before I find a test-able url.
                 */
            }
        }
    }
}