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
