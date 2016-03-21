/*
 * Util.cpp
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */

#define BOOST_FILESYSTEM_VERSION 3

#include "Util.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

int to_int(std::string str)
{
	std::stringstream ss;
	int i;
	
	ss << str;
	ss >> i;
	
	ss.clear();
	ss.str("");
	
	return i;
}

std::list<std::string> readFileLine(std::string filePath)
{
	std::ifstream ifs(filePath.c_str());
		
	if (ifs.fail())
	{
		throw "FileNotFoundException";
	}
	
	std::list<std::string> lines;																						// 行データ保持リスト
	std::string buf;																										// 行データ読み込みバッファ
	
	while (getline(ifs, buf))
	{
		lines.push_back(buf);
	}
	ifs.close();
	
	return lines;
}

std::string readFileLineAsString(std::string filePath)
{
	std::ifstream ifs(filePath.c_str());
			
	if (ifs.fail())
	{
		throw "FileNotFoundException";
	}
	
	std::string returnStr;																								// 行データ保持テキスト
	std::string buf;																										// 行データ読み込みバッファ
	
	while (getline(ifs, buf))
	{
		returnStr += "\n" + buf;
	}
	ifs.close();
	
	return returnStr;
}

bool isPathExist(std::string path)
{
	struct stat st;
	int ret = stat(path.c_str(), &st);
	
	if (ret == 0)
	{
		return true;
	}
	
	return false;
}

bool createDir(std::string path)
{
	boost::filesystem::path dir(path);
	if (boost::filesystem::create_directory(dir))
	{
		return true;
	}
	
	return false;
}

void getFileList(std::string dirPath, std::vector<std::string> *buffer)
{
	boost::filesystem::path filepath;
	boost::filesystem::path dir(dirPath);
	boost::filesystem::directory_iterator end;
	for (boost::filesystem::directory_iterator p(dir); p != end; ++p)
	{
		filepath = *p;
		if (!boost::filesystem::is_directory(filepath))
		{
			buffer->push_back(filepath.leaf().string());
		}
	}
}
