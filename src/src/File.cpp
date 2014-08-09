/*
 * File.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: Adam Kosiorek
 */

#include "File.h"
#include "Utils.h"
#include <glog/logging.h>
#include <cstdio>

using namespace std;

File::File()
	: size_(0), isOpen_(false), path_(""), file_(nullptr), buffer_(512, 0),
	  maxSize_(0) {}

File::File(const std::string& path)
	: File() {
	path_ = path;

	try {
		open();
	} catch (const std::runtime_error& err) {
		create();
	}
}

File::~File() {
	if(isOpen()) {
		close();
	}
}

bool File::isOpen() const {
	return isOpen_;
}

void File::create() {

	if(isOpen_) {
		THROW_LOGIC("This file is already opened");
	}

	if(path_.size() == 0) {
		THROW_LOGIC("Cannot create a file under an empty path");
	}

	file_ = fopen(path_.c_str(), "wbx+");
	if(!file_) {
		THROW_RUNTIME("Couldn't create a file at " + path_);
	}
	isOpen_ = true;
	size_ = 0;
	maxSize_ = 0;
}

void File::create(const std::string& path) {
	path_ = path;
	create();
}

void File::open() {
	if(isOpen_) {
		THROW_LOGIC("This file is already opened");
	}
	if(path_.size() == 0) {
		THROW_LOGIC("Cannot open a file under an empty path");
	}

	file_ = fopen(path_.c_str(), "rb+");
	if(!file_) {
		THROW_RUNTIME("Couldn't open the file at " + path_);
	}
	isOpen_ = true;
	fseek(file_, 0L, SEEK_END);
	size_ = ftell(file_);
	fseek(file_, 0L, SEEK_SET);
}

void File::open(const std::string& path) {

	path_ = path;
	open();
}

void File::close() {

	if(!isOpen_) return;

	isOpen_ = false;
	size_ = 0;
	fclose(file_);
}

size_t File::size() const {
	return size_;
}

const std::string& File::path() const {
	return path_;
}

void File::write(const std::string& str, size_t offset) {
	if(!isOpen_) {
		THROW_LOGIC("Cannot write to a close file");
	}
	if(maxSize_ > 0 && str.size() + offset > maxSize_) {
		THROW_RUNTIME("Size + offset > maxSize");
	}
	if(ftell(file_) != offset) {
		fseek(file_, offset, SEEK_SET);
	}
	size_t dataSize = fwrite(str.c_str(), 1, str.size(), file_);
	if(dataSize != str.size()) {
		THROW_RUNTIME("Couldn't write " + std::to_string(str.size()) + " bytes to file " + path_);
	}
	checkAndUpdateSize();
}

std::string File::read(size_t size, size_t offset) const {
	if(!isOpen_) {
		THROW_LOGIC("Cannot read from a close file");
	}
	if(offset >= size_) {
		THROW_RUNTIME("Cannot read beyond the end of file");
	}

	if(buffer_.size() < size) {
		buffer_.resize(size);
	}
	if(ftell(file_) != offset) {
		fseek(file_, offset, SEEK_SET);
	}
	size_t readBytes = fread(buffer_.data(), 1, size, file_);
	return std::string(buffer_.data(), readBytes);

}

void File::checkAndUpdateSize() {
	size_t current = ftell(file_);
	if(current > size_) {
		size_ = current;
	}
}

void File::writeChar(char c, size_t offset) {
	if(!isOpen_) {
		THROW_LOGIC("Cannot write to a close file");
	}
	if(maxSize_ > 0 && offset >= maxSize_) {
		THROW_RUNTIME("Offset >= maxSize");
	}
	fseek(file_, offset, SEEK_SET);
	fputc(static_cast<int>(c), file_);
	if(offset > size_) {
		size_ = offset + 1;
	}
}

const char File::readChar(size_t offset) const {
	if(!isOpen_) {
		THROW_LOGIC("Cannot read from a close file");
	}
	if(offset >= size_) {
		THROW_RUNTIME("Offset > size");
	}
	fseek(file_, offset, SEEK_SET);
	return static_cast<char>(fgetc(file_));
}

void File::flush() {

	if(!isOpen_) return;

	fflush(file_);
}

size_t File::getMaxSize() const {
	return maxSize_;
}

void File::setMaxSize(size_t maxSize) {
	if(maxSize > size_) {
		maxSize_ = maxSize;
	} else {
		THROW_LOGIC("Max size cannot be less than size");
	}
}
