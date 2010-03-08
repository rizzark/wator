
#include "stdafx.h"
#include "wndwator.h"
#include "watorsim.h"

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int WINAPI WinMain(HINSTANCE hinst,
				   HINSTANCE hinstPrev,
				   LPSTR	 lpCmdLine,
				   int		 nCmdShow)
{
	tklib::SetDbgFlag(_CRTDBG_LEAK_CHECK_DF,/*_CRTDBG_CHECK_CRT_DF|*/_CRTDBG_DELAY_FREE_MEM_DF);

	int				   iReturn		= 0;
	WatorSim		   ws;
	WndWator		   wnd(ws);
	static const DWORD dwStyle  = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_THICKFRAME;

	if(!WndWator::RegisterClass(hinst))
		tklib::TraceError(GetDesktopWindow(),__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot register window class!");
	else if(!wnd.CreateEx(WndWator::CLASSNAME,"WaTor",dwStyle,WS_EX_CONTROLPARENT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hinst,NULL))
		tklib::TraceError(GetDesktopWindow(),__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create window!");
	else
	{
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
	}
	


	return iReturn;
} // end - WinMain


/*
int main(int argc,
		 char *argv[])
{
	int			   iReturn		= 0;
	const unsigned width		= 40;
	const unsigned height		= 40;
	const unsigned uFishBreed	= 5;
	const unsigned uSharkBreed  = 7;
	const unsigned uSharkStarve = 4;
	WatorSim	   ws(width,height,uFishBreed,uSharkBreed,uSharkStarve);
	const unsigned uFishCount   = 200;
	const unsigned uSharkCount  = 40;
	char		   c			= 0;
	FILE		   *pfOut		= fopen("c:\\wator.csv","wb");
	char		   pcData[40][40];
	unsigned	   u = 0;


	srand((unsigned)time(NULL));
	ws.Start(uFishCount,uSharkCount);

	do
	{
		ws.Get((char*)pcData,sizeof(pcData));

		COORD co;

		co.X = 0;
		co.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),co);
		for(u=0; u<height; u++)
		{
			printf("%.40s\r\n",pcData[u]);
		}




		printf("\r\n");
		printf("Fishes: %8u\r\n",ws.GetFishCount());
		printf("Sharks: %8u\r\n",ws.GetSharkCount());


		if(pfOut)
			fprintf(pfOut,"%u,%u\r\n",ws.GetFishCount(),ws.GetSharkCount());

		Sleep(400);
	

		ws.Step();

		//fflush(stdin);
		//c = _getch();
		c = _kbhit();
	}
	//while(c!=27);
	while(c==0);

	fclose(pfOut); pfOut=NULL;

	return iReturn;
} // end - WinMain

*/