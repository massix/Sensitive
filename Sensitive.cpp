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
		generic_calculator->BuildFunction();
	}

	catch (Renal::NextException &e) {
		e.PrintMessage();
	}

	delete(generic_calculator);

	return 0;
}
