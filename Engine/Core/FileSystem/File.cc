#pragma once
#include "File.h"
#include <Core/Common/Logger.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace z {

#if defined(_WIN32)
char FilePath::SEP = '\\';

std::wstring FilePath::StrConvertToWide(std::string const& s) const {
	int size = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
	std::wstring wret(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wret[0], size);
	return wret;
}

std::string FilePath::StrConvertToMulti(std::wstring const& s) const {
	std::string ret;
	if (!s.empty()) {
		int size = WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0, NULL, NULL);
		ret.resize(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(), &ret[0], size, NULL, NULL);
	}
	return ret;
}

void FilePath::ToAbsolute() {
	std::wstring wstr = StrConvertToWide(m_path);
	std::wstring wret(MAX_PATH_LEN, '\0');
	auto lent = GetFullPathNameW(wstr.c_str(), MAX_PATH_LEN, &wret[0], NULL);
	if (lent) {
		m_path = StrConvertToMulti(wret.substr(0, lent));
		m_is_abs = true;
	}
}

bool FilePath::IsExist() const {
	std::wstring wstr = StrConvertToWide(m_path);
	return GetFileAttributesW(wstr.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool FilePath::IsDirectory() const {
	std::wstring wstr = StrConvertToWide(m_path);
	auto ret = GetFileAttributesW(wstr.c_str());
	return ret != INVALID_FILE_ATTRIBUTES && (ret & FILE_ATTRIBUTE_DIRECTORY);
}

bool FilePath::IsFile() const {
	std::wstring wstr = StrConvertToWide(m_path);
	auto ret = GetFileAttributesW(wstr.c_str());
	return ret != INVALID_FILE_ATTRIBUTES && (ret & FILE_ATTRIBUTE_DIRECTORY) == 0;
}
#else
char FilePath::SEP = '/';

void FilePath::ToAbsolute() {
	char temp[MAX_PATH_LEN];
	if (realpath(m_path.c_str(), temp)) {
		m_path = std::string(temp);
		m_is_abs = true;
	}
}

bool FilePath::IsExist() const {
	struct stat st;
	return stat(m_path.c_str(), &st) == 0;
}

bool FilePath::IsDirectory() const {
	struct stat st;
	if (stat(m_path.c_str(), &st)) {
		return S_ISDIR(st.st_mode);
	}
	return false;
}

bool FilePath::IsFile() const {
	struct stat st;
	if (stat(m_path.c_str(), &st)) {
		return S_ISREG(st.st_mode);
	}
	return false;
}

#endif

FilePath::FilePath() :
	m_path("."),
	m_is_abs(false) {
}

FilePath::FilePath(std::string const& path) {
	m_path = ConvertWithCurPlatform(path);
#if defined(_WIN32)
	m_is_abs = (m_path.length() > 1 && m_path[1] == ':');
#else
	m_is_abs = (m_path.length() > 0 && m_path[0] == '/');
#endif
}

FilePath::FilePath(const char* str) :
	FilePath(std::string(str)) {
}

std::string FilePath::ConvertWithCurPlatform(std::string const& str) {
	std::string ret(str);
	for (auto &c : ret) {
		if ((c == '\\' || c == '/') && c != SEP) {
			c = SEP;
		}
	}
	return ret;
}

FilePath FilePath::Connect(FilePath const& other) const {
	if (other.m_is_abs) {
		Log<LWARN>("Can't connect a abosulte path...");
		return *this;
	}
	return FilePath(m_path + FilePath::SEP + other.m_path);
}

FilePath FilePath::ParentDir() const {
	auto pos = m_path.rfind(FilePath::SEP);
	if (pos == std::string::npos) {
		return FilePath();
	}
	return FilePath(m_path.substr(0, pos));
}

std::string FilePath::FileName() const {
	auto pos = m_path.rfind(FilePath::SEP);
	if (pos == std::string::npos) {
		return m_path;
	}
	return m_path.substr(pos + 1);
}

std::string FilePath::FileExt() const {
	auto filename = FileName();
	auto pos = filename.rfind(".");
	if (pos == std::string::npos) {
		return "";
	}
	return filename.substr(pos + 1);
}

std::string FilePath::FileNameNoExt() const {
	auto filename = FileName();
	auto pos = filename.rfind(".");
	if (pos == std::string::npos) {
		return filename;
	}
	return filename.substr(0, pos);
}


FileReader::FileReader(std::string path) {
	m_fp = path;
}

FileReader::~FileReader() {
}

std::string FileReader::ReadAll() {
	std::ifstream t(m_fp);
	return std::string(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
}

}