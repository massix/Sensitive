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

#include "LaGrangeCalculator.h"
#include "NextException.h"

#include <vector>
#include <iterator>
#include <map>
#include <string>
#include <cmath>

namespace Renal {

	bool LaGrangeCalculator::BuildFunction() {
		std::vector<std::pair<double, double> >::iterator ite = coords->begin();

		this->coeffs->clear();

		for (int i = 0; i < coords->size(); i++)
			this->coeffs->push_back(0);

		/* Calculate partial Lagrange's elementary polynoms */
		for (ite = coords->begin(); ite != coords->end(); ++ite) {
			std::vector<std::pair<double, double> >::iterator temp_iterator = coords->begin();
			std::vector<double> x_points;
			std::vector<double>::iterator x_points_iterator;

			double pivot = ite->first; /* x */
			double multiplier = ite->second; /* y */
			double fractor = 1;

			/* x_points will contain the abscissas of the sinusoidal Lagrange's polynom */
			while (temp_iterator != coords->end()) {
				if (temp_iterator != ite)
					x_points.push_back(temp_iterator->first);
				temp_iterator++;
			}

			/* calculate the lower part of the Lagrange's polynom */
			for (x_points_iterator = x_points.begin(); x_points_iterator != x_points.end(); ++x_points_iterator)
				fractor = fractor * (pivot - *x_points_iterator);


			/* Here the magic happens */
			std::vector<std::vector<double> > *container = new std::vector<std::vector<double> >();

			for (int i = 0; i < x_points.size(); i++) {
				std::vector<double> points;
				points.push_back(1);
				points.push_back(x_points.data()[i] * -1);

				container->push_back(points);
			}

			std::vector<double> *polynom = new std::vector<double>();

			try {
				for (std::vector<std::vector<double> >::iterator ite = container->begin(); ite != container->end(); ++ite)
					polynom = MultiplyVectors(*polynom, *ite);
				delete(container);
			}

			catch (NextException & e) {
				e.PrintMessage();
				delete(container);
				return false;
			}

			int exponent = polynom->size() - 1;
			int i = 0;
			for (std::vector<double>::iterator p_iter = polynom->begin(); p_iter != polynom->end(); ++p_iter, --exponent, ++i) {
				*p_iter = *p_iter * (multiplier / fractor);

				coeffs->data()[i] += *p_iter;			}

			delete(polynom);
		}

		return true;
	}


	std::vector<double>* LaGrangeCalculator::MultiplyVectors(std::vector<double> & first, std::vector<double> & second) {
		std::vector<double> coeffs_tmp;
		std::vector<double> *ret;

		if (first.size() < 1) {
			ret = new std::vector<double>(second);
			return ret;
		}

		ret = new std::vector<double>();

		if (second.size() != 2)
			throw NextException("Second vector's size is not 2.");

		for (std::vector<double>::iterator first_ite = first.begin(); first_ite != first.end(); ++first_ite)
			for (std::vector<double>::iterator second_ite = second.begin(); second_ite != second.end(); ++second_ite)
				coeffs_tmp.push_back(*first_ite * *second_ite);

		ret->push_back(coeffs_tmp.data()[0]);

		double tmp = 0;
		for (int i = 1; i < coeffs_tmp.size()-1; i++) {
			tmp += coeffs_tmp.data()[i];

			if (i % 2 == 0) {
				ret->push_back(tmp);
				tmp = 0;
			}
		}

		ret->push_back(coeffs_tmp.data()[coeffs_tmp.size()-1]);

		return ret;
	}
}
