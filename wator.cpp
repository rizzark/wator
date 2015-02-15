/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#include "stdafx.h"
#include "wndwator.h"
#include "watorsim.h"


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
		WatorSim ws;
		WndWator wnd(ws);

		MSG msg;
		int i = 0;

		srand((unsigned)time(NULL));

		while(GetMessage(&msg,NULL,0,0))
		{
			if(!wnd.PreTranslateMsg(msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		iReturn = msg.wParam;
	}
	catch(...)
	{
		iReturn = -1;
	}

	return iReturn;
} // end - WinMain

