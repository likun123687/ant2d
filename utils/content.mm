#import <Foundation/Foundation.h>
#include <utils/Content.h>

namespace ant2d {

Content::Content()
{
    NSBundle* bundle = [NSBundle mainBundle];
    /* this returns the exedir for non-bundled  and the resourceDir for bundled apps */
    const char* base = [[bundle resourcePath] fileSystemRepresentation];
    _assetPath = std::string(base) + "/";
}

bool Content::isFileExist(const std::string& filePath)
{
    if (filePath[0] != '/') {
        std::string path = filePath;
        std::string file;
        size_t pos = path.find_last_of("/");
        if (pos != std::string::npos) {
            file = path.substr(pos + 1);
            path = path.substr(0, pos + 1);
            NSString* fullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                                 ofType:nil
                                                            inDirectory:[NSString stringWithUTF8String:path.c_str()]];
            if (fullPath != nil) {
                return true;
            }
        }
    } else {
        // Search path is an absolute path.
        NSFileManager* fileManager = [NSFileManager defaultManager];
        if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:filePath.c_str()]]) {
            return true;
        }
    }
    return false;
}

std::string Content::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename)
{
    if (directory[0] != '/') {
        NSString* fullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filename.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:directory.c_str()]];
        if (fullPath != nil) {
            return [fullPath UTF8String];
        }
    } else {
        std::string fullPath = directory + filename;
        // Search path is an absolute path.
        NSFileManager* fileManager = [NSFileManager defaultManager];
        if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]) {
            return fullPath;
        }
    }
    return "";
}

bool Content::isAbsolutePath(const std::string& strPath)
{
    NSString* path = [NSString stringWithUTF8String:strPath.c_str()];
    return [path isAbsolutePath] ? true : false;
}

}
