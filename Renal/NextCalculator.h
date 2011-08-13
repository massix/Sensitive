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

#ifndef NEXTCALCULATOR_H_
#define NEXTCALCULATOR_H_

#include <vector>
#include <string>

#include <BSpline.h>

/* Useful typedefs */
typedef BSpline<double> Spline;
typedef BSplineBase<double> SplineBase;
typedef std::pair<double, double> Pair;
typedef std::vector<double> DVector;
typedef std::vector<Pair> PVector;

using namespace std;

namespace Renal {

class NextCalculator {
public:
	NextCalculator(int precision = 5);
	virtual ~NextCalculator();

	bool				InsertCoords(double x, double y);
	bool				InsertCoords(Pair & coords);
	virtual bool		BuildFunction() = 0;
	size_t				CountCoords();
	DVector*			GetPolynom();
	void				Clear();
	virtual double	CalculateInPoint(double x);
	void				SetPrecision(int precision);
	int					GetPrecision();
	float				GetMaxError();
	double				Express10();

protected:
	PVector 	*coords;
	DVector 	*coeffs;
	int	precision;

	DVector*	MultiplyVectors(DVector & first, DVector & second);

private:
	bool	_InsertCoords(double x, double y);
};

}

#endif /* NEXTCALCULATOR_H_ */
