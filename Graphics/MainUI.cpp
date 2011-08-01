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
#include "AboutDialog.h"

#include <vector>
#include <cmath>

#include <QtCore>
#include <QtGui>

#include <QPropertyAnimation>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>

#define MAX_ERROR 0.00000001

namespace Graphics {

MainUI::MainUI(Renal::NextCalculator *calculator) : QMainWindow() {
	setWindowTitle("Sensitive UI");
	setWindowIcon(QIcon(":/bundle/icon.svg"));

	this->calculator = calculator;

	/* Defining layout */
	fixed_widget = new QWidget();
	docked_widget = new QWidget();

	coords_dock = new QDockWidget("Coordinates");

	docked_layout = new QVBoxLayout(docked_widget);
	central_layout = new QVBoxLayout(fixed_widget);
	bottom_buttons = new QHBoxLayout();

	addDockWidget(Qt::LeftDockWidgetArea, coords_dock);
	coords_dock->setWidget(docked_widget);

	/* ++++ BUILDING UP THE DOCKED WIDGET WHICH WILL CONTAIN THE COORDINATES TABLE ++++ */

	/* Table Widget that will contain the coordinates */
	coords_table = new QTableWidget(1, 2);

	/* Headers for the table */
	QStringList headers;
	headers.push_back("x");
	headers.push_back("y");

	coords_table->setHorizontalHeaderLabels(headers);
	coords_table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	docked_layout->addWidget(coords_table);

	/* Buttons for the coordinates management */
	reset_coords = new QPushButton("Reset");
	add_coord = new QPushButton("Add");
	delete_coord = new QPushButton("Remove");
	interpole = new QPushButton("Interpole!");

	bottom_buttons->addWidget(add_coord);
	bottom_buttons->addWidget(delete_coord);
	bottom_buttons->addWidget(reset_coords);
	bottom_buttons->addWidget(interpole);

	docked_layout->addLayout(bottom_buttons);

	/* Connecting signals */
	QObject::connect(add_coord, SIGNAL(clicked()), this, SLOT(AddCoord()));
	QObject::connect(delete_coord, SIGNAL(clicked()), this, SLOT(DeleteCoord()));
	QObject::connect(reset_coords, SIGNAL(clicked()), coords_table, SLOT(clearContents()));
	QObject::connect(interpole, SIGNAL(clicked()), this, SLOT(Interpole()));



	/* ++++ BUILDING UP THE FIXED PART OF THE WINDOW, WHICH WILL CONTAIN THE PLOT AND THE RESULTS ++++ */
	results_grid = new QGridLayout();
	polynom_line = new QTextEdit();
	input_point = new QLineEdit();
	output_point = new QLineEdit();

	input_point->setMaxLength(20);
	input_point->setMaximumWidth(100);

	polynom_line->setReadOnly(true);
	polynom_line->setMaximumHeight(25);

	input_point->setPlaceholderText("point");

	output_point->setPlaceholderText("f(point) = ");
	output_point->setReadOnly(true);

	results_grid->addWidget(new QLabel("Interpolating Polynom: "), 0, 0);
	results_grid->addWidget(polynom_line, 0, 1);

	results_grid->addWidget(input_point, 1, 0);
	results_grid->addWidget(output_point, 1, 1);

	plot = new QwtPlot();
	plot->setTitle("Welcome to Sensitive");

	function = new QwtPlotCurve("f(x)");
	function->setTitle("f(x)");
	function->setStyle(QwtPlotCurve::Lines);

	plot->setAxisTitle(QwtPlot::xBottom, "x");
	plot->setAxisTitle(QwtPlot::yLeft, "y");

	central_layout->addWidget(plot);
	central_layout->addLayout(results_grid);

	/* Connecting signals */
	QObject::connect(input_point, SIGNAL(returnPressed()), this, SLOT(CalculateInPoint()));

	CreateMenus();

	statusBar()->show();
	statusBar()->showMessage("Welcome to Sensitive UI. Please feel free to smoke a joint while interpolating.");

	setCentralWidget(fixed_widget);

	showMaximized();
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
		if (coords_table->item(row, 0) != 0 && coords_table->item(row, 1) != 0) {
			double x, y;
			bool result_x, result_y;

			x = coords_table->item(row, 0)->text().toDouble(&result_x);
			y = coords_table->item(row, 1)->text().toDouble(&result_y);

			if (result_x && result_y)
				calculator->InsertCoords(x, y);
		}
	}

	try {
		calculator->BuildFunction();
		std::vector<double> *polynom = calculator->GetPolynom();
		QString output("<i>f(x)</i> = ");

		int pol_grade = polynom->size()-1;
		for (std::vector<double>::iterator ite = polynom->begin(); ite != polynom->end(); ++ite, --pol_grade) {
			if (*ite == 0)
				continue;

			double rounded = round(*ite * calculator->Express10())/calculator->Express10();
			if (rounded == 0)
				continue;

			output.append(QString("%0 ").arg(rounded > 0? "+" : "-"));

			if (rounded != 1 && rounded != -1 || pol_grade == 0)
				output.append(QString("%1").arg(rounded < 0? rounded * -1 : rounded));

			if (pol_grade > 0) {
				output.append("x");
				if (pol_grade > 1)
					output.append(QString("<sup>%1</sup>").arg(pol_grade));
			}


			output.append(" ");
		}

		polynom_line->setText(output);
		delete(polynom);

		QVector<double> x_points;
		QVector<double> y_points;
		for (int i = -25; i < 25; i++) {
			x_points.push_back(i);
			y_points.push_back(calculator->CalculateInPoint(i));
		}

		function->setSamples(x_points, y_points);
		function->attach(plot);

		plot->replot();
	}

	catch(Renal::NextException &e) {
		polynom_line->setText(e.GetMessage()->c_str());
		calculator->Clear();
	}
}

void MainUI::CreateMenus() {
	QMenu *fileMenu = menuBar()->addMenu("&File");

	QAction *exit = fileMenu->addAction("Exit");
	exit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	exit->setStatusTip(tr("Exits from Sensitive"));


	QMenu *helpMenu = menuBar()->addMenu("&Help");
	QAction *about = helpMenu->addAction("About");
	about->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	about->setStatusTip(tr("Show a dialog box with some informations about Sensitive"));

	QAction *aboutQT= helpMenu->addAction("About QT");
	aboutQT->setStatusTip(tr("About QT framework"));

	QObject::connect(exit, SIGNAL(triggered()), this, SLOT(close()));
	QObject::connect(about, SIGNAL(triggered()), AboutDialog::GetInstance(), SLOT(exec()));
	QObject::connect(aboutQT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}



void MainUI::CalculateInPoint() {
	bool result;
	double point = input_point->text().toDouble(&result);

	if (result) {
		try {
			output_point->setText(QString("f(%0) = %1").arg(point).arg(calculator->CalculateInPoint(point)));
		}
		catch (Renal::NextException &e) {
			QString message(e.GetMessage()->c_str());
			output_point->setText(message);
		}
	}
}

void MainUI::ShowAboutDialog() {

}

}

