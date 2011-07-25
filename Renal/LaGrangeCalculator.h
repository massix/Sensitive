/*
 * LaGrangeCalculator.h
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#ifndef LAGRANGECALCULATOR_H_
#define LAGRANGECALCULATOR_H_

#include "NextCalculator.h"

#include <map>
#include <string>

namespace Renal {

	class LaGrangeCalculator: public Renal::NextCalculator {
		public:
			LaGrangeCalculator() : Renal::NextCalculator() {};

			bool	BuildFunction();

		private:
	};

}

#endif /* LAGRANGECALCULATOR_H_ */
