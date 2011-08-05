/*
 * SensitiveServer.h
 *
 *  Created on: 05/ago/2011
 *      Author: Massi
 */

#ifndef SENSITIVESERVER_H_
#define SENSITIVESERVER_H_

#include <QString>
#include <QObject>
#include <QTcpServer>

namespace Protocol {

class SensitiveServer : public QTcpServer {

	Q_OBJECT

public:
	SensitiveServer(QObject *parent = 0);
	virtual ~SensitiveServer();

private:
	QString		message;
	QTcpSocket*	connection;
	quint16		blockSize;

private slots:
	void	HandleConnection();
	void	ReadMessage();

signals:
	/* Emitted while waiting a message */
	void	WaitingMessage();

	/* Emitted after having successfully received the coordinates */
	void	CoordinatesSuccess();

	/* Emitted if the client didn't behave like we were expecting */
	void	CoordinatesFail();
};

}

#endif /* SENSITIVESERVER_H_ */
