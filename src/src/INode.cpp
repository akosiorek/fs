/*
 * INode.cpp
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#include "INode.h"

#include <cstring>

const size_t INode::nameOffset = 0;
const size_t INode::nameSize = 20;
const size_t INode::blockSize = sizeof(name) + sizeof(size) + sizeof(modified) + sizeof(dataBlock);

INode::INode() : size(0), modified(0), dataBlock(-1) {

	memset(name, 0, 20);
};

void INode::setName(const char* name) {
	strcpy(this->name, name);
}

bool INode::sameName(const char* name) {
	return strcmp(this->name, name) == 0;
}

void INode::serialize(std::ofstream& ostream) const {
	ostream.write(name, nameSize);
	ostream.write((char*)&size, sizeof(size));
	ostream.write((char*)&modified, sizeof(modified));
	ostream.write((char*)&dataBlock, sizeof(dataBlock));
}

void INode::deserialize(std::ifstream& istream) {

	istream.read(name, nameSize);
	istream.read((char*)&size, sizeof(size));
	istream.read((char*)&modified, sizeof(modified));
	istream.read((char*)&dataBlock, sizeof(dataBlock));
}
