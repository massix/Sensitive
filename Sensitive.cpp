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

#include <iostream>
#include <cstdio>


#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/LaGrangeCalculator.h>

Renal::NextCalculator *generic_calculator;

void TestWithCoords(std::vector<std::pair<double, double> > coords_vector) {
	generic_calculator->Clear();

	for (std::vector<std::pair<double, double> >::iterator ite = coords_vector.begin(); ite != coords_vector.end(); ++ite)
		generic_calculator->InsertCoords(*ite);

	try {
		if (generic_calculator->BuildFunction()) {
			std::vector<double> polynom = generic_calculator->GetPolynom();

			printf("\t\tf(x) = ");
			for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
				printf("%.2f ", *ite);
			printf("\n");
		}
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}

}

void PrintOut(double x) {
	double result = generic_calculator->CalculateInPoint(x);

	printf("\t\tf(%.f) = %.f\n\n\n\n", x, result);
}


int main() {
	generic_calculator = new Renal::LaGrangeCalculator();

	std::vector<std::pair<double, double> > coords;

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 2.));
	coords.push_back(std::pair<double, double>(2., 3.));
	coords.push_back(std::pair<double, double>(3., 4.));
	TestWithCoords(coords); /* 0 1 1 = x + 1 */
	PrintOut(8); /* 9 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 7.));
	coords.push_back(std::pair<double, double>(3., 31.));
	coords.push_back(std::pair<double, double>(5., 71.));
	TestWithCoords(coords); /* 2 4 1 = 2x^2 + 4x + 1 */
	PrintOut(7); /* 127 */

	coords.clear();
	coords.push_back(std::pair<double, double>(2., 1.));
	coords.push_back(std::pair<double, double>(4., 2.));
	coords.push_back(std::pair<double, double>(8., 4.));
	TestWithCoords(coords); /* 0 0.50 0 = x/2 */
	PrintOut(14); /* 7 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 3.));
	coords.push_back(std::pair<double, double>(2., 7.));
	coords.push_back(std::pair<double, double>(3., 13.));
	TestWithCoords(coords); /* 1 1 1 = x^2 + x + 1 */
	PrintOut(5); /* 31 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 30.));
	coords.push_back(std::pair<double, double>(3., 296.));
	coords.push_back(std::pair<double, double>(4., 525.));
	TestWithCoords(coords); /* 32 5 -7 = 32x^2 + 5x - 7 */
	PrintOut(2); /* 131 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 2.));
	coords.push_back(std::pair<double, double>(2., 4.));
	coords.push_back(std::pair<double, double>(3., 6.));
	coords.push_back(std::pair<double, double>(4., 8.));
	TestWithCoords(coords); /* 0 0 2 0 = 2x */
	PrintOut(16); /* 32 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 1.));
	coords.push_back(std::pair<double, double>(2., 2.));
	TestWithCoords(coords); /* 1 0 = x */
	PrintOut(9); /* 9 */

	coords.clear();
	coords.push_back(std::pair<double, double>(2., 4.));
	coords.push_back(std::pair<double, double>(4., 8.));
	coords.push_back(std::pair<double, double>(8., 16.));
	coords.push_back(std::pair<double, double>(3., 6.));
	coords.push_back(std::pair<double, double>(5., 10.));
	coords.push_back(std::pair<double, double>(25., 50.));
	coords.push_back(std::pair<double, double>(9., 18.));
	coords.push_back(std::pair<double, double>(11., 22.));
	coords.push_back(std::pair<double, double>(1., 2.));
	coords.push_back(std::pair<double, double>(7., 14.));
	TestWithCoords(coords); /* 0 .. 0 2 0 = 2x */
	PrintOut(15); /* 30 */

	delete(generic_calculator);

	return 0;
}
