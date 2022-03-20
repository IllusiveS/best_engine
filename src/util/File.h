#pragma once

#include <string>
#include <stdint.h>
#include <filesystem>
#include <fstream>

class Directory
{
public:
	static const std::string getProjectDir();
};

class File
{
public:
	static File loadFileFromProject(const std::string& fileloc);

	File() = default;
	File(File&& otherFile);
	File(File& otherFile) = default;
	File(const std::string & fileloc);
	~File();

	int64_t getModifiedTimestamp() const;

	File& operator=(File&& other) = default;
	File& operator=(const File&) = delete;
private:


private:
	std::filesystem::path path;
public:
	void* fileData;
	uint64_t fileSize;
};
