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

#ifndef SENSITIVESERVER_H_
#define SENSITIVESERVER_H_

#include <QString>
#include <QObject>
#include <QTcpServer>
#include <vector>

namespace Protocol {

class SensitiveServer : public QTcpServer {

	Q_OBJECT

public:
	SensitiveServer(QObject *parent = 0);
	virtual ~SensitiveServer();

	void		StartServer(quint16 port);
	std::vector<std::pair<double, double> >* GetCoordinates();

public slots:
	void		Shutdown();

private:
	QString		message;
	QTcpSocket*	connection;
	quint16		blockSize;
	bool		connected;
	bool		syn, ack;
	int			receive_max;
	int			received;
	std::vector<std::pair<double,double> >* coordinates;

private slots:
	void	HandleConnection();
	void	ReadMessage();

signals:
	/* Signals sent during the different phases of transaction */
	void	SynAcquired(QString& client);
	void 	BcooAcquired(int coords);
	void	CoorAcquired();
};

}

#endif /* SENSITIVESERVER_H_ */
