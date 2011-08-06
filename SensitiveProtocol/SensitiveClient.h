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

#ifndef SENSITIVECLIENT_H_
#define SENSITIVECLIENT_H_

#include <QTcpSocket>
#include <QObject>
#include <vector>

namespace Protocol {

class SensitiveClient : public QTcpSocket {

	Q_OBJECT

public:
	SensitiveClient(QObject *parent = 0);
	virtual ~SensitiveClient();

	void InitiateProtocol(QString& server_ip, quint16 port);
	void SetCoordinates(std::vector<std::pair<double, double> >& coords);


public slots:
	void ResetClient();

private:
	bool ack;
	QByteArray *block;
	quint16	blockSize;
	std::vector<std::pair<double, double> > *coords;

private slots:
	void	ReadMessage();
	void	ErrorHandler(QAbstractSocket::SocketError error);

signals:
	/* Signals emitted during the transaction */
	void	AckAcquired(QString& server);
	void	BcooAcquired();
	void	EcooAcquired();
};

}

#endif /* SENSITIVECLIENT_H_ */
