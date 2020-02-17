#ifndef MODULES_DROPBOX_H
#define MODULES_DROPBOX_H

#include <string>
#include <vector>

#include "../utils/curl.h"

class Dropbox{
    public:
        Dropbox(std::string token);
        ~Dropbox(){};
        void upload(std::string basePath, std::vector<std::string> paths);
    private:
        std::string _token;
        Curl _curl;
};


#endif
