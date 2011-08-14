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

#include <QtCore>

#include <Graphics/MainApplication.h>
#include <Graphics/MainUI.h>

#include <SensitiveProtocol/SensitiveServer.h>
#include <SensitiveProtocol/SensitiveClient.h>

#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/NextMatrix.h>

#include <iostream>

using namespace Renal;
using namespace std;

int main(int argc, char *argv[]) {
#if 0
	Q_INIT_RESOURCE(Sensitive);

	Graphics::MainApplication run(argc, argv);

	run.setAttribute(Qt::AA_ImmediateWidgetCreation, true);

	return run.exec();
#else
	NextMatrix matrix(3);
	matrix.set_value(1, 0, 0);
	matrix.set_value(2, 0, 1);
	matrix.set_value(3, 0, 2);

	matrix.set_value(0, 1, 0);
	matrix.set_value(1, 1, 1);
	matrix.set_value(2, 1, 2);

	matrix.set_value(0, 2, 0);
	matrix.set_value(0, 2, 1);
	matrix.set_value(3, 2, 2);

	if (matrix.is_triangular())
		std::cout << "Test OK" << std::endl;

	else
		std::cout << "Test NOT OK" << std::endl;

	return 0;
#endif
}
