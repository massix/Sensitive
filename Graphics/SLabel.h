/*
 * SPixmap.h
 *
 *  Created on: 01/ago/2011
 *      Author: Massi
 */

#ifndef SPIXMAP_H_
#define SPIXMAP_H_

#include <QtGui>

namespace Graphics {

class SLabel : public QLabel {

	Q_OBJECT

public:
	SLabel() : QLabel() {};
	virtual ~SLabel();

protected:
	/* Override */
	void mousePressEvent(QMouseEvent *ev);

signals:
	void clicked(QWidget*);
};

}

#endif /* SPIXMAP_H_ */
