//
// Created by till on 09.04.20.
//
#pragma once

#include <string>
#include <curl/curl.h>

namespace youtube {
    namespace request {

        size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data) {
            data->append((char *) ptr, size * nmemb);
            return size * nmemb;
        }

        std::string http_get(std::string &url) {
            /*
             * Used to request via http-get
             */

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

                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

                std::string response;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

                CURLcode res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    std::cout << curl_easy_strerror(res);
                }
                curl_easy_cleanup(curl);
                return response;
            }
            return "";
        }
    }
}