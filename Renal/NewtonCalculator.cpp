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

#include "NewtonCalculator.h"

using namespace std;

#include <cstdio>

#include <vector>
#include "NextException.h"

#ifdef DEBUG
#	define DPRINTF(x...) fprintf(stdout, x)
#else
#	define DPRINTF(x...)
#endif

namespace Renal {

bool NewtonCalculator::BuildFunction() {
	vector<double> coords_x, coords_y;
	vector<double> tmp_divert[2];
	vector<double> divert;

	/* Initialize the coeffs' vector to all 0s */
	for (int i = 0; i < coords->size(); i++)
		coeffs->push_back(0.);

	/* Almost useless, makes the debugging easier */
	for (vector<pair<double, double> >::iterator ite = coords->begin(); ite != coords->end(); ++ite) {
		coords_x.push_back(ite->first);
		coords_y.push_back(ite->second);
		tmp_divert[0].push_back(ite->second);
	}

	divert.push_back(tmp_divert[0].data()[0]);

	if (coords_x.size() != coords_y.size())
		throw NextException("What exactly happened, here? o_O");

	/* Calculate the divided differences, using two vectors for the calculations */
	int skip = 1;
	int used_vector = 0;
	while (divert.size() < coords->size()) {

		/* 'skip' holds how many "jumps" we must do in the x vector to find the divisor */
		for (int i = 1; i < tmp_divert[used_vector].size(); i++)
			tmp_divert[1 - used_vector].push_back(
					(tmp_divert[used_vector].data()[i] - tmp_divert[used_vector].data()[i - 1]) /
					(coords_x.data()[(i-1) + skip] - coords_x.data()[i-1])
			);

		/* after that, the divided difference we want, is stored as the first element of the vector */
		divert.push_back(tmp_divert[1-used_vector].data()[0]);

		/* we can now clear the "old" vector, because we'll use it again */
		tmp_divert[used_vector].clear();

		used_vector++;
		if (used_vector == 2)
			used_vector = 0;

		skip++;
	}

#ifdef DEBUG
	for (vector<double>::iterator ite = divert.begin(); ite != divert.end(); ++ite)
		DPRINTF("%.5f ", *ite);
	DPRINTF("\n");
#endif

	try {
		/* Now we're building the polynom */
		vector<double>* polynom = new vector<double>();

		coeffs->data()[coeffs->size()-1] += divert.data()[0] + (divert.data()[1] * (coords_x.data()[0] * -1));
		coeffs->data()[coeffs->size()-2] += divert.data()[1];

		for (int i = 1; i < coords_x.size()-1; i++) {

			/* If the divided difference is 0, is quite useless to do further calculations */
			if (divert.data()[i+1] == 0)
				continue;

			polynom->clear();
			polynom->push_back(1);
			polynom->push_back(coords_x.data()[i] * -1);

			DPRINTF("%.f (x - %.f) ", divert.data()[i+1], coords_x.data()[i]);

			for (int k = i-1; k >= 0; k--) {
				vector<double> m_vec;
				m_vec.push_back(divert.data()[i+1]);
				m_vec.push_back((coords_x.data()[k] * divert.data()[i+1]) * -1);

				polynom = MultiplyVectors(*polynom, m_vec);

				DPRINTF("(x %.f) ", m_vec.data()[1]);
			}

			DPRINTF("=");

			/* Fill the empty polynom positions with 0s */
			while (polynom->size() < coeffs->size())
				polynom->insert(polynom->begin(), 0.);

			/* Insert the Coeffs */
			for (int j = 0; j < polynom->size(); j++) {
				DPRINTF(" %.f", polynom->data()[j]);
				coeffs->data()[j] += polynom->data()[j];
			}

			DPRINTF("\n");
		}

		delete(polynom);
	}
	catch (NextException & e) {
		return false;
	}

#ifdef DEBUG
	DPRINTF("COEFF\n");

	for (vector<double>::iterator ite = coeffs->begin(); ite != coeffs->end(); ++ite)
		DPRINTF("%.f ", *ite);


	DPRINTF("\n");
#endif

	return true;
}

NewtonCalculator::~NewtonCalculator() {

}

}
