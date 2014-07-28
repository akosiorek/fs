/*
 * Header.cpp
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#include "Header.h"

void Header::serialize(std::ofstream& ostream) const {

	ostream.write((char*)&blockSize, sizeof(blockSize));
	ostream.write((char*)&iNodeMapBlocks, sizeof(iNodeMapBlocks));
	ostream.write((char*)&dataMapBlocks, sizeof(dataMapBlocks));
	ostream.write((char*)&maxFiles, sizeof(maxFiles));
	ostream.write((char*)&diskSize, sizeof(diskSize));
}

void Header::deserialize(std::ifstream& istream) {

	istream.read((char*)&blockSize, sizeof(blockSize));
	istream.read((char*)&iNodeMapBlocks, sizeof(iNodeMapBlocks));
	istream.read((char*)&dataMapBlocks, sizeof(dataMapBlocks));
	istream.read((char*)&maxFiles, sizeof(maxFiles));
	istream.read((char*)&diskSize, sizeof(diskSize));
}



