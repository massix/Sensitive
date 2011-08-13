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

#include "PlotterThread.h"

#include <cmath>

namespace Graphics {

void PlotterThread::SetCalculator(Renal::NextCalculator *calculator) {
	this->calculator = calculator;
}

void PlotterThread::SetMin(double min) {
	this->min = min;
}

void PlotterThread::SetMax(double max) {
	this->max = max;
}

void PlotterThread::SetStep(double step) {
	this->step = step;
}

void PlotterThread::SetPlot(QwtPlot *plot) {
	this->plot = plot;
}

void PlotterThread::SetPlotCurve(QwtPlotCurve *plot_curve) {
	this->plot_curve = plot_curve;
}

int PlotterThread::GetElapsed() {
	return elapsed;
}

void PlotterThread::run() {
	try {
		QVector<double> x_points;
		QVector<double> y_points;
		int steps = 0;
		timer->start();
		for(double i = min;i <= max;i += step, steps++){
			x_points.push_back(i);
			y_points.push_back(calculator->CalculateInPoint(i));
			emit StepDone(steps);
		}

		emit StepDone(TotalSteps());
		elapsed = timer->elapsed();

		plot_curve->setSamples(x_points, y_points);
		plot_curve->attach(plot);
	} catch (Renal::NextException& e) {

	}
}

int PlotterThread::TotalSteps() {
	return round((max - min)/step);
}

} /* namespace Graphics */
