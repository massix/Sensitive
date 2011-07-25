/*
 * LaGrangeCalculator.cpp
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#include "LaGrangeCalculator.h"
#include "NextException.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <string>

#ifdef	DEBUG
#define		DPRINTF(x...) fprintf(stdout, x);
#else
#define		DPRINTF(x...)
#endif

namespace Renal {

	bool LaGrangeCalculator::BuildFunction() {
		std::map<std::string, double> temp_map;

		std::vector<std::pair<double, double> >::iterator ite = coords->begin();

		/* Cascade style switch, I love it. */
		switch(coords->size()) {
		case 0:
		case 1:
			throw NextException("Too few interpolation points.");
			break;
		case 4:
			temp_map.insert(std::pair<std::string, double>("x3", 0.));
		case 3:
			temp_map.insert(std::pair<std::string, double>("x2", 0.));
		case 2:
			temp_map.insert(std::pair<std::string, double>("x1", 0.));
			temp_map.insert(std::pair<std::string, double>("x0", 0.));
			break;
		default:
			throw NextException("Too many interpolation points.");
		}

		DPRINTF("Interpolation points: %d\n", temp_map.size());

		/* Calculate partial Lagrange's elementary polynoms */
		for (ite = coords->begin(); ite != coords->end(); ++ite) {
			std::vector<std::pair<double, double> >::iterator temp_iterator = coords->begin();
			std::vector<double> x_points;
			std::vector<double>::iterator x_points_iterator;

			double pivot = ite->first; /* x */
			double multiplier = ite->second; /* y */
			double fractor = 1;

			while (temp_iterator != coords->end()) {
				if (temp_iterator != ite)
					x_points.push_back(temp_iterator->first);
				temp_iterator++;
			}

			for (x_points_iterator = x_points.begin(); x_points_iterator != x_points.end(); ++x_points_iterator) {
				DPRINTF("(x - %0.f) ", *x_points_iterator);
				fractor = fractor * (pivot - *x_points_iterator);
			}

			DPRINTF("= ");

			x_points_iterator = x_points.begin();

			std::vector<double> first;
			first.push_back(1);
			first.push_back(x_points.data()[0] * -1);

			std::vector<double> second;
			second.push_back(1);
			second.push_back(x_points.data()[1] * -1);

			std::vector<double> *polynom;

			try {
				polynom = MultiplyVectors(first, second);
			}

			catch (NextException & e) {
				e.PrintMessage();
				return false;
			}

			int exponent = polynom->size() - 1;
			for (std::vector<double>::iterator p_iter = polynom->begin(); p_iter != polynom->end(); ++p_iter, --exponent) {
				*p_iter = *p_iter * (multiplier / fractor);
				if (*p_iter != 0) {

					if (exponent == 1) {
						DPRINTF("%s%0.fx ", *p_iter > 0? "+" : "",*p_iter);
					}

					else if (exponent == 0) {
						DPRINTF("%s%0.f ", *p_iter > 0? "+" : "",*p_iter);
					}

					else {
						DPRINTF("%s%0.fx^%d ", *p_iter > 0? "+" : "",*p_iter, exponent);
					}
				}
			}

			delete(polynom);
			DPRINTF("\nPivoting with %0.f, multiplier is %0.f, fractor is %0.f, polynom's grade is %d\n\n",
						pivot, multiplier, fractor, x_points.size());
		}

		return true;
	}


	std::vector<double>* LaGrangeCalculator::MultiplyVectors(std::vector<double> & first, std::vector<double> & second) {
		std::vector<double> coeffs_tmp;
		std::vector<double> *ret = new std::vector<double>();
		double tmp[4] = {0, 0, 0, 0};

		if (second.size() != 2)
			throw NextException("Second vector's size is not 2.");

		size_t elements = first.size() * second.size();

		for (std::vector<double>::iterator first_ite = first.begin(); first_ite != first.end(); ++first_ite)
			for (std::vector<double>::iterator second_ite = second.begin(); second_ite != second.end(); ++second_ite)
				coeffs_tmp.push_back(*first_ite * *second_ite);

		int i = 0;
		int j = 0;
		for (std::vector<double>::iterator coeffs_ite = coeffs_tmp.begin(); coeffs_ite != coeffs_tmp.end(); ++coeffs_ite) {
			if (coeffs_ite == coeffs_tmp.begin())
				ret->push_back(*coeffs_ite);

			else if (coeffs_ite == coeffs_tmp.end()-1) {
				for (int k = 0; k < 4; k++)
					if (tmp[k] != 0)
						ret->push_back(tmp[k]);
				ret->push_back(*coeffs_ite);
			}

			else {
				if (i++ > 2)
					j++;
				tmp[j] += *coeffs_ite;
			}
		}

		return ret;
	}
}
