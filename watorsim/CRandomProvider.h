#pragma once

#include <stdlib.h>

#include "IRandomNumberProvider.h"

namespace wator
{

	class CRandomProvider : public IRandomNumberProvider
	{
	public:
		inline CRandomProvider()
		{
		}

		virtual inline ~CRandomProvider()
		{
		}

		virtual inline void Delete()
		{
			delete this;
		} // end - Delete


		virtual inline unsigned Get(const unsigned maxValue)
		{
			return rand() % (maxValue + 1);
		} // end - Get
	};
}
