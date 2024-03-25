/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#include "stdafx.h"
#include "wndwator.h"


#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


tbase2::windows::Module gl_Module;



int WINAPI WinMain(HINSTANCE hinst,
				   HINSTANCE hinstPrev,
				   LPSTR	 lpCmdLine,
				   int		 nCmdShow)
{
	tbase2::debug::AutoCheckMemoryLeaks();

	int iReturn	= 0;
	try
	{
		auto rnd = std::shared_ptr<wator::IRandomNumberProvider>(wator::CreateRandomProvider("C"), [](wator::IRandomNumberProvider* p) { p->Delete(); });
		auto wator = std::shared_ptr<wator::ISimulation>(wator::CreateSimulation("sim1", *rnd), [](wator::ISimulation* p) { p->Delete(); });


		const std::locale locale(tbase2::unicode::WStringToCodepage(gl_Module.String(IDS_LOCALE).c_str(),CP_ACP,0));
		WndWator		  wnd(*wator,locale);

		MSG msg;
		int i = 0;

		srand(static_cast<unsigned>(time(NULL)));
		while(GetMessage(&msg,NULL,0,0))
		{
			if(!wnd.PreTranslateMsg(msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		iReturn = static_cast<int>(msg.wParam);
	}
	catch(...)
	{
		iReturn = -1;
	}

	return iReturn;
} // end - WinMain

