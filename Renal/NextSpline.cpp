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

#include "NextSpline.h"
#include "NextException.h"

namespace Renal {

using namespace std;

NextSpline::~NextSpline() {
	if (result != 0)
		delete(result);
	delete(base);
}

bool NextSpline::BuildFunction() {
	if(coords->size() < 10)
		throw NextException("I don't have enough coordinates to build a spline");

	SplineBase::Debug(1);
	Spline::Debug(1);

	/* The SplineBase constructor takes an array of values */
	x = new double[coords->size()];
	y = new double[coords->size()];
	int i = 0;
	for(PVector::iterator ite = coords->begin();ite != coords->end();++ite, i++){
		x[i] = ite->first;
		y[i] = ite->second;
	}

	/* Build the SplineBase with wavelength 1/10 of coordinates or 2.0 */
	double wl = coords->size() * 0.1;
	base = new SplineBase(x, coords->size(), (wl < 2.0? 2.0 : wl));
	if(!base->ok())
		throw NextException("Couldn't build the SplineBase class");

	/* Let's build the spline */
	result = base->apply(y);
	if (!result->ok())
		throw NextException("Couldn't retrieve the Spline curve for the given set of coordinates");

	/* Take some coefficients */
	for (int i = 0; i < 4; i++)
		coeffs->push_back(result->coefficient(i));

	return (base->ok() && result->ok());
}

double NextSpline::CalculateInPoint(double x) {
	if (result != 0) {
		if (result->ok())
			return result->evaluate(x);
		else
			return 0;
	}

	return 0;
}

/* namespace Renal */
}

