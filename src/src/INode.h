/*
 * INode.h
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#ifndef INODE_H_
#define INODE_H_

#include "typedefs.h"
#include "Serializable.h"

struct INode : public Serializable {

	static const size_t nameOffset;
	static const size_t nameSize;
	static const size_t blockSize;

	INode();
	INode(const INode&) = delete;

	void setName(const char* name);
	bool sameName(const char* name);
	virtual void serialize(std::ofstream& ostream) const ;
	virtual void deserialize(std::ifstream& istream);

	char name[20];
	size_t size;
	time_t modified;
	int dataBlock;
};



#endif /* INODE_H_ */
