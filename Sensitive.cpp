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

#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/LaGrangeCalculator.h>
#include <Renal/NewtonCalculator.h>

#include <Graphics/MainApplication.h>
#include <Graphics/MainUI.h>

#include <SensitiveProtocol/SensitiveServer.h>
#include <SensitiveProtocol/SensitiveClient.h>

using namespace Renal;
using namespace std;

int main(int argc, char *argv[]) {
#if 1

	Q_INIT_RESOURCE(Sensitive);

	Graphics::MainApplication run(argc, argv);

#if 0
	Protocol::SensitiveServer server;
	Protocol::SensitiveClient client;

	server.StartServer();
	std::vector<std::pair<double,double> > vector;
	vector.push_back(std::pair<double, double>(0, 0));
	vector.push_back(std::pair<double, double>(1, 1));

	client.SetCoordinates(vector);
	client.InitiateProtocol();
#endif

	run.setApplicationName("Sensitive");

	run.setAttribute(Qt::AA_ImmediateWidgetCreation, true);

	return run.exec();
#else
	NextCalculator *calculator = new NewtonCalculator(3);

	calculator->Clear();

	calculator->InsertCoords(0, 1);
	calculator->InsertCoords(4, 5);
	calculator->InsertCoords(9, 10);
	calculator->InsertCoords(10, 11);
	calculator->InsertCoords(14, 15);
	calculator->InsertCoords(19, 20);
	calculator->BuildFunction();

	return 0;
#endif
}
