/*
 * NextCalculator.h
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#ifndef NEXTCALCULATOR_H_
#define NEXTCALCULATOR_H_

#include <vector>
#include <string>

namespace Renal {

	class NextCalculator {
		public:
			NextCalculator();
			virtual ~NextCalculator();

			bool					InsertCoords(double x, double y);
			bool					InsertCoords(std::pair<double, double> & coords);
			virtual bool			BuildFunction() = 0;
			size_t					CountCoords();
			std::vector<double>		GetPolynom();
			void					Clear();

		protected:
			std::vector<std::pair<double, double> > 	*coords;
			std::vector<double> 						*coeffs;

			bool	_InsertCoords(double x, double y);
	};

}

#endif /* NEXTCALCULATOR_H_ */
