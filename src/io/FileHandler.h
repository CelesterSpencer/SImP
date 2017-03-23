//
// Created by Windrian on 19.03.2017.
//

#ifndef SRCCMAKE_FILEHANDLER_H
#define SRCCMAKE_FILEHANDLER_H

// windows
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <iostream>

#include "util/Singleton.h"

class FileHandler: public Singleton<FileHandler> {
    friend class Singleton<FileHandler>;
public:
    int openFilePathDialog(std::string& filePath);
private:
    FileHandler(){};
    ~FileHandler() {};
};


#endif //SRCCMAKE_FILEHANDLER_H
