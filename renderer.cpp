/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#include "stdafx.h"
#include "renderer.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

extern tbase2::windows::Module gl_Module;


const COLORREF Renderer::DEFAULT_COLOR_FISH  = RGB(0,255,0);
const COLORREF Renderer::DEFAULT_COLOR_SHARK = RGB(255,0,0);
const COLORREF Renderer::DEFAULT_COLOR_WATER = RGB(0,0,0);
const COLORREF Renderer::DEFAULT_COLOR_PAPER = RGB(255,255,255);
const COLORREF Renderer::DEFAULT_COLOR_GRAPH = RGB(0,0,0);
const COLORREF Renderer::DEFAULT_COLOR_GRID  = RGB(50,50,50);


Renderer::Renderer(const std::locale &locale) : m_locale(locale),
												m_colorWater(DEFAULT_COLOR_WATER),
											    m_colorFish(DEFAULT_COLOR_FISH),
											    m_colorShark(DEFAULT_COLOR_SHARK),
											    m_colorPaper(DEFAULT_COLOR_PAPER),
											    m_colorGraph(DEFAULT_COLOR_GRAPH),
												m_colorGrid(DEFAULT_COLOR_GRID),
												m_uOfs(0)
{
	memset(m_puFish,0,sizeof(m_puFish));
	memset(m_puShark,0,sizeof(m_puShark));
} // end - Renderer::Renderer


Renderer::~Renderer()
{
} // end - Renderer::~Renderer


void Renderer::SetDefault()
{
	m_colorFish  = DEFAULT_COLOR_FISH;
	m_colorShark = DEFAULT_COLOR_SHARK;
	m_colorWater = DEFAULT_COLOR_WATER;
	m_colorPaper = DEFAULT_COLOR_PAPER;
	m_colorGraph = DEFAULT_COLOR_GRAPH;
	m_colorGrid  = DEFAULT_COLOR_GRID;
} // end - Renderer::SetDefault


void Renderer::Render(WatorSim							  &wator,
					  tbase2::windows::gdi::DeviceContext &dc,
					  const unsigned					  width,
					  const unsigned					  height)
{
	try
	{
		const size_t			   lenData = wator.GetWidth() * wator.GetHeight();
		tbase2::buffer			   bfData(lenData);
		char					   *pcData = reinterpret_cast<char*>(bfData.getBuffer());
		unsigned				   hDisplay = height>300 ? height-100 : height;
		unsigned				   hDiagram = height>300 ? 100 : 0;
		tbase2::windows::gdi::Rect rcDisplay(0,0,width,hDisplay);
		tbase2::windows::gdi::Rect rcDiagram(0,hDisplay+1,width,hDisplay+hDiagram);

		wator.Get(pcData,lenData);
		RenderDisplay(wator,dc,rcDisplay,pcData,lenData);
		if(hDiagram)
			RenderDiagram(wator,dc,rcDiagram,pcData,lenData);
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}
} // end - Renderer::Render


void Renderer::RenderDisplay(WatorSim							 &wator,
							 tbase2::windows::gdi::DeviceContext &dc,
							 const tbase2::windows::gdi::Rect	 &rcDst,
							 const char							 pcData[],
							 const size_t						 lenData)
{
	try
	{
		tbase2::windows::gdi::Brush brWater(m_colorWater);
		tbase2::windows::gdi::Brush brFish(m_colorFish);
		tbase2::windows::gdi::Brush brShark(m_colorShark);
		tbase2::windows::gdi::Pen   pnWater(PS_SOLID,1,m_colorWater);
		tbase2::windows::gdi::Pen   pnFish(PS_SOLID,1,m_colorFish);
		tbase2::windows::gdi::Pen   pnShark(PS_SOLID,1,m_colorShark);
		unsigned					x		= 0;
		unsigned					y	    = 0;

		const unsigned uUnitW = rcDst.width() / wator.GetWidth();
		const unsigned uUnitH = rcDst.height() / wator.GetHeight();
		const unsigned uDeltaX = (rcDst.width() - (uUnitW * wator.GetWidth())) / 2;
		const unsigned uDeltaY = (rcDst.height() - (uUnitH * wator.GetHeight())) / 2;


		x = uDeltaX;
		y = uDeltaY;

		dc.SetPen(pnWater);
		for(unsigned u=0; u<lenData; u++)
		{
			if(pcData[u]==WatorSim::CHAR_FISH)
			{
				dc.SetBrush(brFish);
				dc.SetPen(pnFish);
			}
			else if(pcData[u]==WatorSim::CHAR_SHARK)
			{
				dc.SetBrush(brShark);
				dc.SetPen(pnShark);
			}
			else
			{
				dc.SetBrush(brWater);
				dc.SetPen(pnWater);
			}

			if(uUnitW>4 && uUnitH>4)
				dc.SetPen(pnWater);

			Rectangle(dc,x,y,x+uUnitW,y+uUnitH);
			x += uUnitW;
			if((u+1)%wator.GetWidth()==0)
			{
				x = uDeltaX;
				y += uUnitH;
			}
		}
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}
} // end - Renderer::RenderDisplay


void Renderer::RenderDiagram(WatorSim							 &wator,
							 tbase2::windows::gdi::DeviceContext &dc,
							 const tbase2::windows::gdi::Rect	 &rcDst,
							 const char							 pcData[],
							 const size_t						 lenData)
{
	try
	{
		tbase2::windows::gdi::Brush brPaper(m_colorPaper);
		tbase2::windows::gdi::Pen   pnPaper(PS_SOLID,1,m_colorPaper);

		dc.SetPen(pnPaper);
		dc.SetBrush(brPaper);
		dc.Rectangle(rcDst);

		tbase2::windows::gdi::Rect rcText = RenderText(wator,dc,rcDst);

		const unsigned uWRemain = rcDst.width() - rcText.width();
		if(uWRemain > 200)
		{
			tbase2::windows::gdi::Font fntGraph(dc,6,gl_strDlgFont.c_str());
			const unsigned			   uMaxItems = wator.GetWidth() * wator.GetHeight();

			dc.SetFont(fntGraph);

			const SIZE				   sizMaxLabel = dc.GetTextExtent(tbase2::localize::to_locstring<wchar_t>(uMaxItems,m_locale));
			tbase2::windows::gdi::Pen  pnGraph(PS_SOLID,1,m_colorGraph);
			tbase2::windows::gdi::Pen  pnGrid(PS_DOT,1,m_colorGrid);
			tbase2::windows::gdi::Rect rcGraph;

			rcGraph.left   = rcText.right + 5;
			rcGraph.top    = rcDst.top + 5;
			rcGraph.right  = rcDst.right - 10 - sizMaxLabel.cx;
			rcGraph.bottom = rcDst.bottom - 5;

			// Draw graph frame			
			dc.SetPen(pnGraph);
			dc.Rectangle(rcGraph);

			
			typedef std::vector<std::pair<unsigned,unsigned> > watorhistory;

			const int				     x1 = rcGraph.left  + 1;
			const int				     x2 = rcGraph.right - 1;
			const int				     y1 = rcGraph.top + 1;
			const int				     y2 = rcGraph.bottom - 1;
			tbase2::windows::gdi::Rect   rcDiagram(x1,y1,x2,y2);
			const double			     dYScale    = static_cast<double>(rcGraph.height()) / static_cast<double>(uMaxItems);
			const double			     dYStep		= static_cast<double>(rcGraph.height()) / 5 / dYScale;
			const unsigned			     uValueStep = tbase2::round<unsigned>(dYStep);
			const watorhistory		     &vHistory	= wator.GetHistory();

				
			if(vHistory.size()>0)
			{
				std::vector<POINT> vPtFish;
				std::vector<POINT> vPtShark;

				int x = x2;
				for(watorhistory::const_reverse_iterator it=vHistory.rbegin(); 
					it!=vHistory.rend() && x>x1; it++,x--)
				{
					const unsigned uFishCount  = it->first;
					const unsigned uSharkCount = it->second;

					vPtFish.push_back(tbase2::windows::gdi::Point(x,y2 - tbase2::round<unsigned>(uFishCount * dYScale)));
					vPtShark.push_back(tbase2::windows::gdi::Point(x,y2 - tbase2::round<unsigned>(uSharkCount * dYScale)));
				}


				tbase2::windows::gdi::Pen pnFish(PS_SOLID,2,m_colorFish);
				tbase2::windows::gdi::Pen pnShark(PS_SOLID,2,m_colorShark);

				dc.SetPen(pnFish);
				dc.Lines(vPtFish);
				dc.SetPen(pnShark);
				dc.Lines(vPtShark);
/*
				dc.SetPen(pnShark);
				::PolyBezier(dc,vPtShark.data(),vPtShark.size());
				dc.SetPen(pnFish);
				::PolyBezier(dc,vPtFish.data(),vPtFish.size());
*/
			}
			



			// Draw horizontal helper lines
			dc.SetPen(pnGrid);
			dc.SetTextColor(m_colorGraph);
			dc.SetBkColor(m_colorPaper);
			dc.SetTextAlign(TA_RIGHT | TA_BOTTOM);
			for(unsigned uValue=uValueStep; uValue<uMaxItems; uValue+=uValueStep)
			{
				int y = rcGraph.bottom - tbase2::round<unsigned>((static_cast<double>(uValue) * dYScale));


				MoveToEx(dc,x1,y,NULL);
				LineTo(dc,x2,y);
				dc.TextOut(rcDst.right - 5,y,tbase2::localize::to_locstring<wchar_t>(uValue,m_locale));
			}


			// Draw vertical helper lines
			dc.SetPen(pnGrid);
			for(int x=x2-100; x>x1; x-=100)
			{
				MoveToEx(dc,x,y1,NULL);
				LineTo(dc,x,y2);
			}











		}
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}
} // end - Renderer::RenderDiagram


RECT Renderer::RenderText(WatorSim							  &wator,
						  tbase2::windows::gdi::DeviceContext &dc,
						  const tbase2::windows::gdi::Rect	  &rcDst)
{
	tbase2::windows::gdi::Rect rcText;

	try
	{
		const std::wstring			wstrFish		   = gl_Module.String(IDS_FISHES);
		const std::wstring			wstrShark		   = gl_Module.String(IDS_SHARKS);
		const std::wstring			wstrIterations	   = gl_Module.String(IDS_CYCLES);
		const std::wstring			wstrFishCount	   = tbase2::localize::to_locstring<wchar_t>(wator.GetFishCount(),m_locale);
		const std::wstring			wstrSharkCount	   = tbase2::localize::to_locstring<wchar_t>(wator.GetSharkCount(),m_locale);
		const std::wstring			wstrIterationCount = tbase2::localize::to_locstring<wchar_t>(wator.GetIterations(),m_locale);
		tbase2::windows::gdi::Font	fntText(dc,12,gl_strDlgFont.c_str());
		SIZE						sizeTitles = {0,0};
		SIZE						sizeValues = {0,0};

		dc.SetFont(fntText);
	
		sizeTitles = MaxSize(sizeTitles,dc.GetTextExtent(wstrFish));
		sizeTitles = MaxSize(sizeTitles,dc.GetTextExtent(wstrShark));
		sizeTitles = MaxSize(sizeTitles,dc.GetTextExtent(wstrIterations));

		sizeValues = MaxSize(sizeValues,dc.GetTextExtent(wstrFishCount));
		sizeValues = MaxSize(sizeValues,dc.GetTextExtent(wstrSharkCount));
		sizeValues = MaxSize(sizeValues,dc.GetTextExtent(wstrIterationCount));

		const unsigned uWTotal = sizeTitles.cx + sizeValues.cx + 5;
		const unsigned uHLine  = max(sizeValues.cy,sizeTitles.cy);
		const unsigned uHTotal = uHLine * 3 + 2*5;

		dc.SetTextColor(m_colorGraph);
		dc.SetBkColor(m_colorPaper);

		rcText.left   = rcDst.left;
		rcText.top    = rcDst.top;
		rcText.right  = rcDst.left + 5 + sizeTitles.cx + 5 + sizeValues.cx;
		rcText.bottom = rcDst.top + 5 + 3*uHLine;

		if(rcText.width()>rcDst.width() || rcText.height()>rcDst.height())
		{
			rcText.right  = rcText.left;
			rcText.bottom = rcText.top;
		}
		else
		{
			int x1 = rcDst.left + 5;
			int x2 = x1 + sizeTitles.cx + 5 + sizeValues.cx;
			int y  = rcDst.top + 5;

			dc.SetTextAlign(TA_LEFT|TA_TOP);
			dc.TextOut(x1,y,wstrFish);
			dc.SetTextAlign(TA_RIGHT|TA_TOP);
			dc.TextOut(x2,y,wstrFishCount);
			y += uHLine;

			dc.SetTextAlign(TA_LEFT|TA_TOP);
			dc.TextOut(x1,y,wstrShark);
			dc.SetTextAlign(TA_RIGHT|TA_TOP);
			dc.TextOut(x2,y,wstrSharkCount);
			y += uHLine;

			dc.SetTextAlign(TA_LEFT|TA_TOP);
			dc.TextOut(x1,y,wstrIterations);
			dc.SetTextAlign(TA_RIGHT|TA_TOP);
			dc.TextOut(x2,y,wstrIterationCount);
			y += uHLine;
		}
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}

	return rcText;
} // end - Renderer::RenderText


