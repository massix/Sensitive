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

#include <exception>
#include <QUiLoader>


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
	ui_file = new QFile(":/ui/starter");
	if (!ui_file->open(QIODevice::ReadOnly))
		throw std::exception();

	setWindowIcon(QIcon(":/bundle/icon.svg"));
	setWindowTitle("Sensitive UI - Starter");

	/* Load the ui_file */
	main_layout = new QVBoxLayout(this);

	loader = new QUiLoader();
	loaded_widget = loader->load(ui_file);

	main_layout->addWidget(loaded_widget);

	setResult(LAGRANGE);
	selected = LAGRANGE;


	description = loaded_widget->findChild<QTextEdit*>("description");
	description->setText(LAGRANGE_DESCRIPTION);

	/* Connect signals */
	QDialogButtonBox *box = loaded_widget->findChild<QDialogButtonBox*>("buttonBox");
	connect(box, SIGNAL(accepted()), this, SLOT(Accept()));
	connect(box, SIGNAL(rejected()), this, SLOT(reject()));

	buttons_list 	<< 	"radio_lagrange"
					<<	"radio_newton"
					<<	"radio_vandermonde";

	for (int i = 0; i < buttons_list.size(); i++) {
		QRadioButton *button = loaded_widget->findChild<QRadioButton*>(buttons_list.at(i));
		connect(button, SIGNAL(clicked()), this, SLOT(RadioClicked()));
	}

}

void Starter::Accept() {
	done(selected);
}

void Starter::RadioClicked() {
	QRadioButton *radio_lagrange = loaded_widget->findChild<QRadioButton*>("radio_lagrange");
	QRadioButton *radio_newton = loaded_widget->findChild<QRadioButton*>("radio_newton");
	QRadioButton *radio_vandermonde = loaded_widget->findChild<QRadioButton*>("radio_vandermonde");

	if (radio_lagrange->isChecked()) {
		selected = LAGRANGE;
		description->setText(LAGRANGE_DESCRIPTION);
	}

	else if (radio_newton->isChecked()) {
		selected = NEWTON;
		description->setText(NEWTON_DESCRIPTION);
	}

	else {
		selected = VANDERMONDE;
		description->setText(VANDERMONDE_DESCRIPTION);
	}

	setResult(selected);
}

Starter::~Starter() {

}

}
