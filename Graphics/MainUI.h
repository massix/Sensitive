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
#include "PlotterThread.h"

#include <iostream>

#include <QObject>
#include <QtGui>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_scaleitem.h>

#define COORDS_SPLINEABLE 10
#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/NextSpline.h>

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
	QWidget				*plot_config_widget;

	/* Main Layout elements */
	QDockWidget			*coords_dock;
	QDockWidget			*plot_config_dock;
	QVBoxLayout			*docked_layout;
	QVBoxLayout			*central_layout;
	QVBoxLayout			*plot_config_layout;
	QHBoxLayout			*plot_config_spinners;
	QHBoxLayout			*plot_config_buttons;
	QHBoxLayout			*bottom_buttons;

	/* Table fullfilled with coordinates */
	STableWidget		*coords_table;

	/* Plot config spinners and buttons */
	QDoubleSpinBox		*plot_config_from;
	QDoubleSpinBox		*plot_config_to;
	QDoubleSpinBox		*plot_config_step;
	QPushButton			*plot_config_replot;
	QPushButton			*plot_config_autodetect;
	QLabel				*plot_config_benchmark;

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
	Renal::NextCalculator 	*calculator;

	/* Backup calculator */
	Renal::NextSpline		*spline_calculator;

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

	/* Interpolation thread */
	class SThread : public QThread {
	private:
		Renal::NextCalculator *calculator;
		QTime *timer;
		int elapsed;
		bool exception;
		Renal::NextException *e;

	public:
		SThread() : QThread(), elapsed(0), exception(false), e(0) {timer = new QTime();}

		~SThread() {
			delete(timer);
			if (exception)
				delete(e);
		}

		Renal::NextException* GetException() {
			return e;
		}

		void SetCalculator(Renal::NextCalculator *c) {
			calculator = c;
		}

		Renal::NextCalculator* GetCalculator() {
			return calculator;
		}

		int GetElapsed() {
			return elapsed;
		}

		bool HadException() {
			return exception;
		}

		void run() {
			timer->start();
			exception = false;

			try {
				calculator->BuildFunction();
			} catch (Renal::NextException& e) {
				exception = true;
				this->e = new Renal::NextException(e);
			}

			elapsed = timer->elapsed();
		}
	} *innerThread;

	PlotterThread	*plotterThread;

	QProgressBar	*progressBar;
	QProgressBar	*plot_config_pbar;

private slots:
    void AddCoord();
    void DeleteCoord();
    void Interpole();
    void InterpoleOver();
    void CheckData(QTableWidgetItem *data);
    void CalculateInPoint();
    void ExportPDF();
    void ExportSNS();
    void Reset();
    /* Server and client stuffs */
    void ServerConfigured(quint16 port);
    void ServerDelete();
    void ServerFinished();
    void ClientConfigured(QString & hostname, quint16 port);

    void Replot();
    void ReplotOver();
    void AutoDetect();
private:
    void CreateMenus();
protected:

};

}

#endif /* MAINUI_H_ */
