#import <Foundation/Foundation.h>
#include <utils/content.h>

namespace ant2d {

Content::Content()
{
    NSBundle* bundle = [NSBundle mainBundle];
    /* this returns the exedir for non-bundled  and the resourceDir for bundled apps */
    const char* base = [[bundle resourcePath] fileSystemRepresentation];
    asset_path_ = std::string(base) + "/";
}

bool Content::IsFileExist(const std::string& file_path)
{
    if (file_path[0] != '/') {
        std::string path = file_path;
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
        if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:file_path.c_str()]]) {
            return true;
        }
    }
    return false;
}

std::string Content::GetFullPathForDirectoryAndFilename(const std::string& directory, const std::string& file_name)
{
    if (directory[0] != '/') {
        NSString* fullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file_name.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:directory.c_str()]];
        if (fullPath != nil) {
            return [fullPath UTF8String];
        }
    } else {
        std::string fullPath = directory + file_name;
        // Search path is an absolute path.
        NSFileManager* fileManager = [NSFileManager defaultManager];
        if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]) {
            return fullPath;
        }
    }
    return "";
}

bool Content::IsAbsolutePath(const std::string& str_path)
{
    NSString* path = [NSString stringWithUTF8String:str_path.c_str()];
    return [path isAbsolutePath] ? true : false;
}

}
