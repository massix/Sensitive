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

			for (x_points_iterator = x_points.begin(); x_points_iterator != x_points.end(); ++x_points_iterator)
				fractor = fractor * (pivot - *x_points_iterator);

			DPRINTF("Pivoting with %0.f, fractor is %0.f, multiplier is %0.f\n", pivot, fractor, multiplier);

		}

		return true;
	}
}
