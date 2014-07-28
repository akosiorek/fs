/*
 * Header.h
 *
 *  Created on: May 25, 2014
 *      Author: Adam Kosiorek
 */

#ifndef HEADER_H_
#define HEADER_H_

#include "typedefs.h"
#include "Serializable.h"

struct Header : public Serializable {

	Header() = default;
	Header(const Header&) = delete;

	size_t blockSize;
	size_t iNodeMapBlocks;
	size_t dataMapBlocks;
	size_t maxFiles;
	size_t diskSize;

	virtual void serialize(std::ofstream& ostream) const;
	virtual void deserialize(std::ifstream& istream);
};


#endif /* HEADER_H_ */
