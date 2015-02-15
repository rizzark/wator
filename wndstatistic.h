/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once


class WndStatistic : public tbase2::windows::gui::Wnd
{
	public:
			static const std::wstring CLASSNAME;
			static const unsigned	  SAVECYCLES;

					WndStatistic();
			virtual ~WndStatistic();

			static bool RegisterClass(HINSTANCE hinst);

			virtual void AddData  (const unsigned uFishCount, const unsigned uSharkCount, const unsigned uMax);
			virtual void ClearData();

			virtual inline void SetColorFish (const COLORREF color) {m_crFish=color;}
			virtual inline void SetColorShark(const COLORREF color) {m_crShark=color;}

	protected:
				virtual bool OnCreate(CREATESTRUCT *pcs,
									  LRESULT	   &lres
				);

				virtual bool OnPaint(
				);

	private:	
			std::vector<unsigned> m_vSharkCount;
			std::vector<unsigned> m_vFishCount;
			unsigned			  m_uCycles;
			COLORREF			  m_crFish;
			COLORREF			  m_crShark;
			unsigned			  m_uMax;
};
	

