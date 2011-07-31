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

#include <QObject>
#include <QtGui>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <Renal/NextCalculator.h>

namespace Graphics {

enum InnerRoles {
	COORD
};

class MainUI : public QMainWindow {

	Q_OBJECT

public:
	MainUI(Renal::NextCalculator *calculator);
	virtual ~MainUI();

private:
	QWidget				*fixed_widget;
	QWidget				*docked_widget;

	/* Main Layout elements */
	QDockWidget			*coords_dock;
	QVBoxLayout			*docked_layout;
	QVBoxLayout			*central_layout;
	QHBoxLayout			*bottom_buttons;

	/* Table fullfilled with coordinates */
	QTableWidget		*coords_table;

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
	QwtPlot				*plot;
	QwtPlotCurve		*function;

	/* Calculator */
	Renal::NextCalculator *calculator;

public slots:
	void			AddCoord();
	void			DeleteCoord();
	void			Interpole();
	void			CheckData(QTableWidgetItem* data);
	void			CalculateInPoint();

private slots:
	void			ShowAboutDialog();

private:
	void			CreateMenus();


signals:

};

}

#endif /* MAINUI_H_ */
