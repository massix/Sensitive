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

#include "Starter.h"

#include <QtGui>
#include <QtCore>


#define NEWTON_DESCRIPTION "<h1>Newton Polynomial</h1>\
In the mathematical field of numerical analysis, a Newton polynomial, named after its inventor Isaac Newton, is the interpolation polynomial for a given set of data points in the Newton form. \
The Newton polynomial is sometimes called Newton's divided differences interpolation polynomial because the coefficients of the polynomial are calculated using divided differences. \
For any given set of data points, there is only one polynomial (of least possible degree) that passes through all of them. \
Thus, it is more appropriate to speak of \"the Newton form of the interpolation polynomial\" rather than of \"the Newton interpolation polynomial\". <br /><br />\
Like the Lagrange form, it is merely another way to write the same polynomial.<br /><br /> \
Source: <a href=\"http://en.wikipedia.org/wiki/Newton_polynomial\">Wikipedia</a>"

#define LAGRANGE_DESCRIPTION "<h1>Lagrange Polynomial</h1> \
In numerical analysis, Lagrange polynomials are used for polynomial interpolation. For a given set of distinct points xj and numbers yj, the Lagrange polynomial is the polynomial \
of the least degree that at each point xj assumes the corresponding value yj (i.e. the functions coincide at each point). The interpolating polynomial of the least degree is unique, \
however, and it is therefore more appropriate to speak of \"the Lagrange form\" of that unique polynomial rather than \"the Lagrange interpolation polynomial,\" since the same polynomial \
can be arrived at through multiple methods. Although named after Joseph Louis Lagrange, it was first discovered in 1779 by Edward Waring and rediscovered in 1783 by Leonhard Euler. <br /> \
Lagrange interpolation is susceptible to Runge's phenomenon, and the fact that changing the interpolation points requires recalculating the entire interpolant can make \
Newton polynomials easier to use. <br /><br /> \
Lagrange polynomials are used in the Newton-Cotes method of numerical integration and in Shamir's secret sharing scheme in Cryptography.<br /><br /> \
Source: <a href=\"http://en.wikipedia.org/wiki/Lagrange_polynomial\">Wikipedia</a>"

#define VANDERMONDE_DESCRIPTION "<h1>Vandermonde's Matrix</h1> \
<b>Warning: the Vandermonde's Matrix interpolates only over up to 4 interpolation points!</b><br /> \
The Vandermonde matrix evaluates a polynomial at a set of points; formally, it transforms coefficients of a polynomial  to the values the polynomial takes at the points ai. \
The non-vanishing of the Vandermonde determinant for distinct points αi shows that, for distinct points, the map from coefficients to values at those points is a \
one-to-one correspondence, and thus that the polynomial interpolation problem is solvable with unique solution; this result is called the unisolvence theorem.<br /> \
They are thus useful in polynomial interpolation, since solving the system of linear equations Vu = y for u with V an m × n Vandermonde matrix is equivalent to  \
finding the coefficients uj of the polynomial(s).<br /><br /> \
Source: <a href=\"http://en.wikipedia.org/wiki/Vandermonde_matrix\">Wikipedia</a>"


namespace Graphics {

Starter::Starter() : QDialog() {
	main_layout = new QVBoxLayout(this);

	setWindowTitle("Sensitive UI - Starter");

	selected = LAGRANGE;
	setResult(LAGRANGE);

	presentation = new QLabel("Select interpolation form");
	main_layout->addWidget(presentation, Qt::AlignCenter);

	selectors_layout = new QGridLayout();

	newton_image = new SLabel();
	newton_image->setPixmap(QPixmap(":/portraits/Newton"));
	newton_image->setScaledContents(true);
	newton_image->resize(200, 200);
	selectors_layout->addWidget(newton_image, 0, 0);

	vandermonde_image = new SLabel();
	vandermonde_image->setPixmap(QPixmap(":/portraits/Vandermonde"));
	vandermonde_image->setScaledContents(true);
	vandermonde_image->resize(200, 200);
	selectors_layout->addWidget(vandermonde_image, 0, 1);

	lagrange_image = new SLabel();
	lagrange_image->setPixmap(QPixmap(":/portraits/LaGrange"));
	lagrange_image->setScaledContents(true);
	lagrange_image->resize(200, 200);
	selectors_layout->addWidget(lagrange_image, 0, 2);

	selectors_layout->addWidget(new QLabel("<b>Newton</b> Form"), 1, 0);
	selectors_layout->addWidget(new QLabel("<b>Vandermonde's Matrix"), 1, 1);
	selectors_layout->addWidget(new QLabel("<b>Lagrange</b> Form"), 1, 2);

	main_layout->addLayout(selectors_layout);

	description = new QTextEdit("Select an interpolation form or click <b>Accept</b> to use the default one (Lagrange's).");
	description->setReadOnly(true);

	main_layout->addWidget(description);

	accept_button = new QPushButton("Accept");

	main_layout->addWidget(accept_button, Qt::AlignCenter);

	animation = new QPropertyAnimation();

	setFixedSize(main_layout->sizeHint());

	QObject::connect(lagrange_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::connect(newton_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::connect(vandermonde_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));

	QObject::connect(animation, SIGNAL(finished()), this, SLOT(RestoreAnimations()));

	QObject::connect(accept_button, SIGNAL(clicked()), this, SLOT(AcceptClicked()));
}

Starter::~Starter() {

}


void Starter::Animate(QWidget* target) {
	animation->setTargetObject(target);
	animation->setPropertyName("size");
	animation->setDuration(1000);

	animation->setStartValue(target->size());
	animation->setEndValue(target->size());
	animation->setKeyValueAt(0.2, QSize(target->size().width(), 0));

	animation->setEasingCurve(QEasingCurve::OutBounce);

	if (target == newton_image) {
		selected = NEWTON;
		description->setText(NEWTON_DESCRIPTION);
	}
	else if (target == vandermonde_image) {
		selected = VANDERMONDE;
		description->setText(VANDERMONDE_DESCRIPTION);
	}
	else {
		selected = LAGRANGE;
		description->setText(LAGRANGE_DESCRIPTION);
	}

	setResult(selected);

	QObject::disconnect(lagrange_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::disconnect(newton_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::disconnect(vandermonde_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));

	animation->start();
}

void Starter::RestoreAnimations() {
	QObject::connect(lagrange_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::connect(newton_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::connect(vandermonde_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
}

void Starter::AcceptClicked() {
	done(selected);
}

}
