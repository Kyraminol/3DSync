#ifndef MODULES_DROPBOX_H
#define MODULES_DROPBOX_H

#include <string>
#include <vector>
#include <map>

#include "../utils/curl.h"

class Dropbox{
    public:
        Dropbox(std::string token);
        ~Dropbox(){};
        void upload(std::map<std::pair<std::string, std::string>, std::vector<std::string>> paths);
    private:
        std::string _token;
        Curl _curl;
};


#endif
