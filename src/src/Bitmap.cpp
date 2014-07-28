/*
* Bitmap.cpp
*
*  Created on: 26 May 2014
*      Author: adam
*/

#include "Bitmap.h"
#include "glog/logging.h"

Bitmap::Bitmap() : end(map + BLOCK_SIZE) {

	memset(map, 0, BLOCK_SIZE);
}

void Bitmap::take(int which) {
	if(read(which)) {
		LOG(FATAL) << "Bit #" << which << " is taken. Cannot take it again";
	}

	byte* addr = map + which / 8;
//	LOG(ERROR) << "w: " << which << " w/8: " << which / 8 << " after: " << (1 << (which % 8))
//				<< " was: " << (int)*addr << " will be:" << (*addr | (1 << (which % 8)));

	*addr = (*addr | (1 << (which % 8)));
}

void Bitmap::leave(int which) {
	if(!read(which)) {
		LOG(FATAL) << "Bit #" << which << " is free. Cannot leave it";
	}

	byte* addr = map + which / 8;
	*addr = (*addr & ~(1 << which % 8));
}

bool Bitmap::read(int which) {

	byte* addr = map + which / 8;
	return 1 & ((*addr) >> (which% 8));
}

int Bitmap::firstUnused() {

	for(int i = 0; i < BLOCK_SIZE * 8; ++i) {

		if(!read(i)) {
			return i;
		}
	}
	return -1;
}

void Bitmap::serialize(std::ofstream& ostream) const {

	ostream.write((char*)map, BLOCK_SIZE);
}

void Bitmap::deserialize(std::ifstream& istream) {

	istream.read((char*)map, BLOCK_SIZE);
}

