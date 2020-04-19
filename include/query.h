//
// Created by till on 17.04.20.
//

namespace youtube {
    class StreamQuery {
    private:
        std::vector<Stream> streams;
    public:
        explicit StreamQuery(const std::vector<Stream> &streams) {
            this->streams = streams;
        }

        __attribute__((unused)) StreamQuery sort(const int &attribute);

        __attribute__((unused)) StreamQuery filter(const int &attribute, const std::string &parameter, bool clone);

        __attribute__((unused)) StreamQuery filter(const int &attribute, const int &parameter, bool clone);

        __attribute__((unused)) StreamQuery filter(const int &attribute, const bool &parameter, bool clone);

        __attribute__((unused)) Stream *first();

        __attribute__((unused)) Stream *last();

        __attribute__((unused)) int length();

        __attribute__((unused)) int size();

        __attribute__((unused)) std::vector<Stream> all();

        __attribute__((unused)) Stream *get_by_itag(const int &itag);

        __attribute__((unused)) Stream *get_by_resolution(const std::string &resolution);

        __attribute__((unused)) StreamQuery has_audio(bool clone = false);

        __attribute__((unused)) StreamQuery has_video(bool clone = false);

        __attribute__((unused)) StreamQuery only_video(bool clone = false);

        __attribute__((unused)) StreamQuery only_audio(bool clone = false);

        __attribute__((unused)) __gnu_cxx::__normal_iterator<Stream *, std::vector<Stream>> begin();

        __attribute__((unused)) __gnu_cxx::__normal_iterator<Stream *, std::vector<Stream>> end();
    };

    bool compareString(std::string a, std::string b) {
        std::transform(a.begin(), a.end(), a.begin(), [](unsigned char m) { return std::tolower(m); });
        std::transform(b.begin(), b.end(), b.begin(), [](unsigned char m) { return std::tolower(m); });
        if (a.size() <= b.size()) {
            for (int c : a) {
                for (int d : b) {
                    if (d == c) {
                        break;
                    }
                    return c < d;
                }
            }
            return true;
        } else {
            for (int c : b) {
                for (int d : a) {
                    if (d == c) {
                        break;
                    }
                    return c < d;
                }
            }
            return true;
        }
    }

    struct FunctionRunner {
    public:
        int attribute;

        explicit FunctionRunner(const int &filter_attribute) {
            this->attribute = filter_attribute;
        }

        bool operator()(Stream &a, Stream &b) const {
            switch (this->attribute) {
                case FILTER_URL:
                    return compareString(a.url, b.url);
                case FILTER_ITAG:
                    return a.itag < b.itag;
                case FILTER_MIME:
                    return compareString(a.mime_type, b.mime_type);
                case FILTER_OTF:
                    return a.is_otf > b.is_otf;
                case FILTER_LIVE:
                    return a.is_live > b.is_live;
                case FILTER_3D:
                    return a.is_3d > b.is_3d;
                case FILTER_HDR:
                    return a.is_hdr > b.is_hdr;
                case FILTER_DASH:
                    return a.is_dash > b.is_dash;
                case FILTER_BITRATE:
                    return a.bitrate < b.bitrate;
                case FILTER_ABR:
                    return compareString(a.abr, b.abr);
                case FILTER_FPS:
                    return a.fps < b.fps;
                case FILTER_RESOLUTION:
                    if (a.resolution.empty()) {
                        return false;
                    }
                    if (b.resolution.empty()) {
                        return true;
                    }
                    return std::stoi(a.resolution.substr(0, a.resolution.length() - 2)) >
                           std::stoi(b.resolution.substr(0, b.resolution.length() - 2));
                default:
                    std::cout << "The Attribute " << this->attribute << " is not supported." << std::endl;
                    throw std::exception();
            }
        };


    };

    StreamQuery StreamQuery::sort(const int &attribute) {
        std::sort(this->streams.begin(), this->streams.end(), FunctionRunner(attribute));
        return *this;
    }

    StreamQuery StreamQuery::filter(const int &attribute, const std::string &parameter, bool clone = false) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            switch (attribute) {
                case FILTER_MIME:
                    if (s.mime_type == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_TYPE:
                    if (s.type == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_SUBTYPE:
                    if (s.subtype == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_VIDEOCODEC:
                    if (s.video_codec == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_AUDIOCODEC:
                    if (s.audio_codec == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_CODECS:
                    if (std::find(s.codecs.begin(), s.codecs.end(), parameter) != s.codecs.end()) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_ABR:
                    if (s.abr == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_RESOLUTION:
                    if (s.resolution == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                default:
                    std::cout << "The Attribute " << attribute << " is not supported with a string." << std::endl;
                    throw std::exception();
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    StreamQuery StreamQuery::filter(const int &attribute, const int &parameter, bool clone = false) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            switch (attribute) {
                case FILTER_BITRATE:
                    if (s.bitrate == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_FPS:
                    if (s.fps == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                default:
                    std::cout << "The Attribute " << attribute << " is not supported with a int." << std::endl;
                    throw std::exception();
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    StreamQuery StreamQuery::filter(const int &attribute, const bool &parameter, bool clone = false) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            switch (attribute) {
                case FILTER_OTF:
                    if (s.is_otf == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_DASH:
                    if (s.is_dash == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_3D:
                    if (s.is_3d == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_HDR:
                    if (s.is_hdr == parameter) {
                        filtered.emplace_back(s);
                    }
                    break;
                case FILTER_LIVE:
                    if (s.is_live == parameter) {
                        filtered.emplace_back(s);
                    }
                default:
                    std::cout << "The Attribute " << attribute << " is not supported with a bool." << std::endl;
                    throw std::exception();
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    Stream *StreamQuery::first() {
        if (this->length() > 0) {
            return &this->streams.at(0);
        }
        return nullptr;
    }

    Stream *StreamQuery::last() {
        if (this->length() > 0) {
            return &this->streams.at(this->length() - 1);
        }
        return nullptr;
    }

    int StreamQuery::length() {
        return this->streams.size();
    }

    int StreamQuery::size() {
        return this->streams.size();
    }

    std::vector<Stream> StreamQuery::all() {
        return this->streams;
    }

    __gnu_cxx::__normal_iterator<Stream *, std::vector<Stream>> StreamQuery::begin() {
        return this->streams.begin();
    }

    __gnu_cxx::__normal_iterator<Stream *, std::vector<Stream>> StreamQuery::end() {
        return this->streams.end();
    }

    Stream *StreamQuery::get_by_itag(const int &itag) {
        for (Stream &s : this->streams) {
            if (s.itag == itag) {
                return &s;
            }
        }
        return nullptr;
    }

    __attribute__((unused)) StreamQuery StreamQuery::has_video(bool clone) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            if (!s.video_codec.empty()) {
                filtered.emplace_back(s);
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    __attribute__((unused)) StreamQuery StreamQuery::has_audio(bool clone) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            if (!s.audio_codec.empty()) {
                filtered.emplace_back(s);
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    __attribute__((unused)) StreamQuery StreamQuery::only_audio (bool clone){
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            if (!s.audio_codec.empty() && s.audio_codec.empty()) {
                filtered.emplace_back(s);
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    __attribute__((unused)) StreamQuery StreamQuery::only_video(bool clone) {
        std::vector<Stream> filtered;
        for (Stream &s : this->streams) {
            if (!s.video_codec.empty() && s.video_codec.empty()) {
                filtered.emplace_back(s);
            }
        }
        if (clone) {
            return StreamQuery(filtered);
        }
        this->streams = filtered;
        return *this;
    }

    __attribute__((unused)) Stream *StreamQuery::get_by_resolution(const std::string &resolution) {
        return this->filter(youtube::FILTER_RESOLUTION, resolution).first();
    }
}