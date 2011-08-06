/* Copyright (C) 2011 by Massimo Gengarelli <massimo.gengarelli@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SensitiveServer.h"
#include "SensitiveMessage.h"
#include "SensitiveException.h"

#include <vector>

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QStringList>
#include <QHostInfo>

namespace Protocol {

SensitiveServer::SensitiveServer(QObject *parent)
	: QTcpServer(parent), connected(false), syn(false), ack(false), receive_max(-1), received(0)
{

}

void SensitiveServer::Shutdown() {
	connected = false;
	syn = false;
	ack = false;
	receive_max = -1;
	received = 0;
	blockSize = 0;

	if (isListening())
		close();
}

void SensitiveServer::StartServer(quint16 port) {
	QObject::connect(this, SIGNAL(newConnection()), this, SLOT(HandleConnection()));
	listen(QHostAddress::Any, port);
}


void SensitiveServer::HandleConnection() {
	/* We're already handling someone, they have to sit down in the waiting room. */
	if (connected)
		return;

	/* Start serving another client */
	blockSize = 0;
	connection = nextPendingConnection();
	QObject::connect(connection, SIGNAL(disconnected()), connection, SLOT(deleteLater()));
	QObject::connect(connection, SIGNAL(readyRead()), this, SLOT(ReadMessage()));

	connected = true;
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

	/* First kind of message we may receive is the SYN! message */
	if (message.startsWith(MESSAGE_SYN) && !syn) {
		message.remove(0, 4);
		syn = true;

		emit SynAcquired(message);

		/* Send (b)ACK */
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		SensitiveMessage msg(MESSAGE_ACK,QHostInfo::localHostName().toStdString().c_str());

		out << (quint16)0;
		out << msg.GetMessage();
		out.device()->seek(0);
		out << (quint16)(block.size() - sizeof(quint16));

		connection->write(block);
	}

	/* BCOO Message, "Begin Coordinates" sending */
	else if (message.startsWith(MESSAGE_BCOO) && syn) {
		message.remove(0, 4);
		bool ok;

		receive_max = message.toInt(&ok);

		if (!ok)
			throw SensitiveException("Malformed BCOO message received.");

		emit BcooAcquired(receive_max);

		received = 0;

		/* Send the message back */
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		out << (quint16) (msg.size() - sizeof(quint16));
		out << msg;

		connection->write(block);
	}

	/* List of the "COOR"dinates */
	else if (message.startsWith(MESSAGE_COOR) && (receive_max > 0)) {
		message.remove(0, 4);

		QStringList receivedCoords = message.split(';');

		if (receivedCoords.size() != receive_max)
			throw SensitiveException("Too much coordinates received.");

		coordinates = new std::vector<std::pair<double, double> >();
		foreach(QString string, receivedCoords) {
			QStringList partial = string.split(':');
			if (partial.size() != 2)
				continue;

			bool ok_x, ok_y;
			double x = partial.at(0).toDouble(&ok_x);
			double y = partial.at(1).toDouble(&ok_y);

			if (ok_x && ok_y) {
				coordinates->push_back(std::pair<double, double>(x, y));
				++received;
			}
		}

		emit CoorAcquired();

		/* Send Ecoo to signal to the client that we successfully received the coordinates */
		SensitiveMessage ecoo(MESSAGE_ECOO, "Goodbye");
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out << (quint16)0;
		out << ecoo.GetMessage();
		out.device()->seek(0);
		out << (quint16)(block.size() - sizeof(quint16));

		connection->write(block);

//		connection->disconnectFromHost();
//		connection->close();

		syn = false;
		ack = false;
		receive_max = -1;
		received = 0;
	}

	/* Something bad happened :( */
	else
		throw SensitiveException("Protocol Error");

	/* Reset the blockSize in order to receive the next message */
	blockSize = 0;
}

std::vector<std::pair<double, double> >* SensitiveServer::GetCoordinates() {
	return coordinates;
}

SensitiveServer::~SensitiveServer() {
	if (connection->isValid())
		connection->disconnectFromHost();

	if (connection->isOpen())
		connection->close();

	delete(coordinates);
	delete(connection);
}

}
