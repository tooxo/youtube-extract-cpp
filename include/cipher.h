//
// Created by till on 10.04.20.
//

/*
#ifndef YOUTUBEDL_CPP_CIPHER_H
#define YOUTUBEDL_CPP_CIPHER_H

#endif //YOUTUBEDL_CPP_CIPHER_H
*/
#pragma once

#include <map>
#include <list>
#include <string>
#include <regex>
#include <unordered_map>
#include "exceptions.h"
#include "helpers.h"

namespace youtube {
    namespace cipher {

        std::string get_initial_function_name(const std::string &js) {
            std::vector<std::string> function_patterns
                    ({
                             R"(\b[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\b[a-zA-Z0-9]+\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\b(?P<sig>[a-zA-Z0-9$]{2})\s*=\s*function\(\s*a\s*\)\s*{\s*a\s*=\s*a\.split\(\s*""\s*\))",
                             R"((?P<sig>[a-zA-Z0-9$]+)\s*=\s*function\(\s*a\s*\)\s*{\s*a\s*=\s*a\.split\(\s*""\s*\))",
                             R"((["\'])signature\1\s*,\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\.sig\|\|(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(yt\.akamaized\.net/\)\s*\|\|\s*.*?\s*[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*(?:encodeURIComponent\s*\()?\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\b[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\b[a-zA-Z0-9]+\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\bc\s*&&\s*a\.set\([^,]+\s*,\s*\([^)]*\)\s*\(\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\bc\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*\([^)]*\)\s*\(\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                             R"(\bc\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*\([^)]*\)\s*\(\s*(?P<sig>[a-zA-Z0-9$]+)\()",
                     });

            for (const std::string &raw_pattern : function_patterns) {
                RE2 re(raw_pattern);
                std::string group_1;
                if (RE2::PartialMatch(js, re, &group_1)) {
                    return group_1;
                }
            }
            throw youtube::exceptions::RegexMatchError();
        }

        std::vector<std::string> get_transform_plan(const std::string &js) {
            std::string name = get_initial_function_name(js);
            std::string pattern = name + R"(=function\(\w\){[a-z=\.\(\"\)]*;(.*);(?:.+)})";
            std::string result = helpers::regex_search(pattern, js);

            std::string token;
            std::vector<std::string> output;
            size_t pos;
            while ((pos = result.find(';')) != std::string::npos) {
                token = result.substr(0, pos);
                output.emplace_back(token);
                result.erase(0, pos + 1);
            }
            output.emplace_back(result); // add the remaining
            return output;
        }

        std::vector<std::string> get_transform_object(const std::string &js, std::string &var) {
            // std::string pattern = R"(var\s)" + var + R"(=\{([^^]+\})\};var\s)";
            std::string pattern = R"(var\s)" + var + R"(=\{([^?]+\})\};)";
            RE2 re(pattern); // check regex type

            std::string result;
            if (RE2::PartialMatch(js, re, &result)) {
                helpers::replace_string_in_place(result, "\n", "");
                std::vector<std::string> output;
                size_t pos;
                std::string token;
                while ((pos = result.find("},")) != std::string::npos) {
                    token = result.substr(0, pos + 1);
                    output.emplace_back(token);
                    result.erase(0, pos + 2);
                }

                output.emplace_back(result); // add the remaining contents to the list
                return output;
            }
            throw youtube::exceptions::RegexMatchError();
        }


/*
 * The following 4 functions are js operations transcribed into cpp
 */

        namespace js {

            void reverse(std::vector<std::string> &arr, const int &b) {
                /*
                 * Reverses a list.
                 */
                std::reverse(arr.begin(), arr.end());
            }

            void splice(std::vector<std::string> &arr, const int &b) {
                /*
                 * Splices b chars from an offset from b
                 */
                arr.erase(arr.begin() + b, arr.begin() + b * 2);
            }


            void swap(std::vector<std::string> &arr, const int &b) {
                /*
                 * Swap positions at b modulus the list length
                 */
                auto r = b % arr.size();
                std::vector<std::string> output;

                output.emplace_back(arr[r]);
                for (int l = 1; l < r; ++l) {
                    output.emplace_back(arr[l]);
                }
                output.emplace_back(arr[0]);
                for (unsigned long l = r + 1; l < arr.size(); ++l) {
                    output.emplace_back(arr[l]);
                }
                arr = output;
            }
        }

        std::function<void(std::vector<std::string> &, int)> map_functions(std::string &func_name) {
            std::regex rev(R"(\{\w\.reverse\(\)\})");
            std::regex spl(R"(\{\w\.splice\(0,\w\)\})");
            std::regex swp(R"(\{var\s\w=\w\[0\];\w\[0\]=\w\[\w\%\w.length\];\w\[\w\]=\w\})");
            std::regex swp2(R"(\{var\s\w=\w\[0\];\w\[0\]=\w\[\w\%\w.length\];\w\[\w\%\w.length\]=\w\})");

            std::smatch match;
            if (std::regex_search(func_name, match, rev)) {
                return js::reverse;
            }
            if (std::regex_search(func_name, match, spl)) {
                return js::splice;
            }
            if (std::regex_search(func_name, match, swp) or std::regex_search(func_name, match, swp2)) {
                return js::swap;
            }
            throw youtube::exceptions::RegexMatchError();
        }

        std::unordered_map<std::string, std::function<void(std::vector<std::string> &, int)>>
        get_transform_map(const std::string &js, std::string &var) {
            std::vector<std::string> transform_object = get_transform_object(js, var);

            std::unordered_map<std::string, std::function<void(std::vector<std::string> &, int)>> map;
            for (const std::string &obj: transform_object) {
                size_t pos = obj.find_first_of(':');
                std::string name = obj.substr(0, pos);
                std::string function = obj.substr(pos + 1, obj.length());

                map[name] = map_functions(function);
            }
            return map;
        }


        class Cipher {

        public:
            explicit Cipher(const std::string &js) {
                this->js = js;
                this->transform_plan = get_transform_plan(this->js);
                std::string var = this->transform_plan[0].substr(0, this->transform_plan[0].find_first_of('.'));
                this->transform_map = get_transform_map(this->js, var);
            }

            std::string get_signature(std::string &ciphered_signature);

        private:
            std::string js;
            std::vector<std::string> transform_plan;
            std::unordered_map<std::string, std::function<void(std::vector<std::string> &,
                                                               int)>> transform_map;
            std::regex js_func_regex = std::regex(R"(\w+\.(\w+)\(\w,(\d+)\))");

            std::pair<std::string, int> parse_function(std::string &js_func);
        };

        std::string Cipher::get_signature(std::string &ciphered_signature) {
            std::vector<std::string> signature;
            for (char &l : ciphered_signature) {
                signature.emplace_back(std::string(1, l));
            }

            for (std::string js_func : this->transform_plan) {
                std::pair<std::string, int> parsed_function = this->parse_function(
                        js_func); // this->parse_function(js_func)
                std::string name = parsed_function.first;
                int argument = parsed_function.second;

                std::function<void(std::vector<std::string> &, int)> function = this->transform_map[name];

                function(signature, argument);
            }

            std::string return_string;

            for (const std::string &s : signature) {
                return_string += s;
            }

            return return_string;
        }

        std::pair<std::string, int> Cipher::parse_function(std::string &js_func) {
            std::smatch matcher;
            if (std::regex_search(js_func, matcher, this->js_func_regex)) {
                std::string fn_name = matcher.str(1);
                std::string fn_arg = matcher.str(2);
                return std::pair<std::string, int>(fn_name, std::stoi(fn_arg));
            }
            throw youtube::exceptions::RegexMatchError();
        }
    }
}