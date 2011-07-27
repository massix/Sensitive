/*
 * Sensitive.cpp
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#include <iostream>
#include <cstdio>


#include <Renal/NextCalculator.h>
#include <Renal/NextException.h>
#include <Renal/LaGrangeCalculator.h>

Renal::NextCalculator *generic_calculator;


void TestWithCoords(std::pair<double, double> pair1, std::pair<double, double> pair2, std::pair<double, double> pair3)
{
	generic_calculator->Clear();

	generic_calculator->InsertCoords(pair1);
	generic_calculator->InsertCoords(pair2);
	generic_calculator->InsertCoords(pair3);

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

	TestWithCoords(std::pair<double, double> (1., 2.), std::pair<double, double> (2., 3.), std::pair<double, double> (3., 4.)); /* 0 1 1 = x + 1 */
	TestWithCoords(std::pair<double, double> (1., 7.), std::pair<double, double> (3., 31.), std::pair<double, double> (5., 71.)); /* 2 4 1 = 2x^2 + 4x + 1 */
	TestWithCoords(std::pair<double, double> (2., 1.), std::pair<double, double> (4., 2.), std::pair<double, double> (8., 4.)); /* 0 0.05 0 = x/2 */
	TestWithCoords(std::pair<double, double> (1., 3.), std::pair<double, double> (2., 7.), std::pair<double, double> (3., 13.)); /* 1 1 1 = x^2 + x + 1 */
	TestWithCoords(std::pair<double, double> (1., 30.), std::pair<double, double> (3., 296.), std::pair<double, double> (4., 525.)); /* 32 5 -7 = 32x^2 + 5x - 7 */

	delete(generic_calculator);

	return 0;
}
