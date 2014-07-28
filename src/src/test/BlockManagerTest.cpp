/*
 * BitmapTest.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Adam Kosiorek
 */

#include "gtest/gtest.h"
#include "glog/logging.h"

#include "BlockManager.h"

#include <fstream>

struct Block {

	static const size_t blockSize;
};


const size_t Block::blockSize = 100;

struct BlockManagerTest : public testing::Test {

	BlockManagerTest() : offset(0), blockManager(input, output, offset), name("test.txt") {

		buff.resize(100);
	};

	std::ifstream input;
	std::ofstream output;
	int offset;
	BlockManager<Block> blockManager;
	std::string name;
	std::string buff;

	void openOut() {
		output.open(name.c_str());
	}
	void closeOut() {
		output.close();
	}
	void openIn() {
		input.open(name.c_str());
	}
	void closeIn() {
		input.close();
	}

	void log() {
		LOG(ERROR) << "Buffor: " << buff;
	}

};

TEST_F(BlockManagerTest, SomeTEst) {

	std::vector<std::string> names = {"adam", "paulina", "tata", "mama", "damazy"};
	for(auto& v : names) v.resize(20);

	openOut();
	for(int i = 0; i < names.size(); ++i) {
		blockManager.writeSome(i, 30, names[i].data(), 20);
	}
	closeOut();


	openIn();
	for(int i = 0; i < names.size(); ++i) {
		blockManager.readSome(i, 30, buff.data(), 20);
		log();
	}

	closeIn();

}
