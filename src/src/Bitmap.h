/*
 * Bitmap.h
 *
 *  Created on: May 25, 2014
 *      Author: Adam Kosiorek
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "typedefs.h"
#include "Serializable.h"

class Bitmap : public Serializable {
public:
	Bitmap();

	void take(int which);
	void leave(int which);
	bool read(int which);
	int firstUnused();
	virtual void serialize(std::ofstream& ostream) const;
	virtual void deserialize(std::ifstream& istream);

protected:
	byte map[BLOCK_SIZE];
	byte* end;
};

#endif /* BITMAP_H_ */
