/*
 * NextCalculator.cpp
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#include "NextCalculator.h"
#include "NextException.h"

#include <vector>

namespace Renal {

	NextCalculator::NextCalculator() {
		coords = new std::vector<std::pair<double, double> >();
		coeffs = new std::vector<double>();
	}

	NextCalculator::~NextCalculator() {
		delete(coords);
		delete(coeffs);
	}

	bool NextCalculator::InsertCoords(double x, double y) {
		return _InsertCoords(x, y);
	}

	bool NextCalculator::InsertCoords(std::pair<double, double> & coords) {
		return _InsertCoords(coords.first, coords.second);
	}

	size_t NextCalculator::CountCoords() {
		return this->coords->size();
	}

	bool NextCalculator::_InsertCoords(double x, double y) {
		size_t old_size = coords->size();
		coords->push_back(std::pair<double, double>(x, y));

		return (coords->size() > old_size);
	}

	std::vector<double> NextCalculator::GetPolynom() {
		return std::vector<double>(*coeffs);
	}

	void NextCalculator::Clear() {
		coords->clear();
		coeffs->clear();
	}
}
