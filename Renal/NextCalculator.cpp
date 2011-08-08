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
#include <cmath>

namespace Renal {

NextCalculator::NextCalculator(int precision) {
	coords = new std::vector<std::pair<double, double> >();
	coeffs = new std::vector<double>();
	this->precision = precision;
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

std::vector<double>* NextCalculator::GetPolynom() {
	return new std::vector<double>(*coeffs);
}

void NextCalculator::Clear() {
	coords->clear();
	coeffs->clear();
}

double NextCalculator::CalculateInPoint(double x) {
	if (coeffs->size() < 1)
		throw NextException("I have no coefficients.");

	double result = 0;
	int exponent = coeffs->size()-1;
	for (std::vector<double>::iterator ite = coeffs->begin(); ite != coeffs->end()-1; ++ite, --exponent) {
		double coeff = round(*ite * Express10()) / Express10();
		result += (double) (coeff * pow(x, exponent));
	}

	result += coeffs->data()[coeffs->size()-1];

	return round(result * Express10()) / Express10();
}

void NextCalculator::SetPrecision(int precision) {
	this->precision = precision;
}

int NextCalculator::GetPrecision() {
	return this->precision;
}

float NextCalculator::GetMaxError() {
	return pow(10, -precision);
}

double NextCalculator::Express10() {
	return double(pow(10, precision));
}

std::vector<double>* NextCalculator::MultiplyVectors(std::vector<double> & first, std::vector<double> & second) {
	std::vector<double> coeffs_tmp;
	std::vector<double> *ret;

	if (first.size() < 1) {
		ret = new std::vector<double>(second);
		return ret;
	}

	if (second.size() < 1) {
		ret = new std::vector<double>(first);
		return ret;
	}

	if (second.size() != 2)
		throw NextException("Second vector's size is not 2.");

	ret = new std::vector<double>();

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
