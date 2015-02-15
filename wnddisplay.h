/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once


class WndDisplay : public tbase2::windows::gui::Wnd
{
	public:
			static const std::wstring CLASSNAME;

					WndDisplay();
			virtual ~WndDisplay();

			static bool RegisterClass(HINSTANCE hinst);

			virtual bool SetData(const char *pcData, const unsigned uWidth, const unsigned uHeight);

			virtual inline void SetColorWater(const COLORREF color) {m_crWater=color;}
			virtual inline void SetColorFish (const COLORREF color) {m_crFish=color;}
			virtual inline void SetColorShark(const COLORREF color) {m_crShark=color;}

	protected:
				virtual bool OnPaint(
				);

				virtual bool OnDestroy(
				);

				virtual void Render(tbase2::windows::gdi::DeviceContext &dc,
									const unsigned						width,
									const unsigned						height
				);

	private:	
			char	 *m_pcData;
			size_t	 m_sizData;
			unsigned m_uDataWidth;
			unsigned m_uDataHeight;
			COLORREF m_crWater;
			COLORREF m_crFish;
			COLORREF m_crShark;
};


