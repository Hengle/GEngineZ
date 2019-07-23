#pragma once
#include <string>
#include <fstream>
#include <streambuf>

namespace z {

constexpr int MAX_PATH_LEN = 4096;
class FilePath {

public:
	static char SEP;

	FilePath();
	FilePath(std::string const&);
	FilePath(const char* str);

	void ToAbsolute();

	bool IsExist() const;
	bool IsDirectory() const;
	bool IsFile() const;


	FilePath ParentDir() const;
	std::string FileName() const;
	std::string FileExt() const;
	std::string FileNameNoExt() const;

	FilePath Connect(FilePath const& other) const;

	FilePath operator/(FilePath const& other) const {
		return Connect(other);
	}

	friend std::ostream& operator << (std::ostream& os, FilePath const& f) {
		return os << f.m_path;
	}

	std::string Str() { return m_path; }
	operator std::string() const { return m_path; }


private:
	std::string ConvertWithCurPlatform(std::string const& str);

	std::string m_path;
	bool m_is_abs;

#if defined(_WIN32)
	std::wstring StrConvertToWide(std::string const& s) const;
	std::string StrConvertToMulti(std::wstring const& s) const;
#endif

};

class FileReader {
public:
	FileReader(std::string path);
	~FileReader();

	std::string ReadAll();

protected:
	FilePath m_fp;
};

}
