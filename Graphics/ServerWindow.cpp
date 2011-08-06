/*
 * ServerWindow.cpp
 *
 *  Created on: 06/ago/2011
 *      Author: Massi
 */

#include "ServerWindow.h"

#include <QtGui>
#include <QtCore>

namespace Graphics {

ServerWindow::ServerWindow(QWidget *parent) : QDialog(parent) {
	setWindowModality(Qt::ApplicationModal);

	vboxLayout = new QVBoxLayout(this);

	configuration = new QHBoxLayout();
	buttons = new QHBoxLayout();

	portChooser = new QSpinBox();
	portChooser->setRange(1024, 65534);
	portChooser->setValue(53157);
	portChooser->setSingleStep(1);

	startServer = new QPushButton("Start");
	QObject::connect(startServer, SIGNAL(clicked()), this, SLOT(StartServerClicked()));

	stopServer = new QPushButton("Stop");
	stopServer->setEnabled(false);
	QObject::connect(stopServer, SIGNAL(clicked()), this, SLOT(CancelServerClicked()));


	/* Arrange items */
	vboxLayout->addWidget(new QLabel("Please set a port and click on \"Start\"."));

	configuration->addWidget(new QLabel("Port: "));
	configuration->addWidget(portChooser);

	vboxLayout->addLayout(configuration);

	buttons->addWidget(startServer);
	buttons->addWidget(stopServer);

	vboxLayout->addLayout(buttons);

	setFixedSize(vboxLayout->sizeHint());
}


void ServerWindow::StartServerClicked() {
	quint16 port = (quint16) portChooser->value();
	startServer->setEnabled(false);
	stopServer->setEnabled(true);

	emit StartServer(port);
}


void ServerWindow::CancelServerClicked() {
	emit CancelServer();

	stopServer->setEnabled(false);
	startServer->setEnabled(true);
}

ServerWindow::~ServerWindow() {


}


void ServerWindow::Progress0() {
	progressDialog = new QProgressDialog("Receiving coordinates", "Cancel", 0, 4, this);
	QObject::connect(progressDialog, SIGNAL(canceled()), this, SLOT(CancelServerClicked()));
	QObject::connect(progressDialog, SIGNAL(rejected()), this, SLOT(CancelServerClicked()));
	progressDialog->show();
}


void ServerWindow::Progress25(QString& client) {
	progressDialog->setValue(1);
	progressDialog->setLabelText("Receiving from: " + client);
}


void ServerWindow::Progress50(int coords) {
	progressDialog->setValue(2);
	progressDialog->setLabelText(QString("A total of %0 coordinates").arg(coords));
}


void ServerWindow::Progress75() {
	progressDialog->setValue(3);
}


void ServerWindow::Progress100() {
	progressDialog->setValue(4);
	progressDialog->accept();

	startServer->setEnabled(true);
	stopServer->setEnabled(false);
}

}
