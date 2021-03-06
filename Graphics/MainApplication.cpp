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

#include "MainApplication.h"
#include "Starter.h"
#include "MainUI.h"

#include <QApplication>
#include <QString>

#include <Renal/LaGrangeCalculator.h>
#include <Renal/NewtonCalculator.h>
#include <Renal/VandermondeCalculator.h>

namespace Graphics {

MainApplication::MainApplication(int argc, char *argv[]) : QApplication(argc, argv) {
//	QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(CleanUp()));
}

MainApplication::~MainApplication() {
	/* This is not called by Windows */
}

int MainApplication::exec() {
	Starter t;

	switch (t.exec()) {
	case QDialog::Rejected:
		return 0;
	case NEWTON:
		main_window = new Graphics::MainUI(new Renal::NewtonCalculator(), QString("Newton's polynomial form"));
		break;
	case LAGRANGE:
		main_window = new Graphics::MainUI(new Renal::LaGrangeCalculator(), QString("Lagrange's polynomial form"));
		break;
	case VANDERMONDE:
		main_window = new Graphics::MainUI(new Renal::VandermondeCalculator(), QString("Vandermonde's polynomial form"));
		break;
	default:
		return 0;
	}

	main_window->show();

	return QApplication::exec();
}

void MainApplication::CleanUp() {
//	delete(main_window);
}

}
