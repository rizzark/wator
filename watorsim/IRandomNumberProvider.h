#pragma once

#include "IObject.h"

namespace wator
{
	struct IRandomNumberProvider : public IObject
	{
		virtual unsigned Get(const unsigned maxValue) = 0;
	};
}
