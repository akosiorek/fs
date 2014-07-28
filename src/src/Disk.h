/*
 * Disk.h
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#ifndef DISK_H_
#define DISK_H_

#include "typedefs.h"
#include "Bitmap.h"
#include "Header.h"
#include "BlockManager.h"
#include "INode.h"
#include "DataBlock.h"

#include <fstream>
#include <memory>

class Disk {
private:
	typedef std::unique_ptr<Bitmap> BitmapPtr;
	typedef std::unique_ptr<Header> HeaderPtr;
	typedef BlockManager<INode> INodeManager;
	typedef std::unique_ptr<INodeManager> INodeManagerPtr;
	typedef BlockManager<DataBlock> DataManager;
	typedef std::unique_ptr<DataManager> DataManagerPtr;
	typedef std::shared_ptr<INode> INodePtr;
	typedef std::shared_ptr<DataBlock> DataBlockPtr;
public:
	Disk();
	virtual ~Disk();

	void open(const std::string& filepath);
	void create(const std::string& filepath, size_t maxFiles, size_t diskSize, bool overwrite = false);
	void copyToDisk(const std::string& source);
	void copyFromDisk(const std::string& dest, const std::string& name) const;


private:
	bool fileExists(const std::string& filepath);
	void initManagers();
	void openIn();
	void closeIn();
	void openOut(bool overwrite = false);
	void closeOut();
	void dumpNullData(size_t size);
	std::string loadFile(const std::string& filepath);
	bool nameExists(const std::string& name);
	void writeData(const std::string& data, int firstDataBlock);


private:
	std::string filepath_;
	std::ofstream out_;
	std::ifstream in_;
	HeaderPtr header_;
	BitmapPtr iNodeMap_;
	BitmapPtr dataMap_;
	INodeManagerPtr iNodeManager_;
	DataManagerPtr dataManager_;
	bool initialized_;
};

#endif /* DISK_H_ */
