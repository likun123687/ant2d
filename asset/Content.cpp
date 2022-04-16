#include <filesystem>
namespace fs = std::filesystem;

Content::Content()
{
	_assetPath = fs::current_path().string();
}

bool Content::isFileExist(String filePath)
{
	string strPath = filePath;
	if (!Content::isAbsolutePath(strPath))
	{
		strPath.insert(0, _assetPath);
	}
	return GetFileAttributesA(strPath.c_str()) != -1 ? true : false;
}

bool Content::isAbsolutePath(String strPath)
{
	if (strPath.size() > 2
		&& ((strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z'))
		&& strPath[1] == ':')
	{
		return true;
	}
	return false;
}

string Content::getFullPathForDirectoryAndFilename(String directory, String filename)
{
	auto rootPath = fs::path(Content::isAbsolutePath(directory) ? Slice::Empty : _assetPath);
	string fullPath = (rootPath / directory.toString() / filename.toString()).string();
	if (!Content::isFileExist(fullPath))
	{
		fullPath.clear();
	}
	return fullPath;
}

string Content::getFullPath(String filename)
{
	Slice targetFile = filename;
	targetFile.trimSpace();

	while (targetFile.size() > 1 &&
		(targetFile.back() == '\\' || targetFile.back() == '/'))
	{
		targetFile.skipRight(1);
	}

	if (Content::isAbsolutePath(targetFile))
	{
		return targetFile;
	}

	auto it  = _fullPathCache.find(targetFile);
	if (it != _fullPathCache.end())
	{
		return it->second;
	}

	string path, file, fullPath;
	auto fname = fs::path(targetFile.begin(), targetFile.end()).lexically_normal();
	for (const auto& searchPath : _searchPaths)
	{
		std::tie(path, file) = splitDirectoryAndFilename((fs::path(searchPath) / fname).string());
		fullPath = Content::getFullPathForDirectoryAndFilename(path, file);
		if (!fullPath.empty())
		{
			_fullPathCache[fname.string()] = fullPath;
			return fullPath;
		}
	}

	std::tie(path, file) = splitDirectoryAndFilename(targetFile);
	fullPath = Content::getFullPathForDirectoryAndFilename(path, file);
	if (!fullPath.empty())
	{
		_fullPathCache[targetFile] = fullPath;
		return fullPath;
	}

	return targetFile;
}

std::pair<OwnArray<Uint8>,size_t> Content::loadFile(String filename)
{
	Sint64 size = 0;
	Uint8* data = Content::_loadFileUnsafe(filename, size);
	return {OwnArray<Uint8>(data), s_cast<size_t>(size)};
}

Uint8* Content::_loadFileUnsafe(String filename, Sint64& size)
{
	if (filename.empty()) return nullptr;
	string fullPath = Content::getFullPath(filename);
    auto file_io = cfile(fullPath.c_str(), mode::read | mode::binary);

    size = fs::file_size(fullPath)
	Uint8* buffer = new Uint8[s_cast<size_t>(size)];
    file_io.fread<Uint8>(buffer, s_cast<size_t>(size));
	return buffer;
}
