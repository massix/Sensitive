/*
 * SensitiveMessage.cpp
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#include "SensitiveMessage.h"

namespace Protocol {

SensitiveMessage::SensitiveMessage(Headers header, const char * payload)
	: _header(Messages[header]), _payload(payload)
{


}

QString SensitiveMessage::GetMessage() const
{
	return _header + _payload;
}

SensitiveMessage::~SensitiveMessage()
{

}

}
