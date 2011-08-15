/*
 * Starter.h
 *
 *  Created on: 01/ago/2011
 *      Author: Massi
 */

#ifndef STARTER_H_
#define STARTER_H_

#include "SLabel.h"

#include <QtGui>
#include <QtCore>

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
	QVBoxLayout *main_layout;
	QGridLayout *selectors_layout;

	QLabel		*presentation;
	SLabel		*newton_image;
	SLabel		*lagrange_image;
	SLabel		*vandermonde_image;

	QTextEdit	*description;

	QPushButton	*accept_button;

	InterpolationForm selected;

	QPropertyAnimation		*animation;

private slots:
	void		Animate(QWidget *target);
	void		RestoreAnimations();
	void		AcceptClicked();
};

}

#endif /* STARTER_H_ */
