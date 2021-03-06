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

#include "STableWidget.h"

#include <QtGui>
#include <vector>

namespace Graphics {

void STableWidget::RefreshCoords(std::vector<std::pair<double, double> > *newCoords) {
	clearContents();

	int row = 1;
	for (std::vector<std::pair<double, double> >::iterator ite = newCoords->begin(); ite != newCoords->end(); ++ite, row++) {
		setRowCount(row);

		setItem(row-1, 0, new QTableWidgetItem(QString("%0").arg(ite->first)));
		setItem(row-1, 1, new QTableWidgetItem(QString("%0").arg(ite->second)));
	}

	delete(newCoords);

	emit DropFinished();
}

std::vector<std::pair<double, double> >* STableWidget::GetCoordinates() {
	std::vector<std::pair<double, double> >* coords = new std::vector<std::pair<double, double> >();

	for (int lines = 0; lines < rowCount(); lines++) {
		if (item(lines, 0) == 0 || item(lines, 1) == 0)
			continue;

		bool ok_x, ok_y;

		double x = item(lines, 0)->text().toDouble(&ok_x);
		double y = item(lines, 1)->text().toDouble(&ok_y);

		if (ok_x && ok_y)
			coords->push_back(std::pair<double, double>(x, y));
	}

	return coords;
}

void STableWidget::clearContents() {
	QTableWidget::clearContents();

	setRowCount(0);
}

STableWidget::~STableWidget() {

}

}
