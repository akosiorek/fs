/*
 * DataBlock.h
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#ifndef DATABLOCK_H_
#define DATABLOCK_H_

#include "typedefs.h"
#include "Serializable.h"

struct DataBlock : public Serializable {

	DataBlock();
	DataBlock(const DataBlock&) = delete;

	void writeData(byte* data, size_t size);
	void copyDataFrom(byte* data, size_t size);
	void copyDataTo(const byte* data, size_t size);
	virtual void serialize(std::ofstream& ostream) const;
	virtual void deserialize(std::ifstream& istream);

	int next;
	byte data[BLOCK_SIZE - sizeof(int)];

	static const size_t capacity;
	static const size_t blockSize;
};

#endif /* DATABLOCK_H_ */
