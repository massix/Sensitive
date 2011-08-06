/*
 * ClientWindow.cpp
 *
 *  Created on: 06/ago/2011
 *      Author: Massi
 */

#include "ClientWindow.h"

#include <QtGui>
#include <QtCore>

namespace Graphics {

ClientWindow::ClientWindow(QWidget *parent) : QDialog(parent) {
	setWindowModality(Qt::ApplicationModal);

	vboxLayout = new QVBoxLayout(this);

	configuration = new QHBoxLayout();

	host = new QLineEdit();
	host->setPlaceholderText("hostname or ip address");

	portChooser = new QSpinBox();
	portChooser->setRange(1024, 65534);
	portChooser->setValue(53157);
	portChooser->setSingleStep(1);

	send = new QPushButton("Send");
	QObject::connect(send, SIGNAL(clicked()), this, SLOT(SendClicked()));

	/* Arrange items */
	vboxLayout->addWidget(new QLabel("Please set a hostname, a port and click on \"Send\"."));

	configuration->addWidget(host);
	configuration->addWidget(portChooser);

	vboxLayout->addLayout(configuration);

	vboxLayout->addWidget(send);

	setFixedSize(vboxLayout->sizeHint());

}

void ClientWindow::SendClicked() {
	QString hostname(QString(host->text()));
	emit ClientConfigured(hostname, (quint16) portChooser->value());
	accept();
}

ClientWindow::~ClientWindow() {

}

}
