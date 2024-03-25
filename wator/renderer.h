/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "RingBuffer.h"

class Renderer
{
	public:
					Renderer(const std::locale &locale
					);

			virtual ~Renderer();

			virtual void Render(const wator::SIMULATION_PARAMETERS& parameter,
								const wator::SIMULATION_STATUS&		status,
								RingBuffer<HISTORIC_DATA>&			history,
								tbase2::windows::gdi::DeviceContext &dc,
								const unsigned						width,
								const unsigned						height
			);

			virtual inline void SetColorFish (const COLORREF &color)	{m_colorFish=color;}
			virtual inline void SetColorShark(const COLORREF &color)	{m_colorShark=color;}
			virtual inline void SetColorWater(const COLORREF &color)	{m_colorWater=color;}
			virtual void SetDefault();

			virtual inline COLORREF GetColorFish() const	{return m_colorFish;}
			virtual inline COLORREF GetColorShark() const	{return m_colorShark;}
			virtual inline COLORREF GetColorWater() const	{return m_colorWater;}

	protected:
				virtual void RenderDisplay(const wator::SIMULATION_PARAMETERS  &parameter,
										   const std::string& data,
										   tbase2::windows::gdi::DeviceContext &dc,
										   const tbase2::windows::gdi::Rect	   &rcDst
				);

				virtual void RenderDiagram(const wator::SIMULATION_PARAMETERS& parameter, 
										   const wator::SIMULATION_STATUS	   &status,
										   RingBuffer<HISTORIC_DATA>		   &history,
										   tbase2::windows::gdi::DeviceContext &dc,
										   const tbase2::windows::gdi::Rect	   &rcDst
				);


				virtual RECT RenderText(const wator::SIMULATION_STATUS		&status,
									    tbase2::windows::gdi::DeviceContext &dc,
										const tbase2::windows::gdi::Rect	&rcDst
				);
										
				static inline SIZE MaxSize(const SIZE &size1,
	  									   const SIZE &size2)
				{
					SIZE sizeNew = {max(size1.cx,size2.cx),
									max(size1.cy,size2.cy)
					};

					return sizeNew;
				} // end - max


	private:
			const std::locale &m_locale;
			COLORREF		  m_colorFish;
			COLORREF		  m_colorShark;
			COLORREF		  m_colorWater;
			COLORREF		  m_colorPaper;
			COLORREF		  m_colorGraph;
			COLORREF		  m_colorGrid;
			unsigned		  m_puFish[10000];
			unsigned		  m_puShark[10000];
			unsigned		  m_uOfs;

			static const COLORREF DEFAULT_COLOR_FISH;
			static const COLORREF DEFAULT_COLOR_SHARK;
			static const COLORREF DEFAULT_COLOR_WATER;
			static const COLORREF DEFAULT_COLOR_PAPER;
			static const COLORREF DEFAULT_COLOR_GRAPH;
			static const COLORREF DEFAULT_COLOR_GRID;
};
