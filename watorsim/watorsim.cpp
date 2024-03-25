#include "watorsim.h"
#include "sim1.h"
#include "CRandomProvider.h"


namespace wator
{
	IRandomNumberProvider* CreateRandomProvider(const std::string& name)
	{
		if (name == "c" || name == "C")
		{
			return new CRandomProvider();
		}

		return NULL;
	} // end - CreateRandomProvider


	ISimulation* CreateSimulation(const std::string& name, IRandomNumberProvider& rnd)
	{
		if (name == "sim1")
			return new Sim1(rnd);

		return NULL;
	} // end - CreateSimulation
}
