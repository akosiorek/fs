/*
 * Serializable.h
 *
 *  Created on: 26 May 2014
 *      Author: adam
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <fstream>

struct Serializable {

	virtual ~Serializable() = default;

	virtual void serialize(std::ofstream& ostream) const = 0;
	virtual void deserialize(std::ifstream& istream) = 0;

	friend std::ostream& operator<< (std::ofstream& ostream, const Serializable& serial) {

		serial.serialize(ostream);
		return ostream;
	}

	friend std::ifstream& operator>> (std::ifstream& ifstream, Serializable& serial) {

		serial.deserialize(ifstream);
		return ifstream;
	}

};

#endif /* SERIALIZABLE_H_ */
