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
#include "../Config.h"

#include "SPlot.h"
#include "STableWidget.h"
#include "AboutDialog.h"
#include "ServerWindow.h"
#include "ClientWindow.h"
#include "PlotterThread.h"

#include <vector>
#include <cmath>

#include <QtCore>
#include <QtGui>

#include <QPropertyAnimation>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>

#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/NextSpline.h>

#include <SensitiveProtocol/SensitiveServer.h>
#include <SensitiveProtocol/SensitiveClient.h>
#include <SensitiveProtocol/SensitiveException.h>

#define MAX_ERROR 0.00000001

namespace Graphics {

MainUI::MainUI(Renal::NextCalculator *calculator, QString calculator_name) :
				QMainWindow(), calculator(calculator), calculator_name(calculator_name)
{
	setWindowTitle(QString("Sensitive UI - %0").arg(this->calculator_name));
	setWindowIcon(QIcon(":/bundle/icon.svg"));

	/* Defining layout */
	fixed_widget = new QWidget();
	docked_widget = new QWidget();
	plot_config_widget = new QWidget();

	coords_dock = new QDockWidget("Coordinates");
	plot_config_dock = new QDockWidget("Plot configuration");

	docked_layout = new QVBoxLayout(docked_widget);
	plot_config_layout = new QVBoxLayout(plot_config_widget);

	central_layout = new QVBoxLayout(fixed_widget);
	bottom_buttons = new QHBoxLayout();

	addDockWidget(Qt::LeftDockWidgetArea, coords_dock);
	addDockWidget(Qt::LeftDockWidgetArea, plot_config_dock);
	splitDockWidget(coords_dock, plot_config_dock, Qt::Vertical);

	coords_dock->setWidget(docked_widget);
	plot_config_dock->setWidget(plot_config_widget);

	/* ++++ BUILDING UP THE PLOT CONFIGURATION WIDGET ++++ */

	plot_config_from = new QDoubleSpinBox();
	plot_config_to = new QDoubleSpinBox();
	plot_config_step = new QDoubleSpinBox();
	plot_config_pbar = new QProgressBar();
	plot_config_benchmark = new QLabel(":-)");
	plot_config_autodetect = new QPushButton("Autodetect");
	plot_config_replot = new QPushButton("Replot");

	plot_config_step->setRange(0., 100.);
	plot_config_step->setDecimals(3);

	plot_config_from->setRange(-1000., 1000.);
	plot_config_from->setDecimals(10);

	plot_config_to->setRange(-1000., 1000.);
	plot_config_to->setDecimals(10);

	plot_config_buttons = new QHBoxLayout();
	plot_config_buttons->addWidget(plot_config_autodetect);
	plot_config_buttons->addWidget(plot_config_replot);

	plot_config_spinners = new QHBoxLayout();
	plot_config_spinners->addWidget(plot_config_from);
	plot_config_spinners->addWidget(plot_config_to);
	plot_config_spinners->addWidget(plot_config_step);

	plot_config_layout->addWidget(new QLabel("Set the domain and the step"));
	plot_config_layout->addLayout(plot_config_spinners);
	plot_config_layout->addLayout(plot_config_buttons);
	plot_config_layout->addWidget(plot_config_pbar);
	plot_config_layout->addWidget(plot_config_benchmark);

	plot_config_dock->setMaximumHeight(plot_config_dock->sizeHint().height());

	QObject::connect(plot_config_replot, SIGNAL(clicked()), this, SLOT(Replot()));
	QObject::connect(plot_config_autodetect, SIGNAL(clicked()), this, SLOT(AutoDetect()));
	QObject::connect(plot_config_autodetect, SIGNAL(clicked()), this, SLOT(Replot()));

	/* ++++ BUILDING UP THE DOCKED WIDGET WHICH WILL CONTAIN THE COORDINATES TABLE ++++ */

	/* Table Widget that will contain the coordinates */
	coords_table = new STableWidget(1, 2);

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
	interpole = new QPushButton("Interpolate!");

	bottom_buttons->addWidget(add_coord);
	bottom_buttons->addWidget(delete_coord);
	bottom_buttons->addWidget(reset_coords);
	bottom_buttons->addWidget(interpole);

	docked_layout->addLayout(bottom_buttons);

	/* Connecting signals */
	QObject::connect(add_coord, SIGNAL(clicked()), this, SLOT(AddCoord()));
	QObject::connect(delete_coord, SIGNAL(clicked()), this, SLOT(DeleteCoord()));
	QObject::connect(reset_coords, SIGNAL(clicked()), coords_table, SLOT(clearContents()));
	QObject::connect(reset_coords, SIGNAL(clicked()), this, SLOT(Reset()));
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

	plot = new SPlot();
	plot->setTitle("Welcome to Sensitive");

	abscissas = new QwtPlotScaleItem(QwtScaleDraw::RightScale, 0.0);
	abscissas->attach(plot);

	ordinates = new QwtPlotScaleItem(QwtScaleDraw::BottomScale, 0.0);
	ordinates->attach(plot);

	function = new QwtPlotCurve("f(x)");
	function->setTitle("f(x)");
	function->setStyle(QwtPlotCurve::Lines);

	plot->enableAxis(QwtPlot::yLeft, false);
	plot->enableAxis(QwtPlot::xBottom, false);

	central_layout->addWidget(plot);
	central_layout->addLayout(results_grid);

	/* Connecting signals */
	QObject::connect(input_point, SIGNAL(returnPressed()), this, SLOT(CalculateInPoint()));
	QObject::connect(plot, SIGNAL(DropAccepted(std::vector<std::pair<double, double> > *)),
			coords_table, SLOT(RefreshCoords(std::vector<std::pair<double, double> >*)));
	QObject::connect(coords_table, SIGNAL(DropFinished()), this, SLOT(Interpole()));

	printer = new QPrinter();
	painter = new QPainter();

	plot_renderer = new QwtPlotRenderer();
	plot_renderer->setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
	plot_renderer->setDiscardFlag(QwtPlotRenderer::DiscardTitle, true);
	plot_renderer->setDiscardFlag(QwtPlotRenderer::DiscardLegend, true);

	statusBar()->show();
	statusBar()->showMessage("Welcome to Sensitive UI. Please feel free to smoke a joint while interpolating.");

	server = new Protocol::SensitiveServer();
	client = new Protocol::SensitiveClient();

	serverWindow = new ServerWindow(this);
	QObject::connect(serverWindow, SIGNAL(StartServer(quint16)), this, SLOT(ServerConfigured(quint16)));
	QObject::connect(serverWindow, SIGNAL(CancelServer()), this, SLOT(ServerDelete()));
	QObject::connect(serverWindow, SIGNAL(rejected()), server, SLOT(Shutdown()));
	QObject::connect(server, SIGNAL(SynAcquired(QString&)), serverWindow, SLOT(Progress25(QString&)));
	QObject::connect(server, SIGNAL(BcooAcquired(int)), serverWindow, SLOT(Progress50(int)));
	QObject::connect(server, SIGNAL(CoorAcquired()), serverWindow, SLOT(Progress75()));
	QObject::connect(server, SIGNAL(CoorAcquired()), this, SLOT(ServerFinished()));

	clientWindow = new ClientWindow(this);
	QObject::connect(clientWindow, SIGNAL(ClientConfigured(QString&, quint16)), this, SLOT(ClientConfigured(QString&, quint16)));
	QObject::connect(clientWindow, SIGNAL(rejected()), client, SLOT(ResetClient()));

	CreateMenus();

	setCentralWidget(fixed_widget);

	innerThread = new SThread();
	innerThread->SetCalculator(calculator);
	QObject::connect(innerThread, SIGNAL(finished()), this, SLOT(InterpoleOver()));

	plotterThread = new PlotterThread();
	plotterThread->SetCalculator(calculator);
	plotterThread->SetPlot(plot);
	plotterThread->SetPlotCurve(function);
	QObject::connect(plotterThread, SIGNAL(StepDone(int)), plot_config_pbar, SLOT(setValue(int)));
	QObject::connect(plotterThread, SIGNAL(finished()), this, SLOT(ReplotOver()));

	progressBar = new QProgressBar(this);
	docked_layout->addWidget(progressBar);
	progressBar->setVisible(false);

	/* Build a spline calculator to use as a fallback calculator */
	spline_calculator = new Renal::NextSpline();

	showMaximized();
}

MainUI::~MainUI() {
	delete(calculator);
	delete(printer);
	delete(painter);
	delete(innerThread);
	delete(plotterThread);
	delete(server);
	delete(client);
	delete(spline_calculator);
}

void MainUI::CheckData(QTableWidgetItem * data) {

}

void MainUI::AddCoord() {
	coords_table->setRowCount(coords_table->rowCount() + 1);
}

void MainUI::DeleteCoord() {
	coords_table->removeRow(coords_table->currentRow());
}

void MainUI::Reset() {
	function->detach();
	plot->replot();

	input_point->clear();
	output_point->clear();
	polynom_line->clear();

	calculator->Clear();
}


void MainUI::Interpole() {
	if (calculator == 0)
		return;

	calculator->Clear();
	spline_calculator->Clear();

	/* Fill up both the default calculator and the spline one */
	for (int row = 0; row < coords_table->rowCount(); row++) {
		if (coords_table->item(row, 0) != 0 && coords_table->item(row, 1) != 0) {
			double x, y;
			bool result_x, result_y;

			x = coords_table->item(row, 0)->text().toDouble(&result_x);
			y = coords_table->item(row, 1)->text().toDouble(&result_y);

			if (result_x && result_y) {
				calculator->InsertCoords(x, y);
				spline_calculator->InsertCoords(x, y);
			}
		}
	}

	/* Set the default calculator to be the user requested one */
	innerThread->SetCalculator(calculator);
	plotterThread->SetCalculator(calculator);
	splined = false;

	/* If there are too much coordinates set, ask the user if he would like to Spline */
	if (calculator->CountCoords() > COORDS_SPLINEABLE) {
		QMessageBox spline;
		spline.setText("Would you like to spline?");
		spline.setInformativeText(QString("Since you've inserted a large set of coordinates (%0), "
				"the %1 may be innacurate or the resulting polynom may be of a very high degree. "
				"Would you like to use a cubic spline?").arg(calculator->CountCoords()).arg(calculator_name));

		spline.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		switch (spline.exec()) {
		case QMessageBox::Yes:
			innerThread->SetCalculator(spline_calculator);
			plotterThread->SetCalculator(spline_calculator);
			splined = true;
			break;
		default:
			break;
		}
	}

	AutoDetect();

	progressBar->setTextVisible(false);
	progressBar->setRange(0, 0);
	progressBar->setVisible(true);

	statusBar()->showMessage("Interpolating..");
	interpole->setEnabled(false);
	innerThread->start();
}

void MainUI::Replot() {

	plotterThread->SetMin(plot_config_from->value());
	plotterThread->SetMax(plot_config_to->value());
	plotterThread->SetStep(plot_config_step->value());


	plot_config_pbar->setRange(0, plotterThread->TotalSteps());

	plotterThread->start();
}

void MainUI::ReplotOver() {
	plot->replot();
	plot_config_benchmark->setText(QString("Calculated %0 points in %1.%2s")
			.arg(plotterThread->TotalSteps())
			.arg(plotterThread->GetElapsed()/1000)
			.arg(plotterThread->GetElapsed()));
}

void MainUI::AutoDetect() {
	plot_config_from->setValue(innerThread->GetCalculator()->GetXmin());
	plot_config_to->setValue(innerThread->GetCalculator()->GetXmax());
	plot_config_step->setValue(0.5);
}

void MainUI::InterpoleOver() {
	progressBar->setRange(0, 100);
	progressBar->reset();
	progressBar->setVisible(false);

	interpole->setEnabled(true);

	if (innerThread->HadException()) {
		Reset();
		statusBar()->showMessage(":-(");
		polynom_line->setText(innerThread->GetException()->what());
	}

	else {
		Renal::NextCalculator *used_calculator = (splined? calculator : innerThread->GetCalculator());

		/* If we used the spline, let's calculate a 3rd degree polynom using the default calculator */
		/* This will give us an approximation of the splined polynom */
		if (splined) {
			calculator->Clear();

			int average_pt = (innerThread->GetCalculator()->GetXmin() + innerThread->GetCalculator()->GetXmax()) / 2;

			/* Interpole around the average point */
			for (int i = average_pt - 2; i < average_pt + 2; i++)
				calculator->InsertCoords(i, innerThread->GetCalculator()->CalculateInPoint(i));

			calculator->BuildFunction();
		}

		statusBar()->showMessage(QString("Polynom calculated in %0.%1s")
				.arg(innerThread->GetElapsed()/1000)
				.arg(innerThread->GetElapsed()));
		std::vector<double> *polynom = used_calculator->GetPolynom();

		QString output(QString("<i>%0(x)</i> = ").arg(splined? "Spline" : "f"));

		if (polynom->size() > 0) {
			int pol_grade = polynom->size()-1;
			for (std::vector<double>::iterator ite = polynom->begin(); ite != polynom->end(); ++ite, --pol_grade) {
				if (*ite == 0)
					continue;

				double rounded = round(*ite * used_calculator->Express10())/used_calculator->Express10();
				if (rounded == 0)
					continue;

				output.append(QString("%0 ").arg(rounded > 0? "+" : "-"));

				if ((rounded != 1 && rounded != -1) || pol_grade == 0)
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

			Replot();
		}
	}
}

void MainUI::CreateMenus() {
	QMenu *fileMenu = menuBar()->addMenu("&File");

	QMenu *expor = fileMenu->addMenu("&Export");
	QAction *toPdf = expor->addAction("Document to &PDF");
	QAction *toTxt = expor->addAction("Coordinates to &SNS");

	toPdf->setStatusTip("Exports the whole document to a PDF file");
	toTxt->setStatusTip("Exports the coordinates into a Sensitive's coordinates file");

	QAction *exit = fileMenu->addAction("Exit");
	exit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	exit->setStatusTip(tr("Exits from Sensitive"));

	QMenu *exchangeMenu = menuBar()->addMenu("E&xchange");
	QAction *serverStart = exchangeMenu->addAction("Start &server (receive)");
	QAction *clientStart = exchangeMenu->addAction("Start &client (send)");

	serverStart->setStatusTip("Receive coordinates from a remote client");
	clientStart->setStatusTip("Send coordinates to a remote host");

	QMenu *helpMenu = menuBar()->addMenu("&Help");
	QAction *about = helpMenu->addAction("About");
	about->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	about->setStatusTip(tr("Show a dialog box with some informations about Sensitive"));

	QAction *aboutQT= helpMenu->addAction("About Qt");
	aboutQT->setStatusTip(tr("About Qt framework"));

	QObject::connect(exit, SIGNAL(triggered()), this, SLOT(close()));
	QObject::connect(about, SIGNAL(triggered()), AboutDialog::GetInstance(), SLOT(exec()));
	QObject::connect(aboutQT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	QObject::connect(toPdf, SIGNAL(triggered()), this, SLOT(ExportPDF()));
	QObject::connect(toTxt, SIGNAL(triggered()), this, SLOT(ExportSNS()));
	QObject::connect(serverStart, SIGNAL(triggered()), serverWindow, SLOT(show()));
	QObject::connect(clientStart, SIGNAL(triggered()), clientWindow, SLOT(show()));
}



void MainUI::CalculateInPoint() {
	bool result;
	double point = input_point->text().toDouble(&result);

	if (result) {
		try {
			QTime *timer = new QTime();

			timer->start();
			output_point->setText(QString("f(%0) = %1").arg(point)
					.arg(innerThread->GetCalculator()->CalculateInPoint(point)));
			int elapsed = timer->elapsed();

			QString msg(QString("Point %0 calculated in %1.%2s").arg(point).arg(elapsed/1000).arg(elapsed));
			statusBar()->showMessage(msg);
			delete(timer);
		}
		catch (Renal::NextException &e) {
			QString message(e.what());
			output_point->setText(message);
		}
	}
}


void MainUI::ExportPDF() {
	QString filename = QFileDialog::getSaveFileName(this, "Export to PDF", QDir::homePath(), "PDF Files (*.pdf)");

	if (filename.isNull() || filename.isEmpty())
		return;

	printer->setOutputFormat(QPrinter::PdfFormat);
	printer->setOutputFileName(filename);


	/* Build up the HTML */
	QTextDocument *page = new QTextDocument();
	QString htmlPage(QString("<html>"
			"<head>"
			"	<title>Sensitive</title>"
			"		<style type=\"text/css\">"
			"			#gen { text-decoration: underline; width: 100% }"
			"			.pol { text-decoration: strong; font-size: 1.5em; border: 1px dotted black"
			"		</style>"
			"</head>"
			"<body>"
			"<h1>%0 interpolation</h1>"
			"<span align=\"right\" id=\"gen\">Generated by Sensitive on %1</span><br />")
			.arg(calculator_name).arg(QDate::currentDate().toString("dddd, d MMMM yyyy")));

	htmlPage.append(QString("<span align=\"center\" class=\"pol\">%0<br />").arg(polynom_line->document()->toHtml()));
	htmlPage.append(QString("%0</span><br />").arg(output_point->text()));

	htmlPage.append("<h2>Interpolation pts</h2>");
	htmlPage.append("<table width=\"150\" style=\"border: 1px solid black; border-collapse: collapse\">"
			"<thead style=\"border-bottom: 2px solid black\"><tr><th>X</th><th>Y</th></tr></thead>");

	/* Build the coordinates table */
	for (int lines = 0; lines < coords_table->rowCount(); lines++) {
		if (coords_table->item(lines, 0) == 0 || coords_table->item(lines, 1) == 0)
			continue;

		htmlPage.append(QString("<tr><td>%0</td><td>%1</td></tr>")
				.arg(coords_table->item(lines, 0)->text()).arg(coords_table->item(lines, 1)->text()));
	}

	htmlPage.append("</table><br />");

	/* "Interesting points" */
	htmlPage.append("<h2>Interesting pts</h2>");
	htmlPage.append(QString("f(-1) = %0<br />").arg(calculator->CalculateInPoint(-1)));
	htmlPage.append(QString("f(0) = %0<br />").arg(calculator->CalculateInPoint(0)));
	htmlPage.append(QString("f(1) = %0<br />").arg(calculator->CalculateInPoint(1)));


	htmlPage.append("</body></html>");

	page->setUndoRedoEnabled(false);
	page->setTextWidth(printer->pageRect().width());
	page->setHtml(htmlPage);

	/* Now render it */

	painter->begin(printer);


	/* Render HTML */
	painter->translate(QPoint(printer->pageRect().x(), printer->pageRect().y()));

	/* Set 90% width and 100% height */
	painter->scale(0.9, 1.0);

	/* Draw the HTML page */
	page->drawContents(painter);


	/* Render Plot */
	painter->translate(QPoint(printer->pageRect().x() + 150, printer->pageRect().y() + 80));
	plot_renderer->render(plot, painter, QRect(0, 0, 550, 400));


	painter->end();

	delete(page);
}


void MainUI::ExportSNS() {
	QString filename = QFileDialog::getSaveFileName(this, "Export to SNS", QDir::homePath(), "Sensitive Files (*.sns)");

	if (filename.isNull() || filename.isEmpty())
		return;

	QFile file(filename);

	if (!file.open(QFile::WriteOnly))
		return;

	QTextStream stream(&file);

	stream << "## Generated by Sensitive on " << QString("%0").arg(QDate::currentDate().toString("dd MM yy")) << endl;
	for (int lines = 0; lines < coords_table->rowCount(); lines++) {
		if (coords_table->item(lines, 0) == 0 || coords_table->item(lines, 1) == 0)
			continue;

		stream << coords_table->item(lines, 0)->text() << ":" << coords_table->item(lines, 1)->text() << endl;
	}

	file.close();
}

void MainUI::ServerDelete() {
	server->Shutdown();
	serverWindow->hide();
}

void MainUI::ServerConfigured(quint16 port) {
	try {
		server->StartServer(port);
		serverWindow->Progress0();
	}

	catch (Protocol::SensitiveException& e) {
		QMessageBox::information(this, "Exception occured", QString(e.GetMessage().c_str()));
	}
}

void MainUI::ServerFinished() {
	serverWindow->Progress100();
	/* Grab coordinates from the server */

	std::vector<std::pair<double, double> >* newCoords = server->GetCoordinates();
	coords_table->RefreshCoords(newCoords);
	ServerDelete();
}


void MainUI::ClientConfigured(QString& hostname, quint16 port) {
	try {
		std::vector<std::pair<double, double> > *tmp = coords_table->GetCoordinates();
		std::vector<std::pair<double, double> > coordsVector(*tmp);
		delete(tmp);

		client->SetCoordinates(coordsVector);
		client->InitiateProtocol(hostname, port);
	}
	catch (Protocol::SensitiveException& e) {
		QMessageBox::information(this, "Exception occured", QString(e.GetMessage().c_str()));
	}
}

}

