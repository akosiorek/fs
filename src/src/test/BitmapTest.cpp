/*
 * BitmapTest.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Adam Kosiorek
 */

#include "gtest/gtest.h"
#include "glog/logging.h"

#include "Bitmap.h"

struct BitmapMock : public Bitmap {

	byte* getData() {
		return map;
	}

	void setData(byte* ptr, size_t size) {
		memcpy(map, ptr, size);
	}
};

struct BitmapTest : public testing::Test {

	BitmapMock bitmap;
};


TEST_F(BitmapTest, ReadTest) {

	EXPECT_FALSE(bitmap.read(0));
	byte b = 1;
	bitmap.setData(&b, sizeof(byte));
	EXPECT_TRUE(bitmap.read(0));
	EXPECT_FALSE(bitmap.read(7));

	b = 255;
	bitmap.setData(&b, sizeof(byte));
	for(int i = 0; i < 8; ++i)
		EXPECT_TRUE(bitmap.read(i));
	EXPECT_FALSE(bitmap.read(8));
}

TEST_F(BitmapTest, FirstUnusedTest) {

	EXPECT_EQ(bitmap.firstUnused(), 0);
	byte b = 1;
	bitmap.setData(&b, sizeof(byte));
	EXPECT_EQ(bitmap.firstUnused(), 1);
	b = 255;
	bitmap.setData(&b, sizeof(byte));
	EXPECT_EQ(bitmap.firstUnused(), 8);
}

TEST_F(BitmapTest, TakeTest) {

	int un = bitmap.firstUnused();
	EXPECT_EQ(un, 0);
	bitmap.take(un);
	EXPECT_TRUE(bitmap.read(un));

	for(int i = 1; i < 5000; ++i) {
//		LOG(ERROR) << i;
		int un = bitmap.firstUnused();
		ASSERT_EQ(un, i);
		bitmap.take(un);
		ASSERT_TRUE(bitmap.read(un));
	}
}

TEST_F(BitmapTest, LeaveTest) {

	memset(bitmap.getData(), 255, BLOCK_SIZE);
	for(int i = 0; i < 4000; ++i) {
//		LOG(ERROR) << i;
		ASSERT_TRUE(bitmap.read(i));
		bitmap.leave(i);
		ASSERT_FALSE(bitmap.read(i));
	}
}


