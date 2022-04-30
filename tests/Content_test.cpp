#include <iostream>
#include <utils/Content.h>
#include <string>
#include <filesystem>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("test_content")
{
    namespace fs = std::filesystem;
    auto asset_path = SharedContent.getAssetPath();
    std::cout << "asset path "<< asset_path <<std::endl;

    auto file_not_exist = "assets/face.png333";
    REQUIRE( SharedContent.getFullPath(file_not_exist) == std::string() );

    auto file_exist = "assets/face.png";
    auto full_path_file_exist = SharedContent.getFullPath(file_exist);
    REQUIRE(!full_path_file_exist.empty());

    auto test_path = fs::path(asset_path) / file_exist;
    REQUIRE(fs::path(full_path_file_exist) == test_path);

    REQUIRE(SharedContent.isAbsolutePath("C:\\testfile.txt") == true);
    REQUIRE(SharedContent.isAbsolutePath("testfile\\aa\\bb.txt") == false);

    REQUIRE(SharedContent.isFileExist(file_exist) == true);
    REQUIRE(SharedContent.isFileExist(file_not_exist) == false);

    auto file_content = SharedContent.loadFile("assets/file1.txt");
    auto result = std::string(reinterpret_cast<char *>(file_content.first.get()), file_content.second);
    auto expect_result = std::string("111122223333");
    REQUIRE(result == expect_result);
}

