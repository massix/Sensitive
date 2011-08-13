/*
 * PlotterThread.cpp
 *
 *  Created on: 13/ago/2011
 *      Author: Massi
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
