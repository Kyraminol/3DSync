#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>

#include <3ds.h>

#include <curl/curl.h>

#include "libs/inih/INIReader/INIReader.h"
#include "modules/dropbox.h"

#define DEBUG 1


std::vector<std::string> recurse_dir(std::string basepath, std::string additionalpath=""){
    std::vector<std::string> paths;
    DIR *dir;
    struct dirent *ent;
    std::string path(basepath + additionalpath);
    if((dir = opendir(path.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            std::string readpath(path + "/" + ent->d_name);
            std::vector<std::string> recurse = recurse_dir(basepath, additionalpath + "/" + ent->d_name);
            paths.insert(paths.end(), recurse.begin(), recurse.end());
        }
    } else {
        if(additionalpath != "") paths.push_back(additionalpath);
        else printf("Folder /3ds/Checkpoint/saves not found\n");
    }
    closedir(dir);
    return paths;
}

bool componentsInit(){
    bool result = true;
    gfxInitDefault();

    consoleInit(GFX_BOTTOM, NULL);
    printf(CONSOLE_RED "\n 3DSync " VERSION_STRING " by Kyraminol" CONSOLE_RESET);
    printf("\n\n\n\n\n\n  Sync your saves with another 3DS,\n   a PC or even a cloud.");
    printf("\n\n\n\n\n\n Commit: " CONSOLE_BLUE REVISION_STRING CONSOLE_RESET);

    consoleInit(GFX_TOP, NULL);
    printf("Initializing components...\n\n");
    
    APT_SetAppCpuTimeLimit(30);
    cfguInit();
    romfsInit();
    pxiDevInit();
    amInit();
    acInit();

    u32* socketBuffer = (u32*)memalign(0x1000, 0x100000);
    if(socketBuffer == NULL){printf("Failed to create socket buffer.\n"); result = false;}
    if(socInit(socketBuffer, 0x100000)){printf("socInit failed.\n"); result = false;}

    httpcInit(0);
    sslcInit(0);
    return result;
}

void componentsExit(){
    sslcExit();
    httpcExit();
    socExit();
    acExit();
    pxiDevExit();
    romfsExit();
    cfguExit();
    gfxExit();
}


int main(int argc, char** argv){
    if(!componentsInit()) componentsExit();

    INIReader reader("/3ds/3DSync/3DSync.ini");

    if(reader.ParseError() < 0){
        printf("Can't load configuration\n");
    } else {
        std::string dropboxToken = reader.Get("Dropbox", "token", "");
        if(dropboxToken != ""){
            Dropbox dropbox(dropboxToken);
            std::string basePath = "/3ds/Checkpoint/saves";
            std::vector<std::string> paths = recurse_dir(basePath);
            if((int)paths.size() > 0) dropbox.upload(basePath, paths);
            else printf("Nothing to upload\n");
        } else {
            printf("Can't load Dropbox token from 3DSync.ini\n");
        }
    }

    printf("\n\nPress START to exit...");
    while (aptMainLoop()){
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    componentsExit();
    return 0;
}
