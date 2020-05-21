#include "dropbox.h"
#include <3ds.h>

Dropbox::Dropbox(std::string token) : _token(token){
}

void Dropbox::upload(std::map<std::pair<std::string, std::string>, std::vector<std::string>> paths){
    for(auto item : paths){
        for(auto path : item.second){
            printf("Uploading %s\n", (item.first.first + path).c_str());
            FILE *file = fopen((item.first.first + path).c_str(), "rb");
            std::string args("Dropbox-API-Arg: {\"path\":\"/" + item.first.second + path + "\",\"mode\": \"add\",\"mute\": false,\"strict_conflict\": false}");
            std::string auth("Authorization: Bearer " + _token);
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, auth.c_str());
            headers = curl_slist_append(headers, args.c_str());
            headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
            headers = curl_slist_append(headers, "Expect:");
            _curl.setURL(std::string("https://content.dropboxapi.com/2/files/upload"));
            _curl.setHeaders(headers);
            _curl.setReadData((void *)file);
            _curl.perform();
            fclose(file);
            printf("\n");
        }
    }
}
