/*
 * PlotterThread.h
 *
 *  Created on: 13/ago/2011
 *      Author: Massi
 */

#ifndef PLOTTERTHREAD_H_
#define PLOTTERTHREAD_H_

#include <QtCore>
#include <QtGui>

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>

namespace Graphics {

class PlotterThread : public QThread {

	Q_OBJECT

public:
    void run();
	void SetCalculator(Renal::NextCalculator *calculator);
    void SetMin(double min);
    void SetMax(double max);
    void SetStep(double step);
    void SetPlot(QwtPlot *plot);
    void SetPlotCurve(QwtPlotCurve *plot_curve);
    int  TotalSteps();
    int  GetElapsed();

private:
    Renal::NextCalculator *calculator;
    QwtPlot *plot;
    QwtPlotCurve *plot_curve;
    double min, max, step;
    QTime *timer;
    int elapsed;

signals:
		void StepDone(int step);

public:
	PlotterThread() : QThread(), elapsed(0) {timer = new QTime();};
	virtual ~PlotterThread() {};
};

} /* namespace Graphics */
#endif /* PLOTTERTHREAD_H_ */
