//
// Created by till on 10.04.20.
//

#pragma once

#include "regex"
#include "exceptions.h"
#include "re2.h"

namespace youtube {
    namespace helpers {
        std::string regex_search(const std::string &pattern, const std::string &search_string) {
            RE2 re(pattern);
            std::string matches;

            if (RE2::PartialMatch(search_string, re, &matches)) {
                return matches;
            }
            throw youtube::exceptions::RegexMatchError();
        }

        std::string urlDecode(std::string str) {
            std::string ret;
            char ch;
            int i, ii, len = str.length();

            for (i = 0; i < len; i++) {
                if (str[i] != '%') {
                    if (str[i] == '+')
                        ret += ' ';
                    else
                        ret += str[i];
                } else {
                    sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
                    ch = static_cast<char>(ii);
                    ret += ch;
                    i = i + 2;
                }
            }
            return ret;
        }

        std::string urlEncode(std::string &str) {
            std::string new_str;
            char c;
            int ic;
            const char *chars = str.c_str();
            char bufHex[10];
            int len = strlen(chars);

            for (int i = 0; i < len; i++) {
                c = chars[i];
                ic = c;
                // uncomment this if you want to encode spaces with +
                /*if (c==' ') new_str += '+';
                else */
                if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
                else {
                    sprintf(bufHex, "%X", c);
                    if (ic < 16)
                        new_str += "%0";
                    else
                        new_str += "%";
                    new_str += bufHex;
                }
            }
            return new_str;
        }

        std::map<std::string, std::string> mapUrlParams(std::string raw_params, bool decode = false) {
            // std::regex re("&?([^&]+)=([^&]+)");
            RE2 re("&?([^&=]+)=([^&]+)");
            std::string match1;
            std::string match2;
            std::map<std::string, std::string> output;
            while (RE2::PartialMatch(raw_params, re, &match1, &match2)) {
                if (decode) {
                    output[match1] = urlDecode(match2);
                } else {
                    output[match1] = match2;
                }
                unsigned long pos = raw_params.find(match2) + match2.length();
                raw_params.erase(0, pos);
            }
            return output;
        }

        char *strstrip(char *s) {
            /**
             * Strip strings from leading and tailing whitespace, extracted from the linux kernel
             */
            size_t size;
            char *end;

            size = strlen(s);

            if (!size)
                return s;

            end = s + size - 1;
            while (end >= s && isspace(*end))
                end--;
            *(end + 1) = '\0';

            while (*s && isspace(*s))
                s++;

            return s;
        }
        void replace_string_in_place(std::string &subject, const std::string &search,
                                     const std::string &replace) {
            // https://stackoverflow.com/a/14678852/10868935
            size_t pos = 0;
            while ((pos = subject.find(search, pos)) != std::string::npos) {
                subject.replace(pos, search.length(), replace);
                pos += replace.length();
            }
        }
    }
}