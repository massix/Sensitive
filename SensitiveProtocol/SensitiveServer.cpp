/*
 * SensitiveServer.cpp
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#include "SensitiveServer.h"
#include "SensitiveMessage.h"

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

#include <iostream>

namespace Protocol {

SensitiveServer::SensitiveServer(QObject *parent) : QTcpServer(parent) {
	QObject::connect(this, SIGNAL(newConnection()), this, SLOT(HandleConnection()));
	listen(QHostAddress::Any, 53517);
}


void SensitiveServer::HandleConnection() {
	QByteArray block, block_in;
	QDataStream out(&block, QIODevice::WriteOnly);

	blockSize = 0;
	connection = nextPendingConnection();
	QObject::connect(connection, SIGNAL(disconnected()), connection, SLOT(deleteLater()));
	QObject::connect(connection, SIGNAL(readyRead()), this, SLOT(ReadMessage()));

	std::cout << "[SensitiveServer] New connection" << std::endl;
}

void SensitiveServer::ReadMessage() {
	QDataStream in(connection);
	in.setVersion(QDataStream::Qt_4_0);

	if (blockSize == 0) {
		if (connection->bytesAvailable() < (int) sizeof(quint16))
			return;

		in >> blockSize;
	}

	if (connection->bytesAvailable() < blockSize)
		return;


	QString msg;
	in >> msg;

	message = msg;

	std::cout << "[SensitiveServer] " << blockSize << " Received: " << message.toStdString() << std::endl;

//	connection->disconnectFromHost();
//	connection->close();
}

SensitiveServer::~SensitiveServer() {
	close();
}

}
