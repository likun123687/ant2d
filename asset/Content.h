
class Content
{
public:
	string getFullPath(String filename);
	std::pair<OwnArray<Uint8>,size_t> loadFile(String filename);
    bool isAbsolutePath(String strPath);
	virtual ~Content();
protected:
	Content();
	bool isFileExist(String filePath);
	string getFullPathForDirectoryAndFilename(String directory, String filename);
	void copyFileUnsafe(String srcFile, String dstFile);
	bool isFileExist(String filePath);
	bool isPathFolder(String filePath);
private:
	Uint8* _loadFileUnsafe(String filename, Sint64& size);
	string _assetPath;
	vector<string> _searchPaths;

}

#define SharedContent \
    Ant2d::Singleton<Ant2d::Content>::shared()
