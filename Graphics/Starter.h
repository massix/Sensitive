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

#ifndef STARTER_H_
#define STARTER_H_

#include "SLabel.h"

#include <QtGui>
#include <QtCore>
#include <QUiLoader>

namespace Graphics {

enum InterpolationForm {
	NEWTON = 2,
	LAGRANGE = 3,
	VANDERMONDE = 4
};

class Starter : public QDialog {

	Q_OBJECT

public:
	Starter();
	virtual ~Starter();

private:
	QUiLoader		*loader;
	QFile			*ui_file;
	QWidget			*loaded_widget;
	QVBoxLayout		*main_layout;
	QTextEdit		*description;

	QList<QString>	buttons_list;


	InterpolationForm selected;

	QPropertyAnimation		*animation;

private slots:
	void	Accept();
	void	RadioClicked();

};

}

#endif /* STARTER_H_ */
