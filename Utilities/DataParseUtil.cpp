#include "DataParseUtil.h"
#include "boost/filesystem.hpp"
#include "boost/dll.hpp"
#include <iostream>

DataParseUtil::DataParseUtil()
{
}

std::string DataParseUtil::GetFileName(std::string filePath)
{
	boost::filesystem::path p(filePath);
	return p.stem().string();
}

std::string DataParseUtil::GetFileExtension(std::string filePath)
{
	boost::filesystem::path p(filePath);
	return p.extension().string();
}

std::string DataParseUtil::GetFileDirectory(std::string filePath)
{
	boost::filesystem::path p(filePath);
	return p.parent_path().string();
}

std::vector<std::string> DataParseUtil::GetFilesInDirectory(std::string dir)
{
	std::vector<std::string> files;
	for (auto& x : boost::filesystem::directory_iterator(dir))
	{
		files.push_back(x.path().string());
	}
	return files;
}

std::vector<std::string> DataParseUtil::GetFilesInDirectory(std::string dir, std::string ext)
{
	std::vector<std::string> files;
	for (auto& x : boost::filesystem::directory_iterator(dir))
	{
		if (x.path().extension().string() == ext)
		{
			files.push_back(x.path().string());
		}
	}
	return files;
}

bool DataParseUtil::DoesFileExist(std::string filePath)
{
	return boost::filesystem::exists(filePath);
}

bool DataParseUtil::DoesDirectoryExist(std::string dir)
{
	return boost::filesystem::is_directory(dir);
}

std::string DataParseUtil::GetExecutablePath()
{
	return boost::dll::program_location().string();
}

void DataParseUtil::CopyFileFromTo(std::string sourcePath, std::string destinationPath, bool overrideFile)
{
	boost::filesystem::path sourcePath_(sourcePath);

	boost::filesystem::path destinationPath_(destinationPath);
	boost::filesystem::copy_option option = overrideFile ? boost::filesystem::copy_option::overwrite_if_exists : boost::filesystem::copy_option::fail_if_exists;
	boost::filesystem::copy_file(sourcePath_, destinationPath_, option);
}

bool DataParseUtil::MakeDir(std::string directory)
{
    bool res = false;
    try
    {
        boost::filesystem::path p(directory);
        res = boost::filesystem::create_directory(p);
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cout << e.what() << '\n';
    }
    return res;
}

DataParseUtil::~DataParseUtil()
{
}
