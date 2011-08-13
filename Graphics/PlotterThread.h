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
