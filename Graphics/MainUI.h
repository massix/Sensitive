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

#ifndef MAINUI_H_
#define MAINUI_H_

#include "STableWidget.h"
#include "SPlot.h"
#include "ServerWindow.h"
#include "ClientWindow.h"

#include <iostream>

#include <QObject>
#include <QtGui>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_scaleitem.h>

#include <Renal/NextCalculator.h>

#include <SensitiveProtocol/SensitiveClient.h>
#include <SensitiveProtocol/SensitiveServer.h>
#include <SensitiveProtocol/SensitiveException.h>

namespace Graphics {

class MainUI : public QMainWindow {

	Q_OBJECT

public:
	MainUI(Renal::NextCalculator *calculator, QString calculator_name);
	virtual ~MainUI();

private:
	QString				calculator_name;
	QWidget				*fixed_widget;
	QWidget				*docked_widget;

	/* Main Layout elements */
	QDockWidget			*coords_dock;
	QVBoxLayout			*docked_layout;
	QVBoxLayout			*central_layout;
	QHBoxLayout			*bottom_buttons;

	/* Table fullfilled with coordinates */
	STableWidget		*coords_table;

	/* Coordinates management buttons */
	QPushButton			*reset_coords;
	QPushButton			*add_coord;
	QPushButton			*delete_coord;
	QPushButton			*interpole;

	/* Results */
	QGridLayout			*results_grid;
	QTextEdit			*polynom_line;
	QLineEdit			*input_point;
	QLineEdit			*output_point;

	/* Plot Canvas */
	SPlot				*plot;
	QwtPlotCurve		*function;
	QwtPlotScaleItem	*abscissas;
	QwtPlotScaleItem	*ordinates;

	/* Calculator */
	Renal::NextCalculator *calculator;

	/* Printer device */
	QPrinter			*printer;
	QPainter			*painter;
	QwtPlotRenderer		*plot_renderer;

	/* Server and client pointers, not initialized until needed */
	Protocol::SensitiveServer	*server;
	Protocol::SensitiveClient 	*client;

	/* Server and client windows */
	ServerWindow		*serverWindow;
	ClientWindow		*clientWindow;

private slots:
	void			AddCoord();
	void			DeleteCoord();
	void			Interpole();
	void			CheckData(QTableWidgetItem* data);
	void			CalculateInPoint();
	void			ExportPDF();
	void			ExportSNS();

	/* Server and client stuffs */
	void			ServerConfigured(quint16 port);
	void			ServerDelete();
	void			ServerFinished();

	void			ClientConfigured(QString& hostname, quint16 port);

private:
	void			CreateMenus();


signals:

};

}

#endif /* MAINUI_H_ */
