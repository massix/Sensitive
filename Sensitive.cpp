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
#include <Renal/VandermondeCalculator.h>

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
	VandermondeCalculator calc;

	calc.InsertCoords(0, 3);
	calc.InsertCoords(1, 4);
	calc.InsertCoords(2, 7);
	calc.InsertCoords(3, 12);
	calc.InsertCoords(4, 19);

	calc.BuildFunction();

	return 0;
#endif
}
