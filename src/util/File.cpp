#include "File.h"

#include <iostream>
#include <fstream>
#include <cassert>

File File::loadFileFromProject(const std::string& fileloc)
{
	File file(Directory::getProjectDir() + fileloc);
	return file;
}

File::File(const std::string& fileloc)
{
	std::string finalPath = Directory::getProjectDir() + fileloc;

	std::fstream myfile;
	myfile.open(finalPath, std::fstream::in | std::fstream::binary);

	assert(myfile.is_open());

	path = std::filesystem::path { finalPath };

	//Read size
	myfile.seekg(0, std::ios::end);
	fileSize = myfile.tellg();
	myfile.seekg(0, std::ios::beg);

	//Read data
	fileData = new char[fileSize];

	const char* buffer = (const char*) fileData;

	myfile.read((char*) fileData, fileSize);
	myfile.close();
}

File::File(File&& otherFile)
	:path(otherFile.path), fileData(otherFile.fileData), fileSize(otherFile.fileSize)
{

}

File::~File()
{
	delete[] fileData;
}

int64_t File::getModifiedTimestamp() const
{
	return std::filesystem::last_write_time(path).time_since_epoch().count();
}

const std::string Directory::getProjectDir()
{
	static std::string dir = "";
	return dir;
}
