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


int main()
{
	Renal::NextCalculator *generic_calculator = new Renal::LaGrangeCalculator();

	generic_calculator->InsertCoords(1., 2.);
	generic_calculator->InsertCoords(2., 3.);
	generic_calculator->InsertCoords(3., 4.);

	try {
		if (generic_calculator->BuildFunction()) {
			std::vector<double> polynom = generic_calculator->GetPolynom();

			printf("\t\tf(x) = ");
			for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
				printf("%1.f ", *ite);
			printf("\n\n\n\n"); /* 0 1 1 = x + 1 */
		}
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}

	generic_calculator->Clear();

	generic_calculator->InsertCoords(1., 7.);
	generic_calculator->InsertCoords(3., 31.);
	generic_calculator->InsertCoords(5., 71.);

	try {
		if (generic_calculator->BuildFunction()) {
			std::vector<double> polynom = generic_calculator->GetPolynom();

			printf("\t\tf(x) = ");
			for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
				printf("%1.f ", *ite);
			printf("\n\n\n\n"); /* 2 4 1 = 2x^2 + 4x + 1 */
		}
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}


	generic_calculator->Clear();

	generic_calculator->InsertCoords(2., 1.);
	generic_calculator->InsertCoords(4., 2.);
	generic_calculator->InsertCoords(6., 3.);

	try {
		if (generic_calculator->BuildFunction()) {
			std::vector<double> polynom = generic_calculator->GetPolynom();

			printf("\t\tf(x) = ");
			for (std::vector<double>::iterator ite = polynom.begin(); ite != polynom.end(); ++ite)
				printf("%1.2f ", *ite);
			printf("\n\n\n\n"); /* 0 0.5 0 = x/2 */
		}
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}

	delete(generic_calculator);

	return 0;
}
