/*
 * ClientWindow.h
 *
 *  Created on: 06/ago/2011
 *      Author: Massi
 */

#ifndef CLIENTWINDOW_H_
#define CLIENTWINDOW_H_

#include <QtGui>
#include <QtCore>

namespace Graphics {

class ClientWindow : public QDialog {

	Q_OBJECT

public:
	ClientWindow(QWidget *parent = 0);
	virtual ~ClientWindow();

public slots:
	void SendClicked();

signals:
	void	ClientConfigured(QString& host, quint16 port);

private:
	QVBoxLayout		*vboxLayout;

	QHBoxLayout		*configuration;
	QLineEdit		*host;
	QSpinBox		*portChooser;

	QPushButton		*send;

};

}

#endif /* CLIENTWINDOW_H_ */
