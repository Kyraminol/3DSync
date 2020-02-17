#include "curl.h"


Curl::Curl(){
    curl_global_init(CURL_GLOBAL_ALL);
    _curl = curl_easy_init();
    if(!_curl) printf("Failed to init libcurl.\n");
    curl_easy_setopt(_curl, CURLOPT_USERAGENT, "3DSync/0.1.0");
    curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, 50L);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_curl, CURLOPT_PIPEWAIT, 1L);
    #ifdef DEBUG
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
    #endif
}

Curl::~Curl(){
    curl_easy_cleanup(_curl);
}

void Curl::setURL(std::string URL){
    curl_easy_setopt(_curl, CURLOPT_URL, URL.c_str());
}

void Curl::setHeaders(curl_slist *headers){
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
}

void Curl::setReadData(void *pointer){
    curl_easy_setopt(_curl, CURLOPT_READFUNCTION, _read_callback);
    curl_easy_setopt(_curl, CURLOPT_READDATA, pointer);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(_curl, CURLOPT_POST, 1L);
}

int Curl::perform(){
    CURLcode rescode = curl_easy_perform(_curl);
    const char *res = curl_easy_strerror(rescode);
    printf("Curl result: %s\n", res);
    return rescode;
}

size_t Curl::_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata){
    FILE *readhere = (FILE *)userdata;
    curl_off_t nread;
    size_t retcode = fread(ptr, size, nmemb, readhere);
    nread = (curl_off_t)retcode;
    printf("*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
    return retcode;
}
