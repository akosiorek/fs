/*
 * File.h
 *
 *  Created on: Aug 6, 2014
 *      Author: Adam Kosiorek
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <vector>

class File {
public:
	File();
	File(const std::string& path);
	virtual ~File();

	bool isOpen() const;
	void create();
	void create(const std::string& path);
	void open();
	void open(const std::string& path);
	void close();
	size_t size() const;
	const std::string& path() const;

	void write(const std::string& str, size_t offset = 0);
	std::string read(size_t size, size_t offset = 0) const;

	void writeChar(char c, size_t offset);
	const char readChar(size_t offset) const;
	size_t getMaxSize() const;
	void setMaxSize(size_t maxSize);

private:
	size_t size_;
	bool isOpen_;
	std::string path_;
	FILE* file_;
	mutable std::vector<char> buffer_;
	size_t maxSize_;

	void checkAndUpdateSize();


};

#endif /* FILE_H_ */
