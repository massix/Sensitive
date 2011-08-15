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

#ifndef NEXTMATRIX_H_
#define NEXTMATRIX_H_

namespace Renal {

class NextMatrix {
public:
	NextMatrix(int order);
	virtual ~NextMatrix();

	/* Calculate the determinant */
	double		determinant();

	/* Retrieve a certain value */
	double		operator()(const int& row, const int& col);

	/* Retrieve a certain row */
	/* The returned value must be manually free'd by the caller */
	double*	operator[](const int& row);

	/* Almost obvious, just useful to check if we're doing
	 * mistakes while calculating the determinant.
	 */
	bool		is_square();

	/* Returns true if the matrix is triangular */
	bool		is_triangular();

	/* Set a value */
	bool		set_value(double val, int row, int col);

	/* General informations */
	int			get_order();
	int			get_rows();
	int			get_cols();

	/* Debug */
	void		print_matrix();

private:
	double**	gauss_matrix;
	double**	original_matrix;
	int			order;
	int			rows;
	int			cols;
	bool		transformed;

	void		gauss_helper(int line);

	/* Do Gaussian's eliminations until the matrix doesn't become
	 * a triangular one
	 */
	bool		do_gauss_eliminations();
};

} /* namespace Renal */
#endif /* NEXTMATRIX_H_ */
