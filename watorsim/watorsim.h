#pragma once

#include <string>

#include "ISimulation.h"

namespace wator
{
	ISimulation* CreateSimulation(const std::string& name);
}

