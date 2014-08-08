/*
 * test.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Adam Kosiorek
 */


#include "gtest/gtest.h"
#include "File.h"
#include <cstdio>

struct FileTest : public testing::Test {

	FileTest() : path("/tmp/file") {

		FILE* f = fopen(path.c_str(), "w");
		fclose(f);
	}

	void TearDown() {
		remove(path.c_str());
	}

	std::string path;
};

TEST_F(FileTest, CreateTest) {

	File file(path.c_str());
	ASSERT_TRUE(file.isOpen());
	ASSERT_EQ(file.size(), 0);
	ASSERT_EQ(file.path(), path.c_str());
}


TEST_F(FileTest, CloseTest) {

	File file(path.c_str());
	ASSERT_TRUE(file.isOpen());
	file.close();
	ASSERT_FALSE(file.isOpen());
}

TEST_F(FileTest, WriteTest) {

	File file(path);

	file.write("aaaa");
	ASSERT_EQ(file.size(), 4);
	file.close();
	ASSERT_EQ(file.size(), 0);

	file.open();
	ASSERT_EQ(file.size(), 4);

	std::string buf = file.read(4);
	for(int i = 0; i < 4; ++i) {
		ASSERT_EQ(buf.at(i), 'a');
	}
}


TEST_F(FileTest, CharTest) {

	File file(path);
	file.writeChar('z', 2);
	file.close();
	file.open();
	ASSERT_EQ(file.readChar(2), 'z');
}
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

