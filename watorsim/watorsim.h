#pragma once

#include <string>

#include "ISimulation.h"
#include "IRandomNumberProvider.h"

namespace wator
{
	IRandomNumberProvider* CreateRandomProvider(const std::string& name);

	ISimulation* CreateSimulation(const std::string& name, IRandomNumberProvider& rnd);
}

