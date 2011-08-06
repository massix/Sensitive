/*
 * ServerWindow.h
 *
 *  Created on: 06/ago/2011
 *      Author: Massi
 */

#ifndef SERVERWINDOW_H_
#define SERVERWINDOW_H_

#include <QtGui>
#include <QtCore>

namespace Graphics {

class ServerWindow : public QDialog {

	Q_OBJECT

public:
	ServerWindow(QWidget *parent = 0);
	virtual ~ServerWindow();

public slots:
	void	Progress0();
	void	Progress25(QString& client);
	void	Progress50(int coords);
	void	Progress75();
	void	Progress100();


private:
	QVBoxLayout		*vboxLayout;
	QProgressDialog	*progressDialog;

	QHBoxLayout		*configuration;
	QSpinBox		*portChooser;

	QHBoxLayout		*buttons;
	QPushButton		*startServer;
	QPushButton		*stopServer;


private slots:
	void StartServerClicked();
	void CancelServerClicked();

signals:
	void StartServer(quint16 port);
	void CancelServer();
};

}

#endif /* SERVERWINDOW_H_ */
