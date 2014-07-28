/*
 * BlockManager.h
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#ifndef BLOCKMANAGER_H_
#define BLOCKMANAGER_H_

#include "typedefs.h"

#include "glog/logging.h"
#include <memory>
#include <fstream>

template<typename BlockType>
class BlockManager {
protected:
	typedef std::shared_ptr<BlockType> BlockPtr;

public:
	BlockManager(std::ifstream& in, std::ofstream& out, int offset)
	:	in_(in), out_(out), offset_(offset) {};

	virtual ~BlockManager() = default;


	BlockPtr readBlock(int which) {

		findBlockRead(which);
		BlockPtr block = std::make_shared<BlockType>();
		in_ >> *block;
		return block;
	}

	void writeBlock(BlockPtr block, int which) {

		findBlockWrite(which);
		out_ << *block;
	}

	template<typename T>
	void readSome(int which, size_t offset, T* ptr, size_t size) {

		findBlockRead(which, offset);
		in_.read((char*)ptr, size);
	}

	template<typename T>
	void writeSome(int which, size_t offset, T* ptr, size_t size) {

		findBlockWrite(which, offset);
		out_.write((char*)ptr, size);
	}

protected:
	void findBlockRead(int which, int offset = 0) {
		in_.seekg(offset_ + which * BlockType::blockSize + offset);
	}

	void findBlockWrite(int which, int offset = 0) {
		out_.seekp(offset_ + which * BlockType::blockSize + offset);
	}

protected:
	std::ifstream& in_;
	std::ofstream& out_;

private:
	const int offset_;


};

#endif /* BLOCKMANAGER_H_ */
