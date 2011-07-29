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
#include <vector>
#include <cstdio>

#include <QtGui>

#include <Renal/NextCalculator.h>

namespace Graphics {

MainUI::MainUI(Renal::NextCalculator *calculator) : QMainWindow() {
	setWindowTitle("Sensitive UI");
	setWindowIcon(QIcon("Icon.svg"));

	this->calculator = calculator;

	/* Defining layout */
	central_widget = new QWidget();

	central_layout = new QHBoxLayout(central_widget);
	left_part = new QVBoxLayout();
	right_part = new QVBoxLayout();
	bottom_buttons = new QHBoxLayout();

	central_layout->addLayout(left_part);
	central_layout->addLayout(right_part);


	/* Table Widget that will contain the coordinates */
	coords_table = new QTableWidget(1, 2);

	/* Headers for the table */
	QStringList headers;
	headers.push_back("x");
	headers.push_back("y");

	coords_table->setHorizontalHeaderLabels(headers);

	left_part->addWidget(coords_table);

	/* Buttons for the coordinates management */
	reset_coords = new QPushButton("Reset");
	add_coord = new QPushButton("Add");
	delete_coord = new QPushButton("Remove");
	interpole = new QPushButton("Interpole!");

	bottom_buttons->addWidget(add_coord);
	bottom_buttons->addWidget(delete_coord);
	bottom_buttons->addWidget(reset_coords);
	bottom_buttons->addWidget(interpole);

	left_part->addLayout(bottom_buttons);

	/* Connecting signals */
	QObject::connect(add_coord, SIGNAL(clicked()), this, SLOT(AddCoord()));
	QObject::connect(delete_coord, SIGNAL(clicked()), this, SLOT(DeleteCoord()));
	QObject::connect(reset_coords, SIGNAL(clicked()), coords_table, SLOT(clearContents()));
	QObject::connect(interpole, SIGNAL(clicked()), this, SLOT(Interpole()));

	CreateMenus();
	setCentralWidget(central_widget);

}

MainUI::~MainUI() {
	delete(calculator);
}

void MainUI::CheckData(QTableWidgetItem * data) {

}

void MainUI::AddCoord() {
	coords_table->setRowCount(coords_table->rowCount() + 1);
}

void MainUI::DeleteCoord() {
	coords_table->removeRow(coords_table->currentRow());
}

void MainUI::Interpole() {
	if (calculator == 0)
		return;

	calculator->Clear();

	for (int row = 0; row < coords_table->rowCount(); row++) {
		if (coords_table->item(row, 0) != 0) {
			QDoubleValidator dValidator;
			QValidator::State validation;
			QString input(coords_table->item(row, 0)->text());
			int pos;

			bool result_x, result_y;

			validation = dValidator.validate(input, pos);

			switch (validation) {
			case QValidator::Intermediate:
			case QValidator::Acceptable:
				double x, y;
				x = coords_table->item(row, 0)->text().toDouble(&result_x);
				y = coords_table->item(row, 1)->text().toDouble(&result_y);
				if (result_x && result_y)
					calculator->InsertCoords(x, y);
				break;
			default:
				std::cout << "Item is not valid." << std::endl;
			}
		}
	}

	std::cout << "Interpolation requested" << std::endl;
	if (calculator->BuildFunction()) {
		std::vector<double> polynom = calculator->GetPolynom();

		printf("\t\tf(x) = ");
		for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
			printf("%.2f ", *ite);
		printf("\n");
	}

	fflush(stdout);
}

void MainUI::CreateMenus() {
	QMenuBar *menubar = menuBar();
	QMenu *fileMenu = menubar->addMenu("File");

	QAction *exit = fileMenu->addAction("Exit");

	QObject::connect(exit, SIGNAL(triggered()), this, SLOT(close()));
}

}

