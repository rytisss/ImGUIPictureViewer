#ifndef DATAPARSEUTIL_H
#define DATAPARSEUTIL_H

#include <string>
#include <vector>

class DataParseUtil
{
public:
	//
	DataParseUtil();
	// Get file name from path
	static std::string GetFileName(std::string filePath);
	// Get file extension from path
	static std::string GetFileExtension(std::string filePath);
	// Get file directory
	static std::string GetFileDirectory(std::string filePath);
	// Get file in directory
	static std::vector<std::string> GetFilesInDirectory(std::string dir);
	// Get file in directory with extension
	static std::vector<std::string> GetFilesInDirectory(std::string dir, std::string ext);
	// Check if file exist
	static bool DoesFileExist(std::string filePath);
	// Doest directory exist
	static bool DoesDirectoryExist(std::string dir);
	// Get executable location
	static std::string GetExecutablePath();
	//copy file
	static void CopyFileFromTo(std::string sourcePath, std::string destinationPath, bool overrideFile);
    // makes output directory
    static bool MakeDir(std::string directory);
	//
	~DataParseUtil();
};

#endif //DATAPARSEUTIL_H