/*
 * SPixmap.cpp
 *
 *  Created on: 01/ago/2011
 *      Author: Massi
 */

#include "SLabel.h"

namespace Graphics {

void SLabel::mousePressEvent(QMouseEvent *ev) {
	emit clicked(this);
}

SLabel::~SLabel() {

}

}
