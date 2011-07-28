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

void TestWithCoords(std::vector<std::pair<double, double> > coords_vector)
{
	generic_calculator->Clear();

	for (std::vector<std::pair<double, double> >::iterator ite = coords_vector.begin(); ite != coords_vector.end(); ++ite)
		generic_calculator->InsertCoords(*ite);

	try {
		if (generic_calculator->BuildFunction()) {
			std::vector<double> polynom = generic_calculator->GetPolynom();

			printf("\t\tf(x) = ");
			for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
				printf("%.2f ", *ite);
			printf("\n\n\n\n"); /* 0 1 1 = x + 1 */
		}
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}

}


int main()
{
	generic_calculator = new Renal::LaGrangeCalculator();

	std::vector<std::pair<double, double> > coords;

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 2.));
	coords.push_back(std::pair<double, double>(2., 3.));
	coords.push_back(std::pair<double, double>(3., 4.));
	TestWithCoords(coords); /* 0 1 1 = x + 1 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 7.));
	coords.push_back(std::pair<double, double>(3., 31.));
	coords.push_back(std::pair<double, double>(5., 71.));
	TestWithCoords(coords); /* 2 4 1 = 2x^2 + 4x + 1 */

	coords.clear();
	coords.push_back(std::pair<double, double>(2., 1.));
	coords.push_back(std::pair<double, double>(4., 2.));
	coords.push_back(std::pair<double, double>(8., 4.));
	TestWithCoords(coords); /* 0 0.50 0 = x/2 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 3.));
	coords.push_back(std::pair<double, double>(2., 7.));
	coords.push_back(std::pair<double, double>(3., 13.));
	TestWithCoords(coords); /* 1 1 1 = x^2 + x + 1 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 30.));
	coords.push_back(std::pair<double, double>(3., 296.));
	coords.push_back(std::pair<double, double>(4., 525.));
	TestWithCoords(coords); /* 32 5 -7 = 32x^2 + 5x - 7 */

	coords.clear();
	coords.push_back(std::pair<double, double>(1., 2.));
	coords.push_back(std::pair<double, double>(2., 4.));
	coords.push_back(std::pair<double, double>(3., 6.));
	coords.push_back(std::pair<double, double>(4., 8.));
	TestWithCoords(coords); /* 0 0 2 0 = 2x */

	delete(generic_calculator);

	return 0;
}
