/*
 * NextException.cpp
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#include "NextException.h"
#include <string>
#include <cstdio>


namespace Renal {

	NextException::NextException(const char *message) {
		this->message = new std::string(message);

	}

	NextException::~NextException() throw() {
		delete(message);
	}

	std::string * NextException::GetMessage() {
		return message;
	}

	void NextException::PrintMessage() {
		fprintf(stderr, "Exception caught: [0x%p] %s\n", message, message->c_str());
	}

}
