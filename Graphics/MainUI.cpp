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

#include "MainUI.h"

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTreeView>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QStringListModel>

namespace Graphics {

MainUI::MainUI() : QWidget() {
	setWindowTitle("Sensitive UI");
	setWindowIcon(QIcon("Icon.svg"));

	button_hit = false;

	layout = new QGridLayout(this);
	button = new QPushButton("Click me");
	label = new QLabel("Click him!");
	coords_view = new QTreeView();

	layout->addWidget(label, 0, 0);
	layout->addWidget(button, 0, 1);
	layout->addWidget(coords_view, 1, 0);

	model = new QStandardItemModel();

	model->setHorizontalHeaderItem(0, new QStandardItem("x"));
	model->setHorizontalHeaderItem(1, new QStandardItem("y"));

	model->setItem(0, 0, new QStandardItem("coord 1"));
	model->setItem(0, 1, new QStandardItem("coord 2"));

	coords_view->setModel(model);

	QObject::connect(button, SIGNAL(clicked()), this, SLOT(ButtonClicked()));
	QObject::connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(CheckData(QStandardItem*)));
}

MainUI::~MainUI() {
	delete(label);
	delete(button);
	delete(model);
	delete(coords_view);
	delete(layout);
}

void MainUI::ButtonClicked() {
	if (!button_hit)
		label->setText("Clicked him, huh?");
	else
		label->setText("Click him!");

	int row = model->rowCount();

	model->setItem(row, 0, new QStandardItem("added"));
	model->setItem(row, 1, new QStandardItem("clicking"));

	label->setText(QString("%0").arg(model->rowCount()));

	button_hit = !button_hit;
}

void MainUI::CheckData(QStandardItem * data) {
	std::cout << data->text().toStdString() << std::endl;
}

}

