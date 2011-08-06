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

#ifndef STABLEWIDGET_H_
#define STABLEWIDGET_H_

#include <QtGui>
#include <vector>

namespace Graphics {

class STableWidget : public QTableWidget {

	Q_OBJECT

public:
	STableWidget(int rows, int cols, QWidget *parent = 0) : QTableWidget(rows, cols, parent) {};
	virtual ~STableWidget();

	std::vector<std::pair<double, double> >* GetCoordinates();

public:


public slots:
	void	RefreshCoords(std::vector<std::pair<double, double> > *newCoords);

	/* Override */
	void	clearContents();

signals:
	void	DropFinished();
};

}

#endif /* STABLEWIDGET_H_ */
