#pragma once
/////////////////////////////////////////////////////////////////////
// Checkin.h -           Code Repository                           //
// ver 1.0                                                         //
// YUXUAN XING CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provide the class with function including:
* accept a package address and find the files in it
* check the version class and get the version number
* store those files in the repositorycore with version number appended
* confirm the status and the source folder
* Required Files:
* ---------------
* Version.h
* RepositoryCore.h
* Definitions.h
* Maintenance History:
* --------------------
* 3/6/2018 fisrt release
*/

#include "../CppNoSqlDb/DbCore/Definitions.h"
#include "../CppNoSqlDb/DbCore/DbCore.h"
#include "../Version/Version.h"
//#include "../DbCore/DbCore.h"
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <windows.h>

using namespace NoSqlDb;
template<typename P>
class Checkin {
public:
	using Vertype = std::unordered_map<std::string, char>;
	using iterator = typename Vertype::iterator;
	Checkin(DbCore<P>& Repo) : Repo_(Repo) {}
	static std::string getExt(const std::string& fileSpec);
	static std::string getName(const std::string& fileSpec, bool withExt = true);
	static std::string toLower(const std::string& src);
	static std::string fileSpec(const std::string& path, const std::string& name);
	static std::vector<std::string> getFiles(const std::string& path, const std::string& pattern);
	//repoElement<P> CreateMetadata(const std::string& pacname, const std::string status, const std::string author);
	char doversion(Version& version, const std::string pacname);
	std::string versionedname(char c, std::string name);
	void PushBack(const std::string& name, DbElement<P>& elem);
private:
	DbCore<P>& Repo_;
};

template<typename P>
void Checkin<P>::PushBack(const std::string& name, DbElement<P>& elem) {
	Repo_[name] = elem;
}

class FileSystemSearch
{
public:
	FileSystemSearch();
	~FileSystemSearch();
	std::string firstFile(const std::string& path, const std::string& pattern);
	std::string nextFile();
	std::string firstDirectory(const std::string& path, const std::string& pattern);
	std::string nextDirectory();
	void close();
private:
	HANDLE hFindFile;
	WIN32_FIND_DATAA FindFileData;
	WIN32_FIND_DATAA* pFindFileData;
};


FileSystemSearch::FileSystemSearch() : pFindFileData(&FindFileData) {}
FileSystemSearch::~FileSystemSearch() { ::FindClose(hFindFile); }
void FileSystemSearch::close() { ::FindClose(hFindFile); }


std::string FileSystemSearch::firstFile(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Checkin<std::string>::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextFile()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
	return "";
}
//----< find first file >--------------------------------------------------
std::string FileSystemSearch::firstDirectory(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Checkin<std::string>::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextDirectory()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
	return "";
}

template<typename P>
std::string Checkin<P>::getName(const std::string &fileSpec, bool withExt)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
	{
		pos = fileSpec.find_last_of("\\");
		if (pos >= fileSpec.length())
		{
			// no path prepended
			if (withExt)
				return fileSpec;
			else
			{
				// remove ext
				size_t pos = fileSpec.find(".");
				if (pos > fileSpec.size())
					return fileSpec;
				return fileSpec.substr(0, pos - 1);
			}
		}
	}
	if (withExt)
		return fileSpec.substr(pos + 1, fileSpec.length() - pos);
	else
	{
		// remove ext
		size_t pos2 = fileSpec.find(".", pos);
		if (pos2 > fileSpec.size())
			// no ext
			return fileSpec.substr(pos + 1);
		return fileSpec.substr(pos + 1, pos2 - pos - 1);
	}
}

template<typename P>
std::string Checkin<P>::getExt(const std::string& fileSpec)
{
	size_t pos1 = fileSpec.find_last_of('/');
	size_t pos2 = fileSpec.find_last_of('\\');
	size_t pos = fileSpec.find_last_of('.');
	// handle ../ or ..\\ with no extension
	if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
	{
		if (pos < min(pos1, pos2))
			return std::string("");
	}
	// only . is extension delimiter
	if (0 <= pos && pos < fileSpec.length())
		return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
	return std::string("");
}

template<typename P>
std::string Checkin<P>::toLower(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += tolower(src[i]);
	return temp;
}
template<typename P>
std::vector<std::string> Checkin<P>::getFiles(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> files;
	FileSystemSearch fss;
	std::string file = fss.firstFile(path, pattern);
	if (file.size() == 0)
		return files;
	files.push_back(file);
	while (true)
	{
		file = fss.nextFile();
		if (file.size() == 0)
			return files;
		files.push_back(file);
	}
	return files;
}

template<typename P>
std::string Checkin<P>::fileSpec(const std::string &path, const std::string &name)
{
	std::string fs;
	size_t len = path.size();
	if (path[len - 1] == '/' || path[len - 1] == '\\')
		fs = path + name;
	else
	{
		if (path.find("/") < path.size())
			fs = path + "/" + name;
		else if (path.find("\\") < path.size())
			fs = path + "\\" + name;
		else
			fs = path + "/" + name;
	}
	return fs;
}

template<typename P>
char Checkin<P>::doversion(Version& version, const std::string pacname) {
	char c;
	iterator iter = version.vercore().find(pacname);
	if (iter == version.vercore().end()) {
		c = '1';
		version.vercore()[pacname] = c;
	}
	else {
		version.vercore()[pacname] = version.vercore()[pacname] + 1;
		c = version.vercore()[pacname];
	}
	return c;
}

template<typename P>
std::string Checkin<P>::versionedname(char c, std::string name) {
	std::string newname;
	newname = name + c;
	return newname;
}
