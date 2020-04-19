//
// Created by till on 10.04.20.
//

#pragma once

#include "exception"

namespace youtube {
    namespace exceptions {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
        struct ExtractError : public std::exception {
            /**
             * Not used
             * @return
             */
            [[nodiscard]] const char *what() const noexcept override {
                return "ExtractError";
            }
        };
#pragma clang diagnostic pop

        struct RegexMatchError : public std::exception {
            [[nodiscard]] const char *what() const noexcept override {
                return "RegexMatchError";
            }
        };

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
        struct LiveStreamError : public std::exception {
            /**
             * Not used
             * @return
             */
            [[nodiscard]] const char *what() const noexcept override {
                return "This video is a live stream.";
            }
        };
#pragma clang diagnostic pop

        struct VideoUnavailable : public std::exception {
            std::string text;

            explicit VideoUnavailable(const std::string &text = "This video is unavailable.") {
                this->text = text;
            }

            [[nodiscard]] const char *what() const noexcept override {
                return this->text.c_str();
            }
        };

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
        struct HtmlParseError : public std::exception {
            /**
             * Not used.
             */
            [[nodiscard]] const char *what() const noexcept override {
                return "ExtractError";
            }
        };
#pragma clang diagnostic pop
    }
}