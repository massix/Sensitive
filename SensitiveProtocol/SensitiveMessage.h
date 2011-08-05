/*
 * SensitiveMessage.h
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#ifndef SENSITIVEMESSAGE_H_
#define SENSITIVEMESSAGE_H_

#define HEADER_SIZE 4

#include <QString>

namespace Protocol {

enum Headers {
	MESSAGE_ACK = 0,
	MESSAGE_SYN,
	MESSAGE_BCOO,
	MESSAGE_ECOO
};

static QString	Messages[] = {"ACK!", "SYN!", "BCOO", "ECOO"};

class SensitiveMessage {

public:
	SensitiveMessage(Headers header, const char * payload);
	virtual ~SensitiveMessage();

	QString GetMessage() const;

protected:
	QString _header;
	QString _payload;
};

}

#endif /* SENSITIVEMESSAGE_H_ */
