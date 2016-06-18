#ifndef _KMAS_COMX_WTL_EXTEND_FRAME_H
#define _KMAS_COMX_WTL_EXTEND_FRAME_H

#include <wtlctrlxp.h>
#include <wtlctrlxp2.h>
#include <wtltreelistview.h>

#include <atlframe.h>
#include <atlctrlsext.h>
#include <atlgdix.h>
#include <OutlookBar.h>

#include <atldock.h>
#include <atldock2.h>

#include <wtlmultipanestatusbar.h>

#define	ATL_IDW_TOOLBAR_BOTTOM		(ATL_IDW_COMMAND_BAR + 1)

template <class T>
class ATL_NO_VTABLE CFrameWindowXPImpl
{
private:
	bool m_bCloseMenuTooltip;
public:
	BEGIN_MSG_MAP(CFrameWindowImplBase)
		MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
	END_MSG_MAP()
public:
	CFrameWindowXPImpl() : m_bCloseMenuTooltip(false)
	{
		/*Do nothing*/
	}

	void EnableMenuTooltip(bool bFlag = true)
	{
		m_bCloseMenuTooltip = !bFlag;
	}

	LRESULT OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_bCloseMenuTooltip)
			return 1;

		T *pT = static_cast<T*>(this);

		if(pT->m_hWndStatusBar == NULL)
			return 1;

		WORD wFlags = HIWORD(wParam);
		if(wFlags == 0xFFFF && lParam == NULL)   // menu closing
		{
			::SendMessage(pT->m_hWndStatusBar, SB_SIMPLE, FALSE, 0L);
		}
		else
		{
			const int cchBuff = 256;
			TCHAR szBuff[cchBuff];
			szBuff[0] = 0;
			if(!(wFlags & MF_POPUP))
			{
				WORD wID = LOWORD(wParam);
				// check for special cases
				if(wID >= 0xF000 && wID < 0xF1F0)                              // system menu IDs
					wID = (WORD)(((wID - 0xF000) >> 4) + ATL_IDS_SCFIRST);
				else if(wID >= ID_FILE_MRU_FIRST && wID <= ID_FILE_MRU_LAST)   // MRU items
					wID = ATL_IDS_MRU_FILE;
				else if(wID >= ATL_IDM_FIRST_MDICHILD)                         // MDI child windows
					wID = ATL_IDS_MDICHILD;

#if (_ATL_VER >= 0x0700)
				int nRet = ::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), wID, szBuff, cchBuff);
#else // !(_ATL_VER >= 0x0700)
				int nRet = ::LoadString(_Module.GetResourceInstance(), wID, szBuff, cchBuff);
#endif // !(_ATL_VER >= 0x0700)
				for(int i = 0; i < nRet; i++)
				{
					if(szBuff[i] == _T('\n'))
					{
						szBuff[i] = 0;
						break;
					}
				}
			}
			//::SendMessage(pT->m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			//::SendMessage(pT->m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)szBuff);
			m_StatusBar.SetPaneText(0, WTL::CString("Cue Line: ") + szBuff);
		}

		return 1;
	}
protected:
	struct TStatusBarInfo{
		int      id;
		UINT     idIcon;
		char     *initText;
	};
public:
	CMPSBarWithAll   m_StatusBar;
	HWND m_hBottomRebar;
	HWND m_hAnimationRebar;
protected:
	template<typename TCmdBar>
	void CreateXPCommandBarOnRebar(TCmdBar &cmdbar)
	{
		T *pT = static_cast<T*>(this);

		HWND hWndCmdBar = cmdbar.Create(pT->m_hWnd, pT->rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		cmdbar.AttachMenu(pT->GetMenu());
		pT->SetMenu(NULL);

		pT->AddSimpleReBarBand(hWndCmdBar);
	}

	template<typename TToolBar, typename TCmdBar>
		void CreateXPToolBarOnRebar(TToolBar &tb, TCmdBar &cmdbar, UINT uToolbar, UINT uImage, UINT uImageHot, UINT uImageGray, int nSize, BOOL bNew = TRUE, HINSTANCE hInst = NULL, UINT *ids = NULL, const int ids_num = 0)
	{
		T *pT = static_cast<T*>(this);

		if (ids == NULL)
		{
			tb = pT->CreateSimpleToolBarCtrl(pT->m_hWnd, uToolbar,  FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}
		else
		{
			tb = pT->CreateSimpleToolBarCtrlExt(pT->m_hWnd, uToolbar, ids, ids_num, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}
		
		tb.LoadTrueColorToolBar(nSize , uImage , uImageHot , uImageGray, hInst);

		cmdbar.LoadImagesExt(uToolbar, uImage, uImageGray, hInst, ids, ids_num);

		//pT->AddSimpleReBarBand(tb, NULL, bNew , 0);
		pT->AddSimpleReBarBandCtrl(pT->m_hWndToolBar, tb, 0, NULL, bNew, 0);

		cmdbar.AddToolbar(tb);

		pT->UIAddToolBar(tb);
	}

	template<typename TToolBar, typename TCmdBar>
	void CreateXPToolBarOnBottomRebar(TToolBar &tb, TCmdBar &cmdbar, UINT uToolbar, UINT uImage, UINT uImageHot, UINT uImageGray, int nSize, BOOL bNew = TRUE, HINSTANCE hInst = NULL, UINT *ids = NULL, const int ids_num = 0)
	{
		T *pT = static_cast<T*>(this);

		if (ids == NULL)
		{
			tb = pT->CreateSimpleToolBarCtrl(pT->m_hWnd, uToolbar,  FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}
		else
		{
			tb = pT->CreateSimpleToolBarCtrlExt(pT->m_hWnd, uToolbar, ids, ids_num, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}

		tb.LoadTrueColorToolBar(nSize , uImage , uImageHot , uImageGray, hInst);

		cmdbar.LoadImagesExt(uToolbar, uImage, uImageGray, hInst, ids, ids_num);

		pT->AddSimpleReBarBandCtrl(m_hBottomRebar, tb, 0, NULL, bNew, 0);

		cmdbar.AddToolbar(tb);

		pT->UIAddToolBar(tb);
	}

	template<typename TToolBar, typename TCmdBar>
		void CreateXPToolBarOnRebar(TToolBar &tb, TCmdBar &cmdbar, UINT uToolbar, CImageList &uImage, CImageList &uImageHot, CImageList &uImageGray, int nSize, BOOL bNew = TRUE, HINSTANCE hInst = NULL, UINT *ids = NULL, const int ids_num = 0)
	{
		T *pT = static_cast<T*>(this);

		if (ids == NULL)
		{
			tb = pT->CreateSimpleToolBarCtrl(pT->m_hWnd, uToolbar,  FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}
		else
		{
			tb = pT->CreateSimpleToolBarCtrlExt(pT->m_hWnd, uToolbar, ids, ids_num, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}

		tb.LoadTrueColorToolBar(uImage , uImageHot , uImageGray);

		cmdbar.LoadImagesExt(uToolbar, uImage, uImageGray, hInst, ids, ids_num);

		//pT->AddSimpleReBarBand(tb, NULL, bNew , 0);
		pT->AddSimpleReBarBandCtrl(pT->m_hWndToolBar, tb, 0, NULL, bNew, 0);

		cmdbar.AddToolbar(tb);

		pT->UIAddToolBar(tb);
	}

	template<typename TToolBar, typename TCmdBar>
		void CreateXPToolBarOnBottomRebar(TToolBar &tb, TCmdBar &cmdbar, UINT uToolbar, CImageList &uImage, CImageList &uImageHot, CImageList &uImageGray, int nSize, BOOL bNew = TRUE, HINSTANCE hInst = NULL, UINT *ids = NULL, const int ids_num = 0)
	{
		T *pT = static_cast<T*>(this);

		if (ids == NULL)
		{
			tb = pT->CreateSimpleToolBarCtrl(pT->m_hWnd, uToolbar,  FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}
		else
		{
			tb = pT->CreateSimpleToolBarCtrlExt(pT->m_hWnd, uToolbar, ids, ids_num, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		}

		tb.LoadTrueColorToolBar(uImage , uImageHot , uImageGray);

		cmdbar.LoadImagesExt(uToolbar, uImage, uImageGray, hInst, ids, ids_num);

		pT->AddSimpleReBarBandCtrl(m_hBottomRebar, tb, 0, NULL, bNew, 0);

		cmdbar.AddToolbar(tb);

		pT->UIAddToolBar(tb);
	}

	/*template<typename TToolBar, typename TCmdBar>
		void CreateXPToolBarOnBottomRebar(TToolBar &tb, TCmdBar &cmdbar, UINT uToolbar, UINT uImage, UINT uImageHot, UINT uImageGray, int nSize, BOOL bNew = TRUE)
	{
		T *pT = static_cast<T*>(this);

		tb = pT->CreateSimpleToolBarCtrl(pT->m_hWnd, uToolbar, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		tb.LoadTrueColorToolBar(nSize , uImage , uImageHot , uImageGray);

		cmdbar.LoadImagesExt(uToolbar, uImage, uImageGray);

		pT->AddSimpleReBarBandCtrl(m_hBottomRebar, tb);

		cmdbar.AddToolbar(tb);

		pT->UIAddToolBar(tb);
	}*/

	void CreateRebar()
	{
		T *pT = static_cast<T*>(this);

		pT->CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		m_hBottomRebar = pT->CreateSimpleReBarCtrl(pT->m_hWnd, ATL_SIMPLE_REBAR_NOBORDER_STYLE|CCS_BOTTOM, ATL_IDW_TOOLBAR_BOTTOM + 1);
	}

	template<typename TCmdBar>
	void PrepareCmdBar(TCmdBar &cmdbar)
	{
		cmdbar.Prepare();
	}

	template<typename TMultiPaneStatusBar>
	void CreateXPStatusBar(TMultiPaneStatusBar &statusbar, TStatusBarInfo *info, int iInfoLen)
	{
		T *pT = static_cast<T*>(this);

		pT->CreateSimpleStatusBar();
		statusbar.SubclassWindow(pT->m_hWndStatusBar);

		int *anPanes = new int[iInfoLen];
		int i = 0;

		for (i = 0 ; i < iInfoLen ; ++i)
		{
			anPanes[i] = info[i].id;
		}

		statusbar.SetPanes(anPanes , iInfoLen);

		for (i = 0 ; i < iInfoLen ; ++i)
		{
			if (info[i].idIcon != -1)
			{
				statusbar.SetPaneIcon(info[i].id , AtlLoadIconImage(info[i].idIcon, LR_DEFAULTCOLOR));
			}
			if (info[i].initText != NULL)
			{
				statusbar.SetPaneText(info[i].id , info[i].initText);
			}
		}

		pT->UIAddStatusBar(statusbar);

		delete[] anPanes;
	}

	void DoIdle()
	{
        T *pT = static_cast<T*>(this);
		
		pT->UIUpdateToolBar();
		pT->UIUpdateStatusBar();
	}

	bool Switch2CheckStatusOf(UINT nID)
	{
		T *pT = static_cast<T*>(this);

		DWORD state = pT->UIGetState(nID);

		pT->UISetCheck(nID, !(state&pT->UPDUI_CHECKED));

		return !(state&pT->UPDUI_CHECKED);
	}

	void UpdateLayout(BOOL bResizeBars = TRUE)
	{
		T *pT = static_cast<T*>(this);

		RECT rect = { 0 };
		pT->GetClientRect(&rect);

		// resize toolbar
		if (pT->m_hWndToolBar != NULL && ((DWORD)::GetWindowLong(pT->m_hWndToolBar, GWL_STYLE) & WS_VISIBLE))
		{
			if (bResizeBars)
				::SendMessage(pT->m_hWndToolBar, WM_SIZE, 0, 0);
			RECT rectTB = { 0 };
			::GetWindowRect(pT->m_hWndToolBar, &rectTB);
			rect.top += rectTB.bottom - rectTB.top;
		}

		// resize status bar
		if (pT->m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(pT->m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
		{
			if(bResizeBars)
				::SendMessage(pT->m_hWndStatusBar, WM_SIZE, 0, 0);
			RECT rectSB = { 0 };
			::GetWindowRect(pT->m_hWndStatusBar, &rectSB);
			rect.bottom -= rectSB.bottom - rectSB.top;
		}

		// Resize bottom toolbar
		if (m_hBottomRebar != NULL && ((DWORD)::GetWindowLong(m_hBottomRebar, GWL_STYLE) & WS_VISIBLE))
		{
			if (bResizeBars)
				::SendMessage(m_hBottomRebar, WM_SIZE, 0, 0);

			RECT rectTB = { 0 };
			::GetWindowRect(m_hBottomRebar, &rectTB);

			int nHeight = rectTB.bottom - rectTB.top;
			rect.bottom -= nHeight;

			// Position above status bar!
			if (pT->m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(pT->m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
			{			
				RECT rectSB = { 0 };
				::GetWindowRect(pT->m_hWndStatusBar, &rectSB);

				pT->ScreenToClient(&rectTB);
				pT->ScreenToClient(&rectSB);

				rectTB.top = rectSB.top - nHeight;
				rectTB.bottom = rectTB.top + nHeight;

				// Move the bottom rebar
				::SetWindowPos(m_hBottomRebar, NULL, rectTB.left, rectTB.top,
					rect.right - rect.left, nHeight, SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}

		// resize client window
		if (pT->m_hWndClient != NULL)
		{
			::SetWindowPos(pT->m_hWndClient, NULL, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);

			if (bResizeBars)
				::SendMessage(pT->m_hWndClient, WM_SIZE, 0, 0);
		}
	}
};

#endif