/*
 * Starter.cpp
 *
 *  Created on: 01/ago/2011
 *      Author: Massi
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

	lagrange_image = new SLabel();
	lagrange_image->setPixmap(QPixmap(":/portraits/LaGrange"));
	lagrange_image->setScaledContents(true);
	lagrange_image->resize(200, 200);
	selectors_layout->addWidget(lagrange_image, 0, 1);

	selectors_layout->addWidget(new QLabel("<b>Newton</b> Form"), 1, 0);
	selectors_layout->addWidget(new QLabel("<b>Lagrange</b> Form"), 1, 1);

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
	else {
		selected = LAGRANGE;
		description->setText(LAGRANGE_DESCRIPTION);
	}

	setResult(selected);

	QObject::disconnect(lagrange_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::disconnect(newton_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));

	animation->start();
}

void Starter::RestoreAnimations() {
	QObject::connect(lagrange_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
	QObject::connect(newton_image, SIGNAL(clicked(QWidget*)), this, SLOT(Animate(QWidget*)));
}

void Starter::AcceptClicked() {
	done(selected);
}

}
