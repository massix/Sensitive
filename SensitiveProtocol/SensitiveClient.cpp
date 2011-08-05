/*
 * SensitiveClient.cpp
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#include "SensitiveClient.h"
#include "SensitiveMessage.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>

namespace Protocol {

SensitiveClient::SensitiveClient(QObject *parent) : QTcpSocket() {
	connectToHost(QHostAddress::LocalHost, 53517);

	SensitiveMessage msg(MESSAGE_SYN, "Test SYN message");

	block = new QByteArray();
	block->clear();

	QDataStream out(block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (quint16)0;
	out << msg.GetMessage();
	out.device()->seek(0);
	out << (quint16) (block->size() - sizeof(quint16));

	write(*block);

	disconnectFromHost();
	close();
}

SensitiveClient::~SensitiveClient() {
	delete(block);
}

}
