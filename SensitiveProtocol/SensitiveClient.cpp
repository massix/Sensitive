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

#include "SensitiveClient.h"
#include "SensitiveMessage.h"
#include "SensitiveException.h"

#include <QAbstractSocket>
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QHostInfo>

#include <iostream>

namespace Protocol {

SensitiveClient::SensitiveClient(QObject *parent) : QTcpSocket(), ack(false), coords(0) {
	QObject::connect(this, SIGNAL(readyRead()), this, SLOT(ReadMessage()));
	QObject::connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ErrorHandler(QAbstractSocket::SocketError)));
}

void SensitiveClient::ResetClient() {
	if (isValid())
		disconnectFromHost();

	ack = 0;
	if (coords != 0)
		delete(coords);
}

void SensitiveClient::InitiateProtocol(QString& server_ip, quint16 port) {
	if (coords == 0)
		throw SensitiveException("No coordinates set");

	if (server_ip.isNull() || server_ip.isEmpty())
		server_ip = "127.0.0.1";

	QHostAddress address(server_ip);

	connectToHost(address, port);

	waitForConnected();

	/* Send SYN */
	SensitiveMessage msg(MESSAGE_SYN, QHostInfo::localHostName().toStdString().c_str());

	block = new QByteArray();
	block->clear();

	QDataStream out(block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (quint16)0;
	out << msg.GetMessage();
	out.device()->seek(0);
	out << (quint16) (block->size() - sizeof(quint16));

	write(*block);

	blockSize = 0;
}

void SensitiveClient::ReadMessage() {
	QDataStream in(this);
	in.setVersion(QDataStream::Qt_4_0);

	if (blockSize == 0) {
		if (bytesAvailable() < (int) sizeof(quint16))
			return;

		in >> blockSize;
	}

	if (bytesAvailable() < blockSize)
		return;


	QString msg;
	in >> msg;

	/* The first message I'm supposed to receive is the ACK by the server */
	if (msg.startsWith(MESSAGE_ACK) && !ack) {
		msg.remove(0, 4);
		ack = true;

		emit AckAcquired(msg);

		/* Send a message indicating how much coordinates he should expect */
		block->clear();
		QDataStream out(block, QIODevice::WriteOnly);
		QString numCoords(QString("%0").arg(coords->size()));
		SensitiveMessage comsg(MESSAGE_BCOO, numCoords.toStdString().c_str());

		out << (quint16)0;
		out << comsg.GetMessage();
		out.device()->seek(0);
		out << (quint16)(block->size() - sizeof(quint16));

		write(*block);
	}

	else if (msg.startsWith(MESSAGE_BCOO) && ack) {
		msg.remove(0, 4);
		QString coordinates;

		emit BcooAcquired();

		/* Send the coordinates */
		for (std::vector<std::pair<double, double> >::iterator ite = coords->begin(); ite != coords->end(); ++ite)
			coordinates.append(QString("%0:%1;").arg(ite->first).arg(ite->second));

		/* remove the last ';' */
		coordinates.chop(1);
		SensitiveMessage coordMsg(MESSAGE_COOR, coordinates.toStdString().c_str());
		block->clear();
		QDataStream out(block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16)0;
		out << coordMsg.GetMessage();
		out.device()->seek(0);
		out << (quint16)(block->size() - sizeof(quint16));

		write(*block);

	}

	else if (msg.startsWith(MESSAGE_ECOO)) {
		ack = !ack;

		disconnectFromHost();
		close();

		emit EcooAcquired();
	}

	else
		throw SensitiveException("Protocol Error");

	blockSize = 0;
}

void SensitiveClient::SetCoordinates(std::vector<std::pair<double, double> >& coords) {
	this->coords = new std::vector<std::pair<double, double> >(coords);
}

void SensitiveClient::ErrorHandler(QAbstractSocket::SocketError error) {
	if (isValid()) disconnectFromHost();
	if (isOpen()) close();

	switch (error) {
	case QAbstractSocket::HostNotFoundError:
		throw SensitiveException("Host not found");
	case QAbstractSocket::ConnectionRefusedError:
		throw SensitiveException("Connection refused. Is server listening on the other side?");
	case QAbstractSocket::RemoteHostClosedError:
		throw SensitiveException("Remote host closed connection");
	case QAbstractSocket::SocketAccessError:
		throw SensitiveException("I can't access the socket. I probably don't have the right priviledges..");
	case QAbstractSocket::SocketResourceError:
		throw SensitiveException("Local system refused to open the socket.");
	case QAbstractSocket::SocketTimeoutError:
		throw SensitiveException("Socket timeout error :-(");
	default:
		throw SensitiveException("Unknown error");
	}
}

SensitiveClient::~SensitiveClient() {
	if (this->isValid()) disconnectFromHost();
	if (this->isOpen()) close();

	delete(block);
}

}
