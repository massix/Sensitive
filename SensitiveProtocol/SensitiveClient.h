/*
 * SensitiveClient.h
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#ifndef SENSITIVECLIENT_H_
#define SENSITIVECLIENT_H_

#include <QTcpSocket>
#include <QObject>

namespace Protocol {

class SensitiveClient : public QTcpSocket {
public:
	SensitiveClient(QObject *parent = 0);
	virtual ~SensitiveClient();

private:
	QByteArray *block;
};

}

#endif /* SENSITIVECLIENT_H_ */
