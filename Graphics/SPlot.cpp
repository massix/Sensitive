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

#include "SPlot.h"

#include <QtGui>
#include <QtCore>
#include <vector>

namespace Graphics {


SPlot::~SPlot() {

}

void SPlot::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasText() && event->mimeData()->urls().isEmpty())
		event->acceptProposedAction();

	else if (!event->mimeData()->urls().isEmpty()) {
		foreach(QUrl url, event->mimeData()->urls()) {
			if (QFileInfo(url.toLocalFile()).suffix().toLower() == "sns")
				event->acceptProposedAction();
		}
	}
}

void SPlot::dropEvent(QDropEvent *event) {
	std::vector<std::pair<double, double> > *newCoords = new std::vector<std::pair<double, double> >();

	if (event->mimeData()->hasText() && event->mimeData()->urls().isEmpty()) {
		QString text = event->mimeData()->text();

		/* Parse Text */
		QStringList lines = text.split('\n');

		if (lines.size() < 2)
			return;

		foreach (QString s, lines) {
			if (s.startsWith('#'))
				continue;

			bool result_x, result_y;
			double x, y;
			QStringList coords = s.split(':');

			if (coords.size() != 2)
				continue;

			x = coords.first().toDouble(&result_x);
			y = coords.last().toDouble(&result_y);

			if (result_x && result_y)
				newCoords->push_back(std::pair<double, double>(x, y));

		}
	}

	else if (!event->mimeData()->urls().isEmpty()) {
		foreach(QUrl url, event->mimeData()->urls()) {
			QFileInfo fileInfo(url.toLocalFile());

			if (fileInfo.suffix().toLower() == "sns") {
				QFile file(url.toLocalFile());
				if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					QTextStream stream(&file);

					/* Parse the file */
					while (!stream.atEnd()) {
						QString read = stream.readLine();
						if (read.startsWith('#'))
							continue;
						QStringList coords = read.split(':');

						if (coords.size() != 2)
							continue;

						bool result_x, result_y;
						double x, y;

						x = coords.first().toDouble(&result_x);
						y = coords.last().toDouble(&result_y);

						if (result_x && result_y)
							newCoords->push_back(std::pair<double, double>(x, y));
					}

					file.close();
				}

			}
		}
	}

	else
		return;

	emit DropAccepted(newCoords);
}

}
