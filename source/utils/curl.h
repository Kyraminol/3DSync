#ifndef UTILS_CURL_H
#define UTILS_CURL_H

#include <string>

#include <curl/curl.h>

class Curl{
    public:
        Curl();
        ~Curl();
        void setURL(std::string URL);
        void setHeaders(curl_slist *headers);
        void setReadData(void *pointer);
        int perform();
    private:
        CURL *_curl;
        static size_t _read_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
        static size_t _write_callback(void *data, size_t size, size_t nmemb, void *userdata);
};


#endif
