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

#include "VandermondeCalculator.h"
#include "NextMatrix.h"
#include "NextException.h"

#include <cmath>
#include <iostream>

bool Renal::VandermondeCalculator::BuildFunction()
{
	if (coords->size() > 10)
		throw NextException("Refusing to use Vandermonde with more than 10 interpolation points.");

	bool skipped = false;

	/* Build the Vandermonde's matrix */
	NextMatrix vandermondes_matrix(coords->size());

	for (int i = 0; i < coords->size(); i++) {
		for (int j = 0; j < coords->size(); j++) {
			vandermondes_matrix.set_value(pow(coords->at(i).first, j), i, j);
		}
	}

	/* Calculate the determinant using Gauss' eliminations and then reset the matrix back to the original values */
	double det = vandermondes_matrix.determinant();
	vandermondes_matrix.reset_matrix();
	vandermondes_matrix.print_matrix();

	std::cout << "Det: " << det << std::endl;

	/* Calculate the coefficients */
	for (int i = 0; i < vandermondes_matrix.get_order(); i++) {
		NextMatrix coeff_matrix(vandermondes_matrix);

		/* Replace the i-th column with the coefficients */
		for (int j = 0; j < coeff_matrix.get_order(); j++)
			coeff_matrix.set_value(coords->at(j).second, j, i);

		coeff_matrix.print_matrix();
		double in_det = coeff_matrix.determinant();
		std::cout << "Det: " << in_det << std::endl;
		std::cout << "Coeff: " << in_det/det << std::endl;
		std::cout << std::endl << std::endl;
	}

	return true;
}

