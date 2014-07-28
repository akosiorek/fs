/*
 * DataBlock.cpp
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#include "DataBlock.h"

#include <cstring>

const size_t DataBlock::capacity =  BLOCK_SIZE - sizeof(int);
const size_t DataBlock::blockSize = BLOCK_SIZE;

DataBlock::DataBlock() : next(-1) {};

void DataBlock::writeData(byte* data, size_t size) {

	memcpy(this->data, data, size);
}

void DataBlock::copyDataFrom(byte* data, size_t size) {

	memcpy(data, this->data, size);
}

void DataBlock::copyDataTo(const byte* data, size_t size) {

	memcpy(this->data, data, size);
}

void DataBlock::serialize(std::ofstream& ostream) const {

	ostream.write((char*)data, capacity);
	ostream.write((char*)&next, sizeof(int));
}

 void DataBlock::deserialize(std::ifstream& istream) {
	istream.read((char*)data, capacity);
	istream.read((char*)&next, sizeof(int));
}





