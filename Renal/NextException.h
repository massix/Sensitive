/*
 * NextException.h
 *
 *  Created on: 25/lug/2011
 *      Author: Massi
 */

#ifndef NEXTEXCEPTION_H_
#define NEXTEXCEPTION_H_

#include <string>
#include <exception>

namespace Renal {

	class NextException : public std::exception {
		public:
			NextException(const char *message);
			virtual ~NextException() throw();

			std::string		*GetMessage();
			void			PrintMessage();

		private:
			std::string		*message;
	};

}

#endif /* NEXTEXCEPTION_H_ */
