/*
 * Disk.cpp
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#include "Disk.h"

#include <glog/logging.h>
#include <sstream>
#include <ctime>

///		Constructors		///////////////////////////////////////////////////////////////////////////////////////////

Disk::Disk() :  header_(new Header()),  iNodeMap_(new Bitmap()), dataMap_(new Bitmap()), initialized_(false) {}

Disk::~Disk() {

	if(!initialized_) return;

	LOG(ERROR) << "Serializing metadata";
	openOut();
	out_.seekp(0);
	out_ << *header_;
	out_ << *iNodeMap_;
	out_ << *dataMap_;
	closeOut();
	LOG(ERROR) << "Finished";
};

///		Public		///////////////////////////////////////////////////////////////////////////////////////////

void Disk::open(const std::string& filepath) {

	if(!fileExists(filepath)) {
		LOG(FATAL) << "File " + filepath + " does not exist.";
	}
	filepath_ = filepath;

	LOG(ERROR) << "Reading metadata";
	openIn();
	in_ >> *header_;
	in_ >> *iNodeMap_;
	in_ >> *dataMap_;
	closeIn();

	initManagers();
	initialized_ = true;
	LOG(ERROR) << "Initialization completed";
}

void Disk::create(const std::string& filepath, size_t maxFiles, size_t diskSize, bool overwrite) {

	if(fileExists(filepath) && !overwrite) {
		LOG(FATAL) << "Cannot create disk at " << filepath << ". File exists.";
	}
	filepath_ = filepath;

	header_->blockSize = BLOCK_SIZE;
	header_->iNodeMapBlocks = 1;
	header_->dataMapBlocks = 1;
	header_->maxFiles = maxFiles;
	header_->diskSize = diskSize;

	initManagers();

	openOut(overwrite);
	dumpNullData(diskSize);
	closeOut();

	initialized_ = true;
}

void Disk::copyToDisk(const std::string& source) {

	if(nameExists(source)) {
		LOG(FATAL) << "A file with name: " << source << " already exists";
	}
	LOG(ERROR) << "Copying file " << source << " to disk";
	std::string fileContents = loadFile(source);

	LOG(ERROR) << "File loaded";

	int iNodeIdx = iNodeMap_->firstUnused();
	iNodeMap_->take(iNodeIdx);
	int dataIdx = dataMap_->firstUnused();

	INodePtr iNode = std::make_shared<INode>();
	iNode->dataBlock = dataIdx;
	iNode->modified = time(nullptr);
	iNode->setName(source.c_str());
	iNode->size = fileContents.size();

	openOut();
	iNodeManager_->writeBlock(iNode, iNodeIdx);
	LOG(ERROR) << "INode saved at position " << iNodeIdx;

	writeData(fileContents, dataIdx);
	LOG(ERROR) << "Data saved";
	closeOut();

}

void Disk::copyFromDisk(const std::string& dest, const std::string& name) const {


}

///		Private		///////////////////////////////////////////////////////////////////////////////////////////

bool Disk::fileExists(const std::string& filepath) {

	std::ifstream input(filepath);
	if(!input) return false;
	input.close();
	return true;
}

void Disk::initManagers() {

	size_t offset = sizeof(Header) + (header_->iNodeMapBlocks + header_->dataMapBlocks) * BLOCK_SIZE;
	iNodeManager_.reset(new INodeManager(in_, out_,  offset));
	offset += header_->maxFiles * INode::blockSize;
	dataManager_.reset(new DataManager(in_, out_, offset));
}

void Disk::openIn() {
	in_.open(filepath_);
}

void Disk::closeIn() {
	in_.close();
}

void Disk::openOut(bool overwrite) {
//	if(overwrite) {
//		out_.open(filepath_, std::ios::trunc);
//	} else {
////		out_.open(filepath_, std::ios::binary | std::ios::app);
		out_.open(filepath_, std::ios::out | std::ios::ate);
//	}
}

void Disk::closeOut() {
	out_.close();
}

void Disk::dumpNullData(size_t size) {

	LOG(ERROR) << "Dumping null bytes";
	byte nullData[BLOCK_SIZE];
	memset(nullData, 0, BLOCK_SIZE);
	size_t written = 0;
	while(written < (size / BLOCK_SIZE) * BLOCK_SIZE) {
		out_.write((char*)this, BLOCK_SIZE);
		written += BLOCK_SIZE;
	}
	out_.write((char*)&nullData, size - written);
	LOG(ERROR) << "Dumped: " << written << " null bytes";
}

std::string Disk::loadFile(const std::string& filepath) {

	std::ifstream file(filepath);
	std::stringstream sstream;
	sstream << file.rdbuf();
	file.close();
	return sstream.str();
}

bool Disk::nameExists(const std::string& name) {

	LOG(ERROR) << "Checking if file '" << name << "' exists";
	openIn();
	char nameBuf[20];
	for(int i = 0; i < header_->maxFiles; ++i) {
		iNodeManager_->readSome(i, INode::nameOffset, nameBuf, INode::nameSize);
		LOG(ERROR) << "name: " << nameBuf;
		if(strcmp(name.c_str(), nameBuf) == 0) {
			closeIn();
			LOG(ERROR) << "File '" << name << "' does exist";
			return true;
		}
	}
	closeIn();
	LOG(ERROR) << "File '" << name << "' does not exist";
	return false;
}

void Disk::writeData(const std::string& data, int thisIdx) {

	LOG(ERROR) << "Saving data";

	int size = data.size();
	const char* contents = data.c_str();
	DataBlockPtr block = std::make_shared<DataBlock>();
	while(size > 0) {

		dataMap_->take(thisIdx);
		block->copyDataTo((byte*)contents, DataBlock::capacity);
		size -= DataBlock::capacity;
		if(size > 0) {
			block->next = dataMap_->firstUnused();
			contents += DataBlock::capacity;
		}
		dataManager_->writeBlock(block, thisIdx);

		thisIdx = block->next;
	}
}

