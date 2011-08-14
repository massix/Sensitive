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

#include "NextMatrix.h"
#include "NextException.h"

Renal::NextMatrix::NextMatrix(int order)
	: order(order), rows(order), cols(order)
{
	inner_matrix = new double*[order];

	for (int i = 0; i < order; i++)
		inner_matrix[i] = new double[order];

}



Renal::NextMatrix::~NextMatrix()
{
	for (int i = 0; i < order; i++)
		delete inner_matrix[i];

	delete inner_matrix;
}



double Renal::NextMatrix::determinant()
{
	if (rows != cols)
		throw NextException("Matrix is not square");

	/* Basics */
	if (order == 1)
		return operator()(0, 0);

	else if (order == 2)
		return ((operator()(0, 0) * operator()(1, 1)) - (operator()(1, 0) * operator()(0, 1)));

	else
		throw NextException("Not implemented yet");
}



double Renal::NextMatrix::operator ()(const int & row, const int & col)
{
	if ((row > rows) || (col > cols))
		throw NextException("Out of bounds");

	return inner_matrix[row][col];
}



double* Renal::NextMatrix::operator [](const int & row)
{
	if (row > rows)
		throw NextException("Out of bounds");

	double* ret = new double[order];

	for (int i = 0; i < order; i++)
		ret[i] = inner_matrix[row][i];

	return ret;
}



bool Renal::NextMatrix::is_square()
{
	return (rows == cols);
}

bool Renal::NextMatrix::set_value(double val, int row, int col)
{
	if (row >= order)
		return false;

	else if (col >= order)
		return false;

	inner_matrix[row][col] = val;

	return true;
}

int Renal::NextMatrix::get_order()
{
	return order;
}


int Renal::NextMatrix::get_rows()
{
	return rows;
}

bool Renal::NextMatrix::do_gauss_eliminations()
{
	return false;
}

bool Renal::NextMatrix::is_triangular()
{
	if (order == 1)
		return true;

	for (int i = 0; i < get_rows(); i++)
		for (int k = 0; k < i; k++)
			if (inner_matrix[i][k] != 0)
				return false;

	return true;
}

int Renal::NextMatrix::get_cols()
{
	return cols;
}
