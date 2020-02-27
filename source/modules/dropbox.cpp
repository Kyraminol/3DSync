#include "dropbox.h"

Dropbox::Dropbox(std::string token) : _token(token){
}

void Dropbox::upload(std::string basePath, std::vector<std::string> paths){
    for(std::vector<std::string>::const_iterator it = paths.begin() ; it != paths.end(); ++it){
        std::string path(basePath + *it);
        printf("Uploading %s\n  ", path.c_str());
        FILE *file = fopen(path.c_str(), "rb");
        std::string args = "Dropbox-API-Arg: {\"path\":\"" + *it + "\",\"mode\": \"add\",\"mute\": false,\"strict_conflict\": false}";
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
