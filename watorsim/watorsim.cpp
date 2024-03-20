#include "watorsim.h"
#include "sim1.h"


namespace wator
{
	ISimulation* CreateSimulation(const std::string& name)
	{
		if (name == "sim1")
			return new Sim1();

		return NULL;
	} // end - CreateSimulation
}
