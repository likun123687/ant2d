#include <iostream>
#include <utils/Content.h>

int main()
{
    auto aa = SharedContent.getAssertPath();
    auto bb = SharedContent.getFullPath("testfile.txt");
    auto cc = SharedContent.getFullPath("asset/testfile1.txt");
    auto dd = SharedContent.isFileExist(SharedContent.getAssertPath());

    auto file_content = SharedContent.loadFile("testfile.txt");
    std::cout<< file_content.first << file_content.second <<std::endl;

    //std::string full_path = SharedContent.getFullPath("aaa");
    std::cout<< aa << " " <<bb << " " << cc <<" " << dd <<std::endl;
    return 0;
}
