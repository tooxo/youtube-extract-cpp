//
// Created by till on 12.04.20.
//

#pragma once
#ifndef YOUTUBEDL_CPP_ITAGS_H
#define YOUTUBEDL_CPP_ITAGS_H

#endif //YOUTUBEDL_CPP_ITAGS_H

#include <algorithm>

inline const char *BoolToString(bool b) {
    return b ? "true" : "false";
}

namespace youtube {
    namespace itags {
        static std::unordered_map<int, std::pair<std::string, std::string>> ITAGS(
                {
                        {5,   std::pair<std::string, std::string>("240p", "64kbps")},
                        {6,   std::pair<std::string, std::string>("270p", "64kbps")},
                        {13,  std::pair<std::string, std::string>("144p", "")},
                        {17,  std::pair<std::string, std::string>("144p", "24kbps")},
                        {18,  std::pair<std::string, std::string>("360p", "96kbps")},
                        {22,  std::pair<std::string, std::string>("720p", "192kbps")},
                        {34,  std::pair<std::string, std::string>("360p", "128kbps")},
                        {35,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {36,  std::pair<std::string, std::string>("240p", "")},
                        {37,  std::pair<std::string, std::string>("1080p", "192kbps")},
                        {38,  std::pair<std::string, std::string>("3072p", "192kbps")},
                        {43,  std::pair<std::string, std::string>("360p", "128kbps")},
                        {44,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {45,  std::pair<std::string, std::string>("720p", "192kbps")},
                        {46,  std::pair<std::string, std::string>("1080p", "192kbps")},
                        {59,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {78,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {82,  std::pair<std::string, std::string>("360p", "128kbps")},
                        {83,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {84,  std::pair<std::string, std::string>("720p", "192kbps")},
                        {85,  std::pair<std::string, std::string>("1080p", "192kbps")},
                        {91,  std::pair<std::string, std::string>("144p", "48kbps")},
                        {92,  std::pair<std::string, std::string>("240p", "48kbps")},
                        {93,  std::pair<std::string, std::string>("360p", "128kbps")},
                        {94,  std::pair<std::string, std::string>("480p", "128kbps")},
                        {95,  std::pair<std::string, std::string>("720p", "256kbps")},
                        {96,  std::pair<std::string, std::string>("1080p", "256kbps")},
                        {100, std::pair<std::string, std::string>("360p", "128kbps")},
                        {101, std::pair<std::string, std::string>("480p", "192kbps")},
                        {102, std::pair<std::string, std::string>("720p", "192kbps")},
                        {132, std::pair<std::string, std::string>("240p", "48kbps")},
                        {151, std::pair<std::string, std::string>("720p", "24kbps")},
                        {133, std::pair<std::string, std::string>("240p", "")},
                        {134, std::pair<std::string, std::string>("360p", "")},
                        {135, std::pair<std::string, std::string>("480p", "")},
                        {136, std::pair<std::string, std::string>("720p", "")},
                        {137, std::pair<std::string, std::string>("1080p", "")},
                        {138, std::pair<std::string, std::string>("2160p", "")},
                        {160, std::pair<std::string, std::string>("144p", "")},
                        {167, std::pair<std::string, std::string>("360p", "")},
                        {168, std::pair<std::string, std::string>("480p", "")},
                        {169, std::pair<std::string, std::string>("720p", "")},
                        {170, std::pair<std::string, std::string>("1080p", "")},
                        {212, std::pair<std::string, std::string>("480p", "")},
                        {218, std::pair<std::string, std::string>("480p", "")},
                        {219, std::pair<std::string, std::string>("480p", "")},
                        {242, std::pair<std::string, std::string>("240p", "")},
                        {243, std::pair<std::string, std::string>("360p", "")},
                        {244, std::pair<std::string, std::string>("480p", "")},
                        {245, std::pair<std::string, std::string>("480p", "")},
                        {246, std::pair<std::string, std::string>("480p", "")},
                        {247, std::pair<std::string, std::string>("720p", "")},
                        {248, std::pair<std::string, std::string>("1080p", "")},
                        {264, std::pair<std::string, std::string>("1440p", "")},
                        {266, std::pair<std::string, std::string>("2160p", "")},
                        {271, std::pair<std::string, std::string>("1440p", "")},
                        {272, std::pair<std::string, std::string>("2160p", "")},
                        {278, std::pair<std::string, std::string>("144p", "")},
                        {298, std::pair<std::string, std::string>("720p", "")},
                        {299, std::pair<std::string, std::string>("1080p", "")},
                        {302, std::pair<std::string, std::string>("720p", "")},
                        {303, std::pair<std::string, std::string>("1080p", "")},
                        {308, std::pair<std::string, std::string>("1440p", "")},
                        {313, std::pair<std::string, std::string>("2160p", "")},
                        {315, std::pair<std::string, std::string>("2160p", "")},
                        {330, std::pair<std::string, std::string>("144p", "")},
                        {331, std::pair<std::string, std::string>("240p", "")},
                        {332, std::pair<std::string, std::string>("360p", "")},
                        {333, std::pair<std::string, std::string>("480p", "")},
                        {334, std::pair<std::string, std::string>("720p", "")},
                        {335, std::pair<std::string, std::string>("1080p", "")},
                        {336, std::pair<std::string, std::string>("1440p", "")},
                        {337, std::pair<std::string, std::string>("2160p", "")},
                        {139, std::pair<std::string, std::string>("", "48kbps")},
                        {140, std::pair<std::string, std::string>("", "128kbps")},
                        {141, std::pair<std::string, std::string>("", "256kbps")},
                        {171, std::pair<std::string, std::string>("", "128kbps")},
                        {172, std::pair<std::string, std::string>("", "256kbps")},
                        {249, std::pair<std::string, std::string>("", "50kbps")},
                        {250, std::pair<std::string, std::string>("", "70kbps")},
                        {251, std::pair<std::string, std::string>("", "160kbps")},
                        {256, std::pair<std::string, std::string>("", "")},
                        {258, std::pair<std::string, std::string>("", "")},
                        {325, std::pair<std::string, std::string>("", "")},
                        {328, std::pair<std::string, std::string>("", "")}
                }
        );

        static const std::vector<int> HDR({330, 331, 332, 333, 334, 335, 336, 337});
        static const std::vector<int> SIXTY_FPS({298, 299, 302, 303, 308, 315, 330, 331, 332, 333, 334, 335, 336, 337});
        static const std::vector<int> THREE_DIMENSIONAL({82, 83, 84, 85, 100, 101, 102});
        static const std::vector<int> LIVE({91, 92, 93, 94, 95, 96, 132, 151});
        static const std::vector<int> DASH_MP4_VIDEO({133, 134, 135, 136, 137, 138, 160, 212, 264, 266, 298, 299});
        static const std::vector<int> DASH_MP4_AUDIO({139, 140, 141, 256, 258, 325, 328});
        static const std::vector<int> DASH_WEBM_VIDEO(
                {167, 168, 169, 170, 218, 219, 278, 242, 243, 244, 245, 246, 247, 248, 271, 272, 302, 303, 308, 313,
                 315});
        static const std::vector<int> DASH_WEBM_AUDIO({171, 172, 249, 250, 251});

        std::unordered_map<std::string, std::string> get_format_profile(const int &itag) {
            std::string res;
            std::string bitrate;
            if (ITAGS.count(itag) > 0) {
                std::pair<std::string, std::string> _ = itags::ITAGS[itag];
                res = _.first;
                bitrate = _.second;
            }
            return std::unordered_map<std::string, std::string>(
                    {
                            {
                                    "resolution", res
                            },
                            {
                                    "abr",        bitrate
                            },
                            {
                                    "is_live",    BoolToString(
                                    std::count(itags::LIVE.begin(),
                                               itags::LIVE.end(), itag))
                            },
                            {
                                    "is_3d",
                                                  BoolToString(std::count(
                                                          itags::THREE_DIMENSIONAL.begin(),
                                                          itags::THREE_DIMENSIONAL.end(),
                                                          itag))
                            },
                            {
                                    "is_hdr",
                                                  BoolToString(std::count(
                                                          itags::HDR.begin(),
                                                          itags::HDR.end(),
                                                          itag))
                            },
                            {
                                    "fps",
                                                  std::count(
                                                          itags::SIXTY_FPS.begin(),
                                                          itags::SIXTY_FPS.end(),
                                                          itag) ? "60" : "30"
                            },
                            {
                                    "is_dash",
                                                  BoolToString(
                                                          (
                                                                  std::count(
                                                                          itags::DASH_MP4_AUDIO.begin(),
                                                                          itags::DASH_MP4_AUDIO.end(),
                                                                          itag) or
                                                                  std::count(
                                                                          itags::DASH_MP4_VIDEO.begin(),
                                                                          itags::DASH_MP4_VIDEO.end(),
                                                                          itag) or
                                                                  std::count(
                                                                          itags::DASH_WEBM_AUDIO.begin(),
                                                                          itags::DASH_WEBM_AUDIO.end(),
                                                                          itag) or
                                                                  std::count(
                                                                          itags::DASH_WEBM_VIDEO.begin(),
                                                                          itags::DASH_WEBM_VIDEO.end(),
                                                                          itag)
                                                          )
                                                  )
                            }
                    }
            );
        }

    }
}