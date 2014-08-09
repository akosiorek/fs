/*
 * test.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Adam Kosiorek
 */


#include "gtest/gtest.h"
#include "File.h"
#include <glog/logging.h>
#include <cstdio>
#include <stdexcept>

struct FileTest : public testing::Test {

	FileTest() : path("/tmp/file") {}

	void TearDown() {
		remove(path.c_str());
	}

	std::string path;
};

TEST_F(FileTest, UninitialisedTest) {

	File file;
	ASSERT_FALSE(file.isOpen());
	ASSERT_EQ(file.size(), 0);
	ASSERT_EQ(file.path(), "");
	ASSERT_EQ(file.getMaxSize(), 0);

	// Throws error upon opening or creating a file with no path
	ASSERT_THROW(file.open(), std::logic_error);
	ASSERT_THROW(file.create(), std::logic_error);

	// Throws error upon io on a closed files
	ASSERT_THROW(file.write(std::string(), 0), std::logic_error);
	ASSERT_THROW(file.read(0, 0), std::logic_error);
	ASSERT_THROW(file.writeChar('c', 0), std::logic_error);
	ASSERT_THROW(file.readChar(0), std::logic_error);

	// Close shouldn't throw any errors
	ASSERT_NO_THROW(file.close());
	ASSERT_FALSE(file.isOpen());
}

TEST_F(FileTest, InitialisedTest) {

	File file(path.c_str());
	ASSERT_TRUE(file.isOpen());
	ASSERT_EQ(file.size(), 0);
	ASSERT_EQ(file.path(), path.c_str());

	// Throws error upon trying to open or to create an opened file
	ASSERT_THROW(file.open(), std::logic_error);
	ASSERT_THROW(file.create(), std::logic_error);

	// Does not throw errors upon writing
	ASSERT_NO_THROW(file.write(std::string(), 0));
	ASSERT_NO_THROW(file.writeChar('c', 0));

	// The file is empty so we cannot read even a single char
	ASSERT_THROW(file.read(0, 0), std::runtime_error);
	ASSERT_THROW(file.readChar(0), std::runtime_error);


	// Close shouldn't throw any errors
	ASSERT_NO_THROW(file.close());
	ASSERT_FALSE(file.isOpen());

	// Create should throw an error still, but runtime
	ASSERT_THROW(file.create(), std::runtime_error);
	ASSERT_FALSE(file.isOpen());

	// Open shouldn't throw any errors and should open the file
	ASSERT_NO_THROW(file.open());
	ASSERT_TRUE(file.isOpen());
}

TEST_F(FileTest, WriteReadTest) {

	File file(path);
	file.write("aaaa");
	ASSERT_EQ(file.size(), 4);
	file.flush();

	std::string buf = file.read(4);
	ASSERT_EQ(buf.size(), 4);
	for(int i = 0; i < 4; ++i) {
		ASSERT_EQ(buf.at(i), 'a');
	}

	// Read with offset >= 4 should throw a runtime error
	ASSERT_THROW(file.read(0, 10), std::runtime_error);

	// Read with size >= actual size should return all data after the offset
	ASSERT_EQ(file.read(10, 0).size(), 4);

	// Offsetting should reduce the read size
	ASSERT_EQ(file.read(10, 2).size(), 2);
}

TEST_F(FileTest, CharTest) {

	File file(path);
	file.writeChar('z', 2);
	file.flush();

	ASSERT_EQ(file.readChar(2), 'z');
	// Size should == 2 + 1 == 3
	ASSERT_EQ(file.size(), 3);
	// Reading a single char beyond the end should throw a runtime
	ASSERT_THROW(file.readChar(3), std::runtime_error);
}

TEST_F(FileTest, MaxSizeTest) {

	File file(path);
	file.setMaxSize(10);
	ASSERT_EQ(file.size(), 0);

	// Batch writing should throw when offset + size exceeds maxSize
	ASSERT_NO_THROW(file.write("aaaaaaaaaa", 0));
	ASSERT_THROW(file.write("aaaaaaaaaaa", 0), std::runtime_error);
	ASSERT_NO_THROW(file.write("aaaaa", 5));
	ASSERT_THROW(file.write("aaaaa", 6), std::runtime_error);

	// Single char writing should throw when offset exceeds maxSize
	ASSERT_NO_THROW(file.writeChar('a', 9));
	ASSERT_THROW(file.writeChar('a', 10), std::runtime_error);
}

//TEST_F(FileTest, Test) {
//
//
//}
//
//TEST_F(FileTest, Test) {
//
//
//}
//
//TEST_F(FileTest, Test) {
//
//
//}

