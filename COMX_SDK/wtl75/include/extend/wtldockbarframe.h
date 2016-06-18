//------------------------------------------------------------------------------
// File    : DockBarFrame.h 
// Version : 1.01
// Date    : July 13, 2004
// Author  : Roger Headrick
// Email   : lilsroro@yahoo.com 
// Systems : VC6/7.0 and VC7.1 (Run under (Window 98/ME), Windows Nt 2000/XP)
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
//
// 10/09/2004 - Fixed drag rect drawing bug in release configuration


#ifndef __DOCKBARFRAME_H__
#define __DOCKBARFRAME_H__

#pragma once

#if (_WIN32_IE < 0x0400)
	#error dockbarframe.h requires IE Version 4.0 or higher
#endif

#ifdef _WIN32_WCE
	#error dockbarframe.h is not compatible with windows CE
#endif

#ifndef __ATLCTRLS_H__
	#error dockbarframe.h requires atlctrls.h to be included first
#endif

#ifndef __ATLFRAME_H__
	#error dockbarframe.h requires atlframe.h to be included first
#endif

#if (_ATL_VER >= 0x0700)
	#if !defined(__ATLSTR_H__)
		#error dockbarframe.h requires CString. In VC 7, include atlstr.h
	#endif
	#if defined(__ATLMISC_H__) && !defined(_WTL_NO_CSTRING)
		#error In VC 7, please define _WTL_NO_CSTRING if you include atlmisc.h
	#endif
	#if !defined(__ATLMISC_H__) && !defined(__ATLTYPES_H__)
		#error dockbarframe.h requires either atlmisc.h or atltypes.h to be included first
	#endif
#else
	#if !defined(_WTL_USE_CSTRING)
		#error dockbarframe.h requires CString. In VC 6, be sure to include atlmisc.h.
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CReBarBandInfo
// CFloatBarWindowImpl<T, TBase, TWinTraits>
// CFloatBarWindow
// CDockBarFrameWindowImpl<T, TBase, TWinTraits>
//


// id's for rebars
#define IDW_REBAR_FIRST		ATL_IDW_COMMAND_BAR+10
#define IDW_REBAR_TOP		IDW_REBAR_FIRST
#define IDW_REBAR_BOTTOM	IDW_REBAR_FIRST+1
#define IDW_REBAR_LEFT		IDW_REBAR_FIRST+2
#define IDW_REBAR_RIGHT		IDW_REBAR_FIRST+3
#define IDW_REBAR_LAST		IDW_REBAR_RIGHT

//misc defines
#if !defined(BTNS_SHOWTEXT)
	#define BTNS_SHOWTEXT				0x0040
#endif
#if !defined(TBSTYLE_EX_MIXEDBUTTONS)
	#define TBSTYLE_EX_MIXEDBUTTONS     0x00000008
#endif

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CReBarBandInfo helper class
class CReBarBandInfo : public REBARBANDINFO
{
public:

	CReBarBandInfo(UINT uMask = 0)
	{
		memset(this, 0, sizeof(REBARBANDINFO));
		cbSize = sizeof(REBARBANDINFO);
		fMask = uMask;		
	}

	operator LPREBARBANDINFO() { return this; }

	operator LPARAM() { return (LPARAM)this; }

	CReBarBandInfo& operator= ( const REBARBANDINFO& srcBand )
	{
		CopyFrom ( &srcBand ) ;
		return *this ;
	}

	void CopyTo(LPREBARBANDINFO lpBand)
	{
		ATLASSERT(lpBand);

		::CopyMemory(lpBand, this, sizeof(REBARBANDINFO));
	}

	void CopyFrom(LPCREBARBANDINFO lpBand)
	{
		ATLASSERT(lpBand);

		::CopyMemory(this, lpBand, sizeof(REBARBANDINFO));
	}

};


///////////////////////////////////////////////////////////////////////////////
// CFloatBarWindowImpl

// user message sent by FloatBarWindow to owner	
#define FLBM_NOTIFY		WM_USER+50
//wParam LoWord = Band ID, HiWord = Notify Code 
//lParam = (HWND)FloatBar, 
//return not used

// Notify codes:
#define FLBN_CLOSEBUTTON	1
#define FLBN_BEGINDRAG		2
#define FLBN_DRAGMOVE		3
#define FLBN_ENDDRAG		4
#define FLBN_CAPTIONDBLCLK  5
#define FLBN_CONTEXTMENU	6

#define FLOATBAR_NOCLIP_WINDOW_STYLE	WS_POPUPWINDOW|WS_VISIBLE/*|WS_CLIPCHILDREN*/|WS_CLIPSIBLINGS|WS_DLGFRAME|WS_THICKFRAME|WS_OVERLAPPED
#define FLOATBAR_CLIP_WINDOW_STYLE		WS_POPUPWINDOW|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_DLGFRAME|WS_THICKFRAME|WS_OVERLAPPED
#define FLOATBAR_WINDOW_STYLE_EX		WS_EX_NOPARENTNOTIFY|WS_EX_TOOLWINDOW|WS_EX_WINDOWEDGE

typedef CWinTraits<FLOATBAR_NOCLIP_WINDOW_STYLE, FLOATBAR_WINDOW_STYLE_EX> CFloatBarWinTraits;

template <class T, class TBase = ATL::CWindow, class TWinTraits = CFloatBarWinTraits>
class ATL_NO_VTABLE CFloatBarWindowImpl : public CFrameWindowImpl<T, TBase, TWinTraits>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, 0)

	typedef CFrameWindowImpl<T, TBase, TWinTraits> baseClass;

	bool m_bTracking;
	UINT m_uTrackSide;
	CPoint m_pointLastTrack;
	CSize m_sizeMinTrack;
	CSize m_sizeMaxTrack;
	bool m_bDragging;
	int m_cyRowHeight;
	CReBarBandInfo m_band;
	UINT m_idPrevBandOwner;
	bool m_bToolBarBand;

	CFloatBarWindowImpl():
		m_bTracking(false),
		m_uTrackSide(0),
		m_bDragging(false),
		m_idPrevBandOwner(0),
		m_cyRowHeight(0),
		m_bToolBarBand(false)
		{
			m_pointLastTrack = CPoint(0);
			m_sizeMinTrack = m_sizeMaxTrack = CSize(0);
		}

	~CFloatBarWindowImpl()
	{
		if(m_hWnd != NULL)
			DestroyWindow();
	}

//attributes
	UINT GetRowHeight()
	{
		return m_cyRowHeight;
	}

	UINT GetPrevBandOwnerId(){return m_idPrevBandOwner;}
	void SetPrevBandOwnerId(UINT nId){m_idPrevBandOwner = nId;}

	BOOL InsertBand(LPCREBARBANDINFO lpBand)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		m_band = *lpBand;
		ATLASSERT(::IsWindow(m_band.hwndChild));
		::SetParent(m_band.hwndChild, m_hWnd);
		m_hWndClient = m_band.hwndChild;
		m_bToolBarBand = ::SendMessage(m_band.hwndChild, TB_BUTTONCOUNT, 0, 0) > 0;
		CalcTrackSizes();
		::ShowWindow(m_hWndClient, SW_SHOW);
		if(m_bToolBarBand)
			SetWindowPos(NULL, 0, 0, m_sizeMaxTrack.cx, m_sizeMinTrack.cy, SWP_NOMOVE | SWP_NOZORDER);
		UpdateLayout();

		return TRUE;
	}

	BOOL GetBandInfo(LPREBARBANDINFO lpBand)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		
		LPTSTR pText = lpBand->lpText;
		*lpBand = m_band;
		lpBand->lpText = pText;
		if((m_band.fMask & RBBIM_TEXT) != 0)
		{
			::GetWindowText(m_hWnd, lpBand->lpText, lpBand->cch);
		}

		return TRUE;
	}

	BOOL DeleteBand()
	{
		m_hWndClient = NULL;
		return TRUE;
	}

	int IdToIndex(UINT uBandID)
	{
		if(m_band.wID == uBandID)
			return 0;

		return -1;
	}

	BOOL ShowBand(BOOL bShow)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		return ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}


	CSize GetNcSize()
	{
		return CSize(::GetSystemMetrics(SM_CXSIZEFRAME) * 2,
			::GetSystemMetrics(SM_CYSIZEFRAME) * 2 + ::GetSystemMetrics(SM_CYSMCAPTION));
	}

	int GetBandRows()
	{
		ATLASSERT(::IsWindow(m_hWndClient));

		int nRet = 1;
		if(m_bToolBarBand && (m_hWndClient != NULL))
		{
			CToolBarCtrl toolbar = m_hWndClient;
			nRet = toolbar.GetRows();
		}
		return nRet;
	}

	BOOL SetBandRows(int nRows)
	{
		ATLASSERT(::IsWindow(m_hWndClient));

		BOOL bRet = FALSE;
		if(m_bToolBarBand && (m_hWndClient != NULL) && (nRows > 0))
		{
			CToolBarCtrl toolbar = m_hWndClient;
			if(nRows != toolbar.GetRows())
			{
				CRect rectTb, rectWnd;
				toolbar.SetRows(nRows, FALSE, rectTb);
				GetWindowRect(rectWnd);
				CSize sizeNc = GetNcSize();
				SetWindowPos(NULL, rectWnd.left, rectWnd.top, 
					max(rectTb.Width() + sizeNc.cx, m_sizeMinTrack.cx), 
					rectTb.Height() + sizeNc.cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);	
			}
			bRet = TRUE;
		}
		return bRet;	
	}

// Implementation
	void CalcTrackSizes()
	{
		m_sizeMinTrack = m_sizeMaxTrack = CSize(0);

		if(!::IsWindow(m_hWndClient))
			return;

		//defaults
		UINT cxMinChild = m_band.cxMinChild;
		UINT cxMaxChild = m_band.cxIdeal;
		UINT cyMinChild = m_cyRowHeight = m_band.cyMinChild;
		UINT cyMaxChild = max(m_band.cyMaxChild, cyMinChild);

		if(m_bToolBarBand)
		{
			CToolBarCtrl toolbar = m_hWndClient;
			int nBtnCount = toolbar.GetButtonCount();
			CRect rect;
			toolbar.SetRows(nBtnCount, TRUE, rect);
			cxMinChild = rect.Width();
			cyMaxChild = rect.Height();
			//set cxMinChild to the widest button
			for(int i = 0; i < nBtnCount; i++)
			{
				toolbar.GetItemRect(i, rect);
				if(cxMinChild < (UINT)rect.Width())
					cxMinChild = rect.Width();
			}
			toolbar.SetRows(1, FALSE, rect);
			cxMaxChild = rect.Width();
		}
		else 
		{
			CRect rect;
			::GetWindowRect(m_band.hwndChild, rect);
			if((m_band.fStyle & RBBS_FIXEDSIZE) != 0)
			{
				cxMinChild = cxMaxChild = rect.Width();
				cyMinChild = cyMaxChild = rect.Height();
			}
			else
			{
				cxMaxChild = ::GetSystemMetrics(SM_CXSCREEN);
			}
		}

		CSize sizeNc = GetNcSize();
		m_sizeMinTrack.cx = sizeNc.cx + cxMinChild;
		m_sizeMaxTrack.cx = sizeNc.cx + cxMaxChild;
		m_sizeMinTrack.cy = sizeNc.cy + cyMinChild;
		m_sizeMaxTrack.cy = sizeNc.cy + cyMaxChild;
	}

	void DoTracking(CPoint pointCursor, UINT uTrackSide)
	{
		CSize sizeMove(pointCursor.x - m_pointLastTrack.x, 
			pointCursor.y - m_pointLastTrack.y);
		if((abs(sizeMove.cx) < ::GetSystemMetrics(SM_CXDRAG))
			&& (abs(sizeMove.cy) < ::GetSystemMetrics(SM_CYDRAG)))
			return;
		CRect rectWnd;
		GetWindowRect(rectWnd);
		CRect rectOldWnd = rectWnd;
		switch(uTrackSide)
		{
		case HTLEFT:
			rectWnd.left = pointCursor.x;
			break;
		case HTTOP:
			rectWnd.top = pointCursor.y;
			break;
		case HTRIGHT:
			rectWnd.right = pointCursor.x;
			break;
		case HTBOTTOM:
			rectWnd.bottom = pointCursor.y;
			break;
		}

		if(m_bToolBarBand)
		{
			CToolBarCtrl toolbar = m_hWndClient;
			int nBtnCount = toolbar.GetButtonCount();
			int nRows = toolbar.GetRows();
			int nOldRows = nRows;
			CSize sizeBtn = toolbar.GetButtonSize();
			if(rectWnd.Height() > rectOldWnd.Height())
			{
				if( (rectOldWnd.Height() < m_sizeMaxTrack.cy)
					&& ((rectWnd.Height() - rectOldWnd.Height()) >= m_cyRowHeight)
					&& (((sizeMove.cy < 0) && (uTrackSide == HTTOP)) 
						|| ((sizeMove.cy > 0) && (uTrackSide == HTBOTTOM))))
				{
					nRows++;
				}
			}
			else if(rectWnd.Width() < rectOldWnd.Width())
			{
				if( (rectOldWnd.Height() < m_sizeMaxTrack.cy)
					&& ((rectOldWnd.Width() - rectWnd.Width()) >= sizeBtn.cx)
					&& (((sizeMove.cx > 0) && (uTrackSide == HTLEFT)) 
						|| ((sizeMove.cx < 0) && (uTrackSide == HTRIGHT))) )
				{
					nRows++;
				}
			}
			else if(rectWnd.Height() < rectOldWnd.Height())
			{
				if( (rectOldWnd.Height() > m_sizeMinTrack.cy)
					&& ((rectOldWnd.Height() - rectWnd.Height()) >= m_cyRowHeight)
					&& (((sizeMove.cy > 0) && (uTrackSide == HTTOP)) 
						|| ((sizeMove.cy < 0) && (uTrackSide == HTBOTTOM))))
				{
					nRows--;
				}
			}
			else if(rectWnd.Width() > rectOldWnd.Width())
			{
				if( (rectOldWnd.Height() > m_sizeMinTrack.cy)
					&& ((rectWnd.Width() - rectOldWnd.Width()) >= sizeBtn.cx)
					&& (((sizeMove.cx < 0) && (uTrackSide == HTLEFT)) 
						|| ((sizeMove.cx > 0) && (uTrackSide == HTRIGHT))) )
				{
					nRows--;
				}
			}
			if(nRows != nOldRows)
			{
				CRect rect;
				toolbar.SetRows(nRows, nRows > nOldRows, rect);
				CSize sizeNc = GetNcSize();
				SetWindowPos(NULL, rectWnd.left, rectWnd.top, 
					max(rect.Width() + sizeNc.cx, m_sizeMinTrack.cx), 
					rect.Height() + sizeNc.cy, SWP_NOZORDER);
			}
		}//if(m_bToolBarBand)
		else if((uTrackSide == HTLEFT) || (uTrackSide == HTRIGHT))
		{
			if(m_sizeMinTrack.cx != m_sizeMaxTrack.cx)
			{
				SetWindowPos(NULL, rectWnd.left, rectWnd.top, 
					min(max(rectWnd.Width(), m_sizeMinTrack.cx), m_sizeMaxTrack.cx), 
					rectWnd.Height(), SWP_NOZORDER);
			}
		}
		else
		{
			if(m_sizeMinTrack.cy != m_sizeMaxTrack.cy)
			{
				SetWindowPos(NULL, rectWnd.left, rectWnd.top, 
					rectWnd.Width(),
					min(max(rectWnd.Height(), m_sizeMinTrack.cy), m_sizeMaxTrack.cy), 
					SWP_NOZORDER);
			}
		}

		m_pointLastTrack = pointCursor;
	}

	BEGIN_MSG_MAP(CFloatBarWindowImpl)
		MESSAGE_HANDLER( WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER( WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER( WM_CLOSE, OnClose)
		MESSAGE_HANDLER( WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER( WM_NCLBUTTONDOWN, OnNcLButtonDown)
		MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER( WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER( WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER( WM_NCRBUTTONDOWN, OnNcRButtonDown)
		MESSAGE_HANDLER( WM_NCLBUTTONDBLCLK, OnNcLButtonDblClk)
		MESSAGE_HANDLER( WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER( RB_IDTOINDEX, OnReBarMessage)
		MESSAGE_HANDLER( RB_SHOWBAND, OnReBarMessage)
		MESSAGE_HANDLER( RB_DELETEBAND, OnReBarMessage)
		MESSAGE_HANDLER( RB_INSERTBAND, OnReBarMessage)
		MESSAGE_HANDLER( RB_GETBANDINFO, OnReBarMessage)
		MESSAGE_HANDLER( RB_GETROWHEIGHT, OnReBarMessage)
		MESSAGE_HANDLER( WM_SETTINGCHANGE, OnSettingChange)
		FORWARD_NOTIFICATIONS()
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		switch(LOWORD(lParam))
		{
		case HTLEFT:
		case HTRIGHT:
			if(m_sizeMinTrack.cx == m_sizeMinTrack.cy)
			{
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
				bHandled = TRUE;
			}
			break;
		case HTTOP:
		case HTBOTTOM:
			if(m_sizeMinTrack.cy == m_sizeMaxTrack.cy)
			{
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
				bHandled = TRUE;
			}
			break;
		case HTTOPLEFT:
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			bHandled = TRUE;
			break;
		}
		return 1;
	}

	LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		this->SendMessageToDescendants(uMsg, wParam, lParam);
		this->DefWindowProc(uMsg, wParam, lParam);
		if((m_hWndClient != NULL) && (wParam == SPI_SETNONCLIENTMETRICS))
		{
			int nRows = GetBandRows();
			CalcTrackSizes();
			SetBandRows(nRows);
			CRect rect;
			::GetWindowRect(m_hWndClient, rect);
			CSize size = GetNcSize();
			SetWindowPos(NULL, 0, 0, rect.Width() + size.cx, rect.Height() + size.cy,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
		}
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect rect;
		GetClientRect(rect);
		::FillRect((HDC)wParam, rect, ::GetSysColorBrush(COLOR_BTNFACE));
		return 1;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		::PostMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_CLOSEBUTTON), 
			(LPARAM)m_hWnd); 
		return 0;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_bToolBarBand)
		{
			// bypass setting focus to a toolbar- causes tooltip problems
			return DefWindowProc(uMsg, wParam, lParam);
		}

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnNcLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRet = 0;
		bHandled = FALSE;
		UINT uHitTest = DefWindowProc(WM_NCHITTEST, 0, lParam);
		switch(uHitTest)
		{
		case HTCAPTION:
			{
				if(::GetActiveWindow() != m_hWnd)
				{
					SetActiveWindow();
				}

				m_bDragging = true;
				SetCapture();
				::SendMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_BEGINDRAG),
					(WPARAM)m_hWnd);
				bHandled = TRUE;
				break;
			}
		case HTLEFT:
		case HTTOP:
		case HTRIGHT:
		case HTBOTTOM:
			if( (((uHitTest == HTLEFT) || (uHitTest == HTRIGHT)) 
					&& (m_sizeMinTrack.cx != m_sizeMaxTrack.cx))
				||
				(((uHitTest == HTTOP) || (uHitTest == HTBOTTOM)) 
					&& (m_sizeMinTrack.cy != m_sizeMaxTrack.cy)))					
			{
				m_bTracking = true;
				m_uTrackSide = uHitTest;
				m_pointLastTrack = CPoint(lParam);
				SetCapture();
			}
			bHandled = TRUE;
			break;
		case HTTOPLEFT:
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
			bHandled = TRUE;
			break;
		}
		return lRet;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		if(m_bTracking)
		{
			CPoint point = CPoint(lParam);
			ClientToScreen(&point);
			DoTracking(point, m_uTrackSide);
			bHandled = TRUE;
		}
		else if(m_bDragging)
		{
			::SendMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_DRAGMOVE),
					(WPARAM)m_hWnd);
			bHandled = TRUE;
		}
		return 0;
	}

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		if(m_bTracking || m_bDragging)
		{
			if(GetCapture() == m_hWnd)
				::ReleaseCapture();

			m_bTracking = m_bDragging = false;
			bHandled = TRUE;
		}
		
		return 0;
	}

	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		if(m_bTracking)
		{
			m_bTracking = false;
			bHandled = TRUE;
		}
		else if(m_bDragging)
		{
			m_bDragging = false;
			::PostMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_ENDDRAG),
					(WPARAM)m_hWnd);
			bHandled = TRUE;
		}
		return 0;
	}

	LRESULT OnNcRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		UINT uHitTest = DefWindowProc(WM_NCHITTEST, 0, lParam);
		if(uHitTest == HTCAPTION)
		{
			// dont show system menu
			if(::GetActiveWindow() != m_hWnd)
				SetActiveWindow();

			return 0;
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnNcLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		UINT uHitTest = DefWindowProc(WM_NCHITTEST, 0, lParam);
		if(uHitTest == HTCAPTION)
		{
			::PostMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_CAPTIONDBLCLK),
				(WPARAM)m_hWnd);
			return 0;
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		::PostMessage(GetParent(), FLBM_NOTIFY, MAKEWPARAM(m_band.wID, FLBN_CONTEXTMENU),
				(WPARAM)m_hWnd);
		return 0;
	}

	LRESULT OnReBarMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = 0;
		switch(uMsg)
		{
		case RB_IDTOINDEX:
			lRet = IdToIndex((UINT)wParam);
			break;
		case RB_DELETEBAND:
			lRet = DeleteBand();
			break;
		case RB_SHOWBAND:
			lRet = ShowBand((BOOL)lParam);	
			break;
		case RB_INSERTBAND:
			lRet = InsertBand((LPREBARBANDINFO)lParam);
			break;
		case RB_GETBANDINFO:
			lRet = GetBandInfo((LPREBARBANDINFO)lParam);
			break;
		case RB_GETROWHEIGHT:
			lRet = GetRowHeight();
			break;
		}
		return lRet;
	}

};

///////////////////////////////////////////////////////////////////////////////
// CFloatBarWindow
class CFloatBarWindow: public CFloatBarWindowImpl<CFloatBarWindow>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("Wtl_FloatBarWindow"), 0)
};

///////////////////////////////////////////////////////////////////////////////
// CDockBarFrameWindowImpl

struct FindBandInfo
{
	HWND hwndParent;
	UINT idParent;
	UINT uBand;
};

// Add this define if you're building a MDI app.
#if defined(_WTL_USE_MDI)
	#define FRAME_BASE_CLASS  CMDIFrameWindowImpl
	template <class T, class TBase = CMDIWindow, class TWinTraits = ATL::CFrameWinTraits>
#else
	#define FRAME_BASE_CLASS  CFrameWindowImpl
	template <class T, class TBase = ATL::CWindow, class TWinTraits = ATL::CFrameWinTraits>
#endif//if defined(_WTL_USE_MDI)
class ATL_NO_VTABLE CDockBarFrameWindowImpl : public FRAME_BASE_CLASS<T, TBase, TWinTraits >
{
public:

	enum
	{
		m_cxDragBorder = 3,
		m_cyDragBorder = 3
	};

	struct _ScreenMetrics
	{
		int cxFullScreen;
		int cyFullScreen;
	};

	//info saved in registry for each rebar band
	struct _BandLayout
	{
		UINT idBand;
		UINT idParent;
		UINT uBand;
		UINT cxWidth;
		BOOL bNewRow;
		BOOL bVisible;
		UINT uRows;
		RECT rcFloatBar;
	};
	
	HWND		m_hWndDragSource;
	UINT		m_idDragSource;
	UINT		m_idDragBand;
	CRect		m_rectLastDrag;
	CSize		m_sizeDragFloat;
	CSize		m_sizeDragBorder;
	CSize		m_sizeFloatOffset;
	CSimpleMap<HWND, CFloatBarWindow*> m_mapFloatBars;
	bool		m_bRestoredLayout;
	CString		m_strAppRegKey;

	CDockBarFrameWindowImpl():
		m_hWndDragSource(NULL),
		m_idDragSource(0),
		m_idDragBand(0),
		m_bRestoredLayout(false)
	{
		m_rectLastDrag = CRect(0,0,0,0);
		m_sizeDragFloat = CSize(0,0);
		m_sizeDragBorder = CSize(m_cxDragBorder, m_cyDragBorder);
		m_sizeFloatOffset = CSize(0,0);
		m_strAppRegKey = _T("");
	}

	typedef CDockBarFrameWindowImpl thisClass;
	typedef FRAME_BASE_CLASS<T, TBase, TWinTraits > baseClass;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		if((m_hWndDragSource != NULL) && (pMsg->hwnd == m_hWndDragSource)
			&& (m_idDragSource != 0) && (pMsg->message == WM_MOUSEMOVE))
		{
			CPoint point = CPoint(pMsg->lParam);
			::ClientToScreen(pMsg->hwnd, &point);
			DoBandDrag(point);
		}
		else if((pMsg->message == WM_SYSKEYDOWN) && (GetActiveWindow() != m_hWnd))
		{
			SetActiveWindow();
			pMsg->hwnd = m_hWnd;
		}

		return FALSE;
	}

	HWND CreateSimpleReBarCtrl(HWND hWndParent, DWORD dwStyle = ATL_SIMPLE_REBAR_STYLE, UINT nID = IDW_REBAR_TOP)
	{
		ATLASSERT((nID >= IDW_REBAR_FIRST) && (nID <= IDW_REBAR_LAST));

		switch(nID)
		{
		case IDW_REBAR_BOTTOM:
			dwStyle |= CCS_BOTTOM | CCS_NOMOVEY | CCS_NOPARENTALIGN;
			break;
		case IDW_REBAR_LEFT:
			dwStyle |= CCS_VERT | CCS_NOPARENTALIGN;
			break;
		case IDW_REBAR_RIGHT:
			dwStyle |= CCS_VERT | CCS_RIGHT | CCS_NOPARENTALIGN;
			break;
		}
		
		return baseClass::CreateSimpleReBarCtrl(hWndParent, dwStyle, nID);
	}

	BOOL CreateSimpleReBar(DWORD dwStyle = ATL_SIMPLE_REBAR_STYLE, UINT nID = IDW_REBAR_TOP)
	{
		ATLASSERT(!::IsWindow(m_hWndToolBar));
		m_hWndToolBar = CreateSimpleReBarCtrl(m_hWnd, dwStyle, nID);
		return (m_hWndToolBar != NULL);
	}

	static BOOL AddSimpleReBarBandCtrl(HWND hWndReBar, HWND hWndBand, int nID = 0, LPTSTR lpstrTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		// make band id = control id
		if(nID == 0)
		{
			nID = ::GetWindowLongPtr(hWndBand, GWLP_ID);
		}

		BOOL bRet = baseClass::AddSimpleReBarBandCtrl(hWndReBar, hWndBand, nID, lpstrTitle, bNewRow, cxWidth, bFullWidthAlways);
		if(bRet)
		{
			CReBarCtrl rebar = hWndReBar;
			UINT uBand = rebar.GetBandCount() - 1;
			CReBarBandInfo rbBand(RBBIM_STYLE);
			rebar.GetBandInfo(uBand, rbBand);
			if(nID != ATL_IDW_COMMAND_BAR)
			{
				rbBand.fStyle |= RBBS_GRIPPERALWAYS;
			}
			rebar.SetBandInfo(uBand, rbBand);
		}
		return bRet;
	}

	BOOL AddSimpleReBarBand(HWND hWndBand, LPTSTR lpstrTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		ATLASSERT(::IsWindow(m_hWndToolBar));   // must be an existing rebar
		ATLASSERT(::IsWindow(hWndBand));        // must be created
		return AddSimpleReBarBandCtrl(m_hWndToolBar, hWndBand, 0, lpstrTitle, bNewRow, cxWidth, bFullWidthAlways);
	}

	void SetAppRegKey(UINT nStringResId)
	{
		m_strAppRegKey.LoadString(nStringResId);
	}
	void SetAppRegKey(LPCTSTR lpsz)
	{
		m_strAppRegKey = lpsz;
	}
	void GetAppRegKey(CString& str)
	{
		str = m_strAppRegKey;
	}

	UINT DockingReBarHitTest(CPoint pointScreen, CRect& rectReBar)
	{
		
		CPoint pointClient = pointScreen;
		this->ScreenToClient(&pointClient);
		HWND hwndChild = this->ChildWindowFromPoint(pointClient);
		if((hwndChild != NULL) && (hwndChild != m_hWnd))
		{
			UINT idChild = ::GetDlgCtrlID(hwndChild);
			if((idChild >= IDW_REBAR_FIRST) && (idChild <= IDW_REBAR_LAST))
			{
				::GetWindowRect(hwndChild, rectReBar);
				return idChild;
			}
		}

		// determine the dragged band's row height
		CReBarCtrl rebar = m_hWndDragSource;
		UINT uBand = rebar.IdToIndex(m_idDragBand);
		UINT cyRow = rebar.GetRowHeight(uBand);

		CRect rect;
		for(UINT id = IDW_REBAR_FIRST; id <= IDW_REBAR_LAST; id++)
		{
			HWND hwnd = GetDlgItem(id);
			if(hwnd != NULL)
			{
				CReBarCtrl rebar = hwnd;
				rebar.GetWindowRect(rect);
				switch(id)
				{
				case IDW_REBAR_TOP:
					if(rect.Height() == 0)
						rect.bottom += cyRow;
					break;
				case IDW_REBAR_BOTTOM:
					if(rect.Height() == 0)
						rect.top -= cyRow;
					break;
				case IDW_REBAR_LEFT:
					if(rect.Width() == 0)
						rect.right += cyRow;
					break;
				case IDW_REBAR_RIGHT:
					if(rect.Width() == 0)
						rect.left -= cyRow;
					break;
				}
				if(::PtInRect(rect, pointScreen))
				{
					rectReBar = rect;
					return id;
				}
			}//if(hwnd != NULL)
		}//for(UINT id = IDW_REBAR_FIRST; id <= IDW_REBAR_LAST; id++)
		rectReBar.SetRectEmpty();
		return 0;
	}

	SIZE CalcDragFloatSize(UINT idRebar, UINT idBand)
	{
		CSize sizeRet(0);
		if((idRebar < IDW_REBAR_FIRST) || (idRebar > IDW_REBAR_LAST))
		{
			ATLASSERT(FALSE);
			return sizeRet;
		}
		
		HWND hwnd = GetDlgItem(idRebar);
		ATLASSERT(hwnd != NULL);
		if(hwnd == NULL)
			return sizeRet;
		CReBarCtrl rebar = hwnd;
		UINT uBand = rebar.IdToIndex(idBand);
		ATLASSERT(uBand != (UINT)-1);
		if(uBand == (UINT)-1)
		{
			return sizeRet;
		}
		CReBarBandInfo band(RBBIM_CHILD | RBBIM_IDEALSIZE);
		rebar.GetBandInfo(uBand, band);
		CRect rectChild;
		::GetWindowRect(band.hwndChild, rectChild);
		int nBtnCount = ::SendMessage(band.hwndChild, TB_BUTTONCOUNT, 0, 0);
		if(nBtnCount > 0)
		{
			if((idRebar == IDW_REBAR_TOP) || (idRebar == IDW_REBAR_BOTTOM))
			{
				sizeRet.cx = (band.cxIdeal > 0) ? band.cxIdeal : rectChild.Width();
				sizeRet.cy = rectChild.Height();
			}
			else
			{
				CToolBarCtrl tb = band.hwndChild;
				CRect rectBtn;
				for(int i = 0; i < nBtnCount; i++)
				{
					tb.GetItemRect(i, rectBtn);
					sizeRet.cx += rectBtn.Width();
				}
				sizeRet.cy = rectBtn.Height();
			}
		}//if(nBtnCount > 0)
		else
		{
			sizeRet.SetSize(rectChild.Width(), rectChild.Height());
		}
		
		sizeRet.cx += ::GetSystemMetrics(SM_CXSIZEFRAME) * 2;
		sizeRet.cy += ::GetSystemMetrics(SM_CYSIZEFRAME) * 2 
			+ ::GetSystemMetrics(SM_CYSMCAPTION);
		return sizeRet;
	}		

	void SetBandOrientation(LPREBARBANDINFO lpBand, bool bVertical = true)
	{
		ATLASSERT(lpBand);
		ATLASSERT(::IsWindow(lpBand->hwndChild));
		
		int nBtnCount = ::SendMessage(lpBand->hwndChild, TB_BUTTONCOUNT, 0,0);
		if(nBtnCount <= 0)
			return;
		CToolBarCtrl toolbar = lpBand->hwndChild;
		CRect rect;
		if(bVertical)
		{
			toolbar.SetRows(nBtnCount, TRUE, rect);
			toolbar.GetItemRect(nBtnCount - 1, rect);
			lpBand->cx = lpBand->cxIdeal = rect.bottom;
			ShowToolBarText(toolbar, false);			
			int cxWidth = rect.right;
			if((toolbar.GetExtendedStyle() & TBSTYLE_EX_DRAWDDARROWS) != 0)
			{
				for(int i = 0; i < nBtnCount; i++)
				{
					toolbar.GetItemRect(i, rect);
					if(rect.right > cxWidth)
						cxWidth = rect.right;
				}
			}
			lpBand->cyMinChild = lpBand->cyIntegral = lpBand->cyChild = cxWidth;
			toolbar.GetItemRect(0, rect);
			lpBand->cxMinChild = rect.bottom;
		}
		else
		{
			toolbar.SetRows(nBtnCount, TRUE, rect);
			toolbar.GetItemRect(nBtnCount - 1, rect);
			lpBand->cyMaxChild = rect.bottom;
			ShowToolBarText(toolbar);
			toolbar.SetRows(1, TRUE, rect);
			toolbar.GetItemRect(nBtnCount - 1, rect);
			lpBand->cx = lpBand->cxIdeal = rect.right;
			lpBand->cyMinChild = lpBand->cyIntegral = lpBand->cyChild = rect.bottom;
			toolbar.GetItemRect(0, rect);
			lpBand->cxMinChild = rect.right;			
		}
	}	

	//if hWndDest is NULL, puts the band in a new FloatBarWindow, otherwise puts the band
	// in hWndDest which is presumed to be a rebar control.
	//if successful, returns the band's new parent, otherwise returns NULL
	HWND TransferBand(HWND hWndDest, HWND hWndSource, UINT idBand, 
		LPRECT lprcFloatBar = NULL, BOOL bNewRow = TRUE)
	{
		ATLASSERT(::IsWindow(hWndSource));

		CReBarCtrl rebar = hWndSource;
		CReBarBandInfo band(RBBIM_BACKGROUND | RBBIM_CHILD |RBBIM_CHILDSIZE | RBBIM_COLORS
					| RBBIM_IDEALSIZE | RBBIM_ID | RBBIM_IMAGE | RBBIM_LPARAM 
					| RBBIM_SIZE | RBBIM_STYLE | RBBIM_TEXT);
		UINT uBand = rebar.IdToIndex(idBand);
		band.cch = 32; // change this as needed
		CString strText;
		band.lpText = strText.GetBufferSetLength(band.cch);
		if((band.lpText == NULL) || !rebar.GetBandInfo(uBand, band)) 
		{
			return NULL;
		}
		strText.ReleaseBuffer();
		if(strText.IsEmpty())
		{
			band.fMask &= ~RBBIM_TEXT;
			band.lpText = NULL;
		}
		if(band.clrFore == ::GetSysColor(COLOR_BTNTEXT))
			band.clrFore = CLR_DEFAULT;
		if(band.clrBack == ::GetSysColor(COLOR_BTNFACE))
			band.clrBack = CLR_DEFAULT;
		rebar.ShowBand(uBand, FALSE);
		rebar.DeleteBand(uBand);
		::SetParent(band.hwndChild, m_hWnd);
		
		HWND hwndRet = NULL;
		if(hWndDest != NULL)
		{
			//dock band
			rebar = hWndDest;
			SetBandOrientation(band, (rebar.GetStyle() & CCS_VERT) != 0 ? true: false);	
			if(bNewRow)
				band.fStyle |= RBBS_BREAK;
			else
				band.fStyle &= ~RBBS_BREAK;
			hwndRet = rebar.InsertBand(-1, band) ? hWndDest : NULL;
		}
		else
		{
			//float band
			ATLASSERT(lprcFloatBar);

			SetBandOrientation(band, false);
			CFloatBarWindow *pFloatBar = new CFloatBarWindow;
			ATLASSERT(pFloatBar);
			if(!pFloatBar)
				return NULL;
			if(strText.IsEmpty())
			{
				CWindow wnd = band.hwndChild;
				wnd.GetWindowText(strText);
			}
			
			bool bToolBar = ::SendMessage(band.hwndChild, TB_BUTTONCOUNT, 0,0) > 0;
			HWND hwndFloatBar = pFloatBar->Create(m_hWnd, lprcFloatBar, (LPCTSTR)strText, 
				bToolBar ? FLOATBAR_NOCLIP_WINDOW_STYLE : FLOATBAR_CLIP_WINDOW_STYLE, 
				FLOATBAR_WINDOW_STYLE_EX);
			ATLASSERT(::IsWindow(hwndFloatBar));
			m_mapFloatBars.Add(hwndFloatBar, pFloatBar);
			pFloatBar->SetPrevBandOwnerId(rebar.GetDlgCtrlID());
			hwndRet =  pFloatBar->InsertBand(band) ? hwndFloatBar : NULL;
		}

		return hwndRet;
	}

	void DoBandDrag(CPoint pointCursor)
	{
		ATLASSERT(::IsWindow(m_hWndDragSource));

		bool bEraseLastDrag = !m_rectLastDrag.IsRectEmpty();

		// determine if mouse has moved far enough away 
		// from source Rebar to show a drag rect without leaving tracks
		if((m_idDragSource != 0) && !bEraseLastDrag)
		{	
			CReBarCtrl rebar = m_hWndDragSource;
			UINT uRowCount = rebar.GetRowCount();
			UINT cyLastRow = rebar.GetRowHeight(uRowCount - 1);
			UINT uBandCount = rebar.GetBandCount();
			UINT cyExtra = max(uBandCount - uRowCount, 1) * cyLastRow;
			CRect rect;
			rebar.GetWindowRect(rect);
			switch(m_idDragSource)
			{
			case IDW_REBAR_TOP:
				rect.InflateRect(0, cyLastRow, 0, cyLastRow + cyExtra);
				break;
			case IDW_REBAR_BOTTOM:
				rect.InflateRect(0, cyLastRow + cyExtra, 0, cyLastRow);
				break;
			case IDW_REBAR_LEFT:
				rect.InflateRect(cyLastRow, 0, cyLastRow + cyExtra, 0);
				break;
			case IDW_REBAR_RIGHT:
				rect.InflateRect(cyLastRow + cyExtra, 0, cyLastRow, 0);
				break;
			}
			if(rect.PtInRect(pointCursor))
			{
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				return;
			}
		}

		// see what rebar, if any, mouse is over
		CRect rectNewDrag;
		UINT idRebarTarget = DockingReBarHitTest(pointCursor, rectNewDrag);
		
		//if over source rebar, remove last drag rect
		if((idRebarTarget != 0) &&  (idRebarTarget == m_idDragSource))
		{
			if(bEraseLastDrag)
			{
				CWindowDC dc(NULL);
				dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
				m_rectLastDrag.SetRectEmpty();
			}
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			return;
		}

		// if over a rebar other than the source rebar, draw drag rect around it
		T* pT = static_cast<T*>(this);
		pT;
		if(idRebarTarget != 0)
		{
			if(pT->CanDockBand(m_idDragBand, idRebarTarget))
			{
				CWindowDC dc(NULL);
				dc.DrawDragRect(rectNewDrag, m_sizeDragBorder, 
					bEraseLastDrag ? (LPRECT)m_rectLastDrag: NULL , m_sizeDragBorder);
				m_rectLastDrag = rectNewDrag;
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
			else
			{
				CWindowDC dc(NULL);
				dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
				m_rectLastDrag.SetRectEmpty();
				::SetCursor(::LoadCursor(NULL, IDC_NO));
			}
			return;
		}

		if(!pT->CanFloatBand(m_idDragBand))
		{
			if(bEraseLastDrag)
			{
				CWindowDC dc(NULL);
				dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
				m_rectLastDrag.SetRectEmpty();
			}
			::SetCursor(::LoadCursor(NULL, IDC_NO));
			return;
		}
		
		//If we got this far, the drag rect is floating
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		if(m_sizeDragFloat == CSize(0))
		{ 
			m_sizeDragFloat = CalcDragFloatSize(m_idDragSource, m_idDragBand);
			if(m_sizeDragFloat == CSize(0))
			{
				if(bEraseLastDrag)
				{
					CWindowDC dc(NULL);
					dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
					m_rectLastDrag.SetRectEmpty();
				}
				return;
			}
		}
				
		rectNewDrag = CRect(CPoint(0,0), m_sizeDragFloat);
		rectNewDrag.OffsetRect(pointCursor);
		rectNewDrag.OffsetRect(m_sizeFloatOffset);
		
		//if new drag rect intersects source rebar, dont show a drag rect
		//as it could leave track marks
		if(m_idDragSource != 0)
		{
			CRect rect;
			::GetWindowRect(m_hWndDragSource, rect);
			if (rect.IntersectRect(rectNewDrag, rect))
			{
				if(bEraseLastDrag)
				{
					CWindowDC dc(NULL);
					dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
					m_rectLastDrag.SetRectEmpty();
				}
				return;
			}
		}
		
		//finally draw the new drag rect
		CWindowDC dc(NULL);
		dc.DrawDragRect(rectNewDrag, m_sizeDragBorder, 
			bEraseLastDrag ? (LPRECT)m_rectLastDrag : NULL, m_sizeDragBorder);
		m_rectLastDrag = rectNewDrag;
		return;
	}

	void DoBandDrop(CPoint pointCursor)
	{
		if(m_rectLastDrag.IsRectEmpty())
		{// band is still inside source rebar
			ClearBandDrag();
			return;
		}

		CRect rectDrop = m_rectLastDrag;
		if(true)
		{
			CWindowDC dc(NULL);
			dc.DrawDragRect(m_rectLastDrag, m_sizeDragBorder, NULL, m_sizeDragBorder);
			m_rectLastDrag.SetRectEmpty();
		}

		CRect rect;
		UINT idDestRebar = DockingReBarHitTest(pointCursor, rect);
		if(idDestRebar != 0)
		{
			if(idDestRebar == m_idDragSource)
			{
				ClearBandDrag();
				return;
			}

			HWND hwndDestRebar = GetDlgItem(idDestRebar);
			if(m_idDragSource != 0)
			{
				// band came from another docked rebar
				TransferBand(hwndDestRebar, m_hWndDragSource, m_idDragBand);	
			}
			else
			{	
				// band came from a floatbar
				CFloatBarWindow* pFloatBar = m_mapFloatBars.Lookup(m_hWndDragSource);
				ATLASSERT(pFloatBar != NULL);
				if(pFloatBar)
				{
					TransferBand(hwndDestRebar, m_hWndDragSource, m_idDragBand);
					m_mapFloatBars.Remove(m_hWndDragSource);
					pFloatBar->DestroyWindow();
					delete pFloatBar;
				}
			}
			ClearBandDrag();
			UpdateLayout();
			return;
		}//if(idDestRebar != 0)

		if(m_idDragSource != 0)
		{
			T* pT = static_cast<T*>(this);
			if(!pT->CanFloatBand(m_idDragBand))
			{
				ClearBandDrag();
				return;
			}
		
			TransferBand(NULL, m_hWndDragSource, m_idDragBand, rectDrop);
			UpdateLayout();
		}//if(m_idDragSource != 0)
		else
		{
			::SetWindowPos(m_hWndDragSource, NULL, rectDrop.left, 
				rectDrop.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		ClearBandDrag();
	}

	void ClearBandDrag()
	{
		m_hWndDragSource = NULL;
		m_idDragSource = m_idDragBand = 0;
		m_sizeDragFloat = m_sizeFloatOffset = CSize(0);
	}

	HWND GetBandChild(HWND hWndReBar, UINT nID)
	{
		ATLASSERT(::IsWindow(hWndReBar));
		CReBarCtrl rebar = hWndReBar;
		CReBarBandInfo band(RBBIM_CHILD);
		UINT uBand = rebar.IdToIndex(nID);
		rebar.GetBandInfo(uBand, band);
		return band.hwndChild;	
	}

	BOOL FindBand(UINT idBand, FindBandInfo& fbi)
	{
		for(UINT id = IDW_REBAR_FIRST; id <= IDW_REBAR_LAST; id++)
		{
			HWND hwnd = GetDlgItem(id);
			if(hwnd != NULL)
			{
				CReBarCtrl rebar = hwnd;
				UINT uBand = rebar.IdToIndex(idBand);
				if(uBand != (UINT)-1)
				{
					fbi.hwndParent = hwnd;
					fbi.idParent = id;
					fbi.uBand = uBand;
					return TRUE;
				}
			}
		}
		for(int i = 0; i < m_mapFloatBars.GetSize(); i++)
		{
			CReBarCtrl rebar = m_mapFloatBars.GetKeyAt(i);
			if(rebar.IdToIndex(idBand) == 0)
			{
				fbi.hwndParent = rebar;
				fbi.idParent = 0;
				fbi.uBand = 0;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL ShowBand(UINT idBand, BOOL fShow = TRUE)
	{
		BOOL bRet = FALSE;
		FindBandInfo fbi;
		if(FindBand(idBand, fbi))
		{
			CReBarCtrl rebar = fbi.hwndParent;
			bRet = rebar.ShowBand(fbi.uBand, fShow);
			if(bRet && (fbi.idParent == IDW_REBAR_TOP))
				UpdateLayout();
		}
		return bRet;
	}

	BOOL IsBandVisible(UINT idBand)
	{
		BOOL bRet = FALSE;
		FindBandInfo fbi;
		if(FindBand(idBand, fbi))
		{
			if(fbi.idParent != 0)
			{
				CReBarCtrl rebar = fbi.hwndParent;
				CReBarBandInfo band(RBBIM_STYLE);
				rebar.GetBandInfo(fbi.uBand, band);
				bRet = (band.fStyle & RBBS_HIDDEN) == 0;
			}
			else
				bRet = ::IsWindowVisible(fbi.hwndParent);
		}
		return bRet;
	}

	void ShowToolBarText(HWND hWndToolBar, bool bShow = true)
	{
		ATLASSERT(::IsWindow(hWndToolBar));
		CToolBarCtrl toolbar = hWndToolBar;
		if((toolbar.GetStyle() & TBSTYLE_LIST) != 0)
		{
			toolbar.SetButtonStructSize();
			TBBUTTON button;
			TBBUTTONINFO info;
			info.cbSize = sizeof(TBBUTTONINFO);
			info.dwMask = TBIF_STYLE;
			for(int i = 0; i < toolbar.GetButtonCount(); i++)
			{
				toolbar.GetButton(i, &button);
				toolbar.GetButtonInfo(button.idCommand, &info);
				CString str;
				if(bShow && (toolbar.GetButtonText(button.idCommand, str.GetBufferSetLength(32)) > 0))
					info.fsStyle |= TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT;
				else
					info.fsStyle &= ~(TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT);
				toolbar.SetButtonInfo(button.idCommand, &info);
			}//for(int i = 0; i < toolbar.GetButtonCount(); i++)
		}//if((toolbar.GetStyle() & TBSTYLE_LIST) != 0)
	}

	BOOL ResizeReBarBands(UINT idReBar)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		HWND hwndRebar = GetDlgItem(idReBar);
		if(hwndRebar == NULL)
			return FALSE;

		CReBarCtrl rebar = hwndRebar;
		bool bVert = (rebar.GetStyle() & CCS_VERT) != 0;
		for(UINT uBand = 0; uBand < rebar.GetBandCount(); uBand++)
		{
			CReBarBandInfo band(RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE);
			if(!rebar.GetBandInfo(uBand, band))
				return FALSE;
			
			int nBtnCount = ::SendMessage(band.hwndChild, TB_BUTTONCOUNT, 0 ,0);
			if(nBtnCount > 0)
			{
				CToolBarCtrl toolbar = band.hwndChild;
				CRect rect(0,0,0,0);
				toolbar.GetItemRect(0, rect);
				band.cxMinChild = bVert ? rect.bottom : rect.right;
				if(bVert && ((toolbar.GetStyle() & TBSTYLE_LIST) != 0))
				{
					band.cyMinChild = 0;
					for(int i = 0; i < nBtnCount; i++)
					{
						toolbar.GetItemRect(i, rect);
						if((UINT)rect.right > band.cyMinChild)
							band.cyMinChild = rect.right;
					}
					band.cxIdeal = rect.bottom;
				}
				else
				{
					toolbar.GetItemRect(nBtnCount - 1, rect);
					band.cyMinChild = bVert ? rect.right : rect.bottom;
					band.cxIdeal = bVert ? rect.bottom : rect.right;
				}
				band.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
			}
			else
			{
				CRect rect(0,0,0,0);
				::GetWindowRect(band.hwndChild, rect);
				band.cyMinChild = bVert ? rect.Width() : rect.Height();
				band.fMask = RBBIM_CHILDSIZE;
			}
			rebar.SetBandInfo(uBand, band);
		}//for(UINT uBand = 0; uBand < rebar.GetBandCount(); uBand++)
		return TRUE;
	}

	BOOL SaveLayout(LPCTSTR szRegKey, HKEY hkRootKey = HKEY_CURRENT_USER)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		CRegKey regKey;
		LONG lRet = regKey.Create(hkRootKey, szRegKey);
		CString strValueName; 

		if(lRet == ERROR_SUCCESS)
		{
			_ScreenMetrics sm;
			sm.cxFullScreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
			sm.cyFullScreen = ::GetSystemMetrics(SM_CYFULLSCREEN);
			strValueName = _T("ScreenMetrics");
			lRet = regKey.SetBinaryValue((LPCTSTR)strValueName,(LPBYTE)&sm, sizeof(sm));
		}	

		if(lRet == ERROR_SUCCESS)
		{
			WINDOWPLACEMENT wndpl;
			::ZeroMemory(&wndpl, sizeof(wndpl));
			wndpl.length = sizeof(wndpl);
			if (::GetWindowPlacement(m_hWnd, &wndpl))
			{
				strValueName = _T("WindowPlacement");
				lRet = regKey.SetBinaryValue((LPCTSTR)strValueName,(LPBYTE)&wndpl, sizeof(wndpl));
			}
			else
				lRet = ::GetLastError(); 
		}

		if(lRet == ERROR_SUCCESS)
		{
			_BandLayout bl;
			::ZeroMemory(&bl, sizeof(bl));
			DWORD dwBandCount = 0;
			for(UINT idRebar = IDW_REBAR_FIRST; idRebar <= IDW_REBAR_LAST; idRebar++)
			{
				HWND hwndRebar = GetDlgItem(idRebar);
				if(hwndRebar != NULL)
				{
					CReBarCtrl rebar = hwndRebar;
					CReBarBandInfo band(RBBIM_ID | RBBIM_STYLE | RBBIM_SIZE);
					for(UINT uBand = 0; uBand < rebar.GetBandCount(); uBand++)
					{
						rebar.GetBandInfo(uBand, band);
						bl.idBand = band.wID;
						bl.idParent = idRebar;
						bl.uBand = uBand;
						bl.cxWidth = band.cx;
						if(uBand > 0)
						{
							CRect rectPrev, rect;
							rebar.GetRect(uBand - 1, rectPrev);
							rebar.GetRect(uBand, rect);
							if((idRebar == IDW_REBAR_TOP) || (idRebar == IDW_REBAR_BOTTOM))
								bl.bNewRow = rect.left <= rectPrev.left;
							else
								bl.bNewRow = rect.top > rectPrev.top;
						}
						else 
							bl.bNewRow = FALSE;
						bl.bVisible = (band.fStyle & RBBS_HIDDEN) == 0;
						strValueName.Format(_T("BandLayout%d"), dwBandCount);
						lRet = regKey.SetBinaryValue((LPCTSTR)strValueName,(LPBYTE)&bl, sizeof(bl));
						dwBandCount++;
					}
					RECT rc;
					rebar.GetWindowRect(&rc);
					::OffsetRect(&rc, -rc.left, -rc.top);
					strValueName.Format(_T("ReBarRect%d"), idRebar - IDW_REBAR_FIRST);
					lRet = regKey.SetBinaryValue((LPCTSTR)strValueName,(LPBYTE)&rc, sizeof(rc));
				}
			}//for(UINT idRebar = IDW_REBAR_FIRST; idRebar <= IDW_REBAR_LAST; idRebar++)
			for(int i = 0; i < m_mapFloatBars.GetSize(); i++)
			{
				CFloatBarWindow *p = m_mapFloatBars.GetValueAt(i);
				if(p)
				{
					bl.idBand = p->m_band.wID;
					bl.idParent = 0;
					bl.uBand = 0;
					bl.uRows = (UINT)p->GetBandRows();
					bl.bVisible = p->IsWindowVisible();
					p->GetWindowRect(&bl.rcFloatBar);
					strValueName.Format(_T("BandLayout%d"), dwBandCount);
					lRet = regKey.SetBinaryValue((LPCTSTR)strValueName,(LPBYTE)&bl, sizeof(bl));
					dwBandCount++;
				}
			}//for(int i = 0; i < m_mapFloatBars.GetSize(); i++)

			//delete old band info in registry, if any.
			strValueName = _T("BandCount");
			DWORD dwOldBandCount = 0;
			regKey.QueryDWORDValue((LPCTSTR)strValueName, dwOldBandCount);
			lRet = regKey.SetDWORDValue(_T("BandCount"), dwBandCount);
			for(DWORD dw = dwBandCount; dw < dwOldBandCount; dw++)
			{
				strValueName.Format(_T("BandLayout%d"), dw);
				regKey.DeleteValue((LPCTSTR)strValueName);
			}
		}//if(lRet == ERROR_SUCCESS)

		return lRet == ERROR_SUCCESS;
	}

	BOOL RestoreLayout(LPCTSTR szRegKey, HKEY hkRootKey = HKEY_CURRENT_USER)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		CRegKey regKey;
		LONG lRet = regKey.Open(hkRootKey, szRegKey, KEY_READ);
		CString strValueName;

		if(lRet == ERROR_SUCCESS)
		{
			_ScreenMetrics sm = {0};
			strValueName = _T("ScreenMetrics");
			DWORD dwSize = sizeof(sm);
			lRet = regKey.QueryBinaryValue((LPCTSTR)strValueName, (LPBYTE)&sm, &dwSize);
			if (lRet == ERROR_SUCCESS)
			{
				ATLASSERT(dwSize == sizeof(sm));
				if (dwSize == sizeof(sm))
				{
					if((sm.cxFullScreen != ::GetSystemMetrics(SM_CXFULLSCREEN))
						|| (sm.cyFullScreen != ::GetSystemMetrics(SM_CYFULLSCREEN)))
						lRet = ERROR_NOT_SUPPORTED;
				}
				else
					lRet = ERROR_INVALID_DATA;
			}
		}

		if(lRet == ERROR_SUCCESS)
		{
			WINDOWPLACEMENT wndpl;
			::ZeroMemory(&wndpl, sizeof(wndpl));
			strValueName = _T("WindowPlacement");
			DWORD dwSize = sizeof(wndpl);
			lRet = regKey.QueryBinaryValue((LPCTSTR)strValueName, (LPBYTE)&wndpl, &dwSize);
			if (lRet == ERROR_SUCCESS)
			{
				ATLASSERT(dwSize == sizeof(wndpl));
				if (dwSize == sizeof(wndpl))
				{	
					RECT rcWorkArea;
					::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWorkArea, 0);

					//check normal size
					if ((wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left) 
						> (rcWorkArea.right - rcWorkArea.left))
						wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left
							+ (rcWorkArea.right - rcWorkArea.left) - 100;
					if ((wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top) 
						> (rcWorkArea.bottom - rcWorkArea.top))
						wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top
							+ (rcWorkArea.bottom - rcWorkArea.top) - 100;

					//check normal position
					if (wndpl.rcNormalPosition.left < rcWorkArea.left)
						::OffsetRect(&(wndpl.rcNormalPosition), rcWorkArea.left - 
							wndpl.rcNormalPosition.left, 0);
					if (wndpl.rcNormalPosition.top < rcWorkArea.top)
						::OffsetRect(&(wndpl.rcNormalPosition), 0, rcWorkArea.top - 
							wndpl.rcNormalPosition.top);
					if (wndpl.rcNormalPosition.right > rcWorkArea.right)
						::OffsetRect(&(wndpl.rcNormalPosition), rcWorkArea.right - 
							wndpl.rcNormalPosition.right, 0);
					if (wndpl.rcNormalPosition.bottom > rcWorkArea.bottom)
						::OffsetRect(&(wndpl.rcNormalPosition), 0, rcWorkArea.bottom - 
							wndpl.rcNormalPosition.bottom);

					// don't show minimized
					if (wndpl.showCmd == SW_SHOWMINIMIZED)
						wndpl.showCmd = SW_SHOWNORMAL;

					::SetWindowPlacement(m_hWnd, &wndpl);
					::ShowWindow(m_hWnd, wndpl.showCmd);
				}
				else
					lRet = ERROR_INVALID_DATA;
			}
		}

		if(lRet == ERROR_SUCCESS)
		{
			strValueName = _T("BandCount");
			DWORD dwBandCount = 0;
			lRet = regKey.QueryDWORDValue((LPCTSTR)strValueName, dwBandCount);
			if(lRet != ERROR_SUCCESS)
				return FALSE;

			CSimpleMap<UINT, LPARAM> mapBandWidths; // <idBand, LPARAM(cxWidth, bNewRow)>

			_BandLayout bl;
			::ZeroMemory(&bl, sizeof(bl));
			DWORD dwSize = sizeof(bl);
			DWORD dwBand = 0;
			while((lRet == ERROR_SUCCESS) && (dwBand < dwBandCount))
			{
				strValueName.Format(_T("BandLayout%d"), dwBand);
				lRet = regKey.QueryBinaryValue((LPCTSTR)strValueName,(LPBYTE)&bl, &dwSize);
				if(lRet == ERROR_SUCCESS)
				{
					FindBandInfo fbi;
					if(FindBand(bl.idBand, fbi))
					{
						HWND hwndParent = fbi.hwndParent;
						if(bl.idParent != fbi.idParent)
						{
							hwndParent = TransferBand(bl.idParent != 0 ? GetDlgItem(bl.idParent) : NULL, fbi.hwndParent, bl.idBand,
								&bl.rcFloatBar, bl.bNewRow);
						}
						if(hwndParent != NULL)
						{
							if(bl.idParent != 0)
							{
								// a docked band
								CReBarCtrl rebar = hwndParent;
								UINT uBandCount = rebar.GetBandCount();
								UINT uBand = rebar.IdToIndex(bl.idBand);
								bl.uBand = min(bl.uBand, uBandCount - 1);
								if(bl.uBand != uBand)
									rebar.MoveBand(uBand, bl.uBand);
							}
							else if(bl.uRows > 1)
							{
								CFloatBarWindow *p = m_mapFloatBars.Lookup(hwndParent);
								ATLASSERT(p);
								if(p)
								{
									p->SetBandRows(bl.uRows);
								}
							}
							if(!bl.bVisible)
							{
								T* pT = static_cast<T*>(this);
								if(pT->CanHideBand(bl.idBand))
								{
									CReBarCtrl rebar = hwndParent;
									rebar.ShowBand(bl.uBand, FALSE);
								}
							}
							mapBandWidths.Add(bl.idBand, MAKELPARAM(bl.cxWidth, bl.bNewRow));
						}//if(FindBand(bl.idBand, fbi))
					}//if(lRet == ERROR_SUCCESS)
				}
				dwBand++;
			}
			UpdateLayout();
			// resize rebars and docked bands to saved dims
			CReBarBandInfo band;
			for(UINT idRebar = IDW_REBAR_FIRST; idRebar <= IDW_REBAR_LAST; idRebar++)
			{
				HWND hwndRebar = GetDlgItem(idRebar);
				if(hwndRebar != NULL)
				{
					
					CReBarCtrl rebar = hwndRebar;
					RECT rc;
					strValueName.Format(_T("ReBarRect%d"), idRebar - IDW_REBAR_FIRST);
					DWORD dwSize = sizeof(rc);
					lRet = regKey.QueryBinaryValue((LPCTSTR)strValueName,(LPBYTE)&rc, &dwSize);
					if(lRet == ERROR_SUCCESS)
						rebar.SizeToRect(&rc);

					for(UINT uBand = 0; uBand < rebar.GetBandCount(); uBand++)
					{
						band.fMask = RBBIM_ID | RBBIM_STYLE;
						if(rebar.GetBandInfo(uBand, band))
						{
							LPARAM lParam = mapBandWidths.Lookup(band.wID);
							band.cx = LOWORD(lParam);
							if((BOOL)HIWORD(lParam))
								band.fStyle |= RBBS_BREAK;
							else
								band.fStyle &= ~RBBS_BREAK;
							band.fMask = RBBIM_SIZE | RBBIM_STYLE;
							rebar.SetBandInfo(uBand, band);
						}	
					}
					if(idRebar == IDW_REBAR_TOP)
						UpdateLayout();
				}
			}//for(UINT idRebar = IDW_REBAR_FIRST; idRebar <= IDW_REBARLAST; idRebar++)
		}

		return lRet == ERROR_SUCCESS;
	}

	BEGIN_MSG_MAP(thisClass)
		NOTIFY_CODE_HANDLER(RBN_BEGINDRAG, OnReBarBeginDrag)
		NOTIFY_CODE_HANDLER(RBN_ENDDRAG, OnReBarEndDrag)
		NOTIFY_HANDLER(IDW_REBAR_TOP, RBN_LAYOUTCHANGED, OnTopReBarLayoutChanged)
		MESSAGE_HANDLER(FLBM_NOTIFY, OnFloatBarNotify)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{ 
		for(int i = 0; i < m_mapFloatBars.GetSize(); i++)
		{
			CFloatBarWindow *p = m_mapFloatBars.GetValueAt(i);
			if(p)
			{
				delete p;
			}
		}
		m_mapFloatBars.RemoveAll();

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnFloatBarNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		UINT idBand = LOWORD(wParam);
		HWND hwndFloatBar = (HWND)lParam;
		switch(HIWORD(wParam))
		{
		case FLBN_BEGINDRAG:
			{	// start float bar drag
				m_hWndDragSource = hwndFloatBar;
				m_idDragSource = 0;
				CPoint pointCursor = ::GetMessagePos();
				CRect rectWnd;
				::GetWindowRect(m_hWndDragSource, rectWnd);
				m_sizeDragFloat = CSize(rectWnd.Width(), rectWnd.Height());
				m_sizeFloatOffset = CSize(rectWnd.left - pointCursor.x, rectWnd.top - pointCursor.y);
				m_idDragBand = idBand;
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				break;
			}
		case FLBN_DRAGMOVE:
			{
				CPoint point = ::GetMessagePos();
				DoBandDrag(point);
				break;
			}
		case FLBN_ENDDRAG:
			{
				CPoint point = ::GetMessagePos();
				DoBandDrop(point);
				break;
			}
		case FLBN_CLOSEBUTTON:
			{ // hide floatbar 
				T* pT = static_cast<T*>(this);
				if(pT->CanHideBand(idBand))
				{
					::ShowWindow(hwndFloatBar, SW_HIDE);
				}
				break;
			}
		case FLBN_CAPTIONDBLCLK:
			{
				//give band back to previous owner
				CFloatBarWindow	*p = m_mapFloatBars.Lookup(hwndFloatBar);
				ATLASSERT(p != NULL);
				if(p)
				{
					HWND hwndDestReBar = GetDlgItem(p->GetPrevBandOwnerId());
					TransferBand(hwndDestReBar, hwndFloatBar, idBand);
					m_mapFloatBars.Remove(hwndFloatBar);
					p->DestroyWindow();
					delete p;
					UpdateLayout();
				}
				break;
			}
		}//switch(lParam)
		return 0;
	}

	LRESULT OnReBarBeginDrag(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		if((idCtrl < IDW_REBAR_FIRST) || (idCtrl > IDW_REBAR_LAST))
		{
			return 0;
		}

		m_hWndDragSource = pnmh->hwndFrom;
		m_idDragSource = idCtrl;
		LPNMREBAR pnmrb = (LPNMREBAR)pnmh;
		m_idDragBand = pnmrb->wID;
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		return 0;
	}

	LRESULT OnReBarEndDrag(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		if((idCtrl >= IDW_REBAR_FIRST) && (idCtrl <= IDW_REBAR_LAST)
			&& (m_hWndDragSource != NULL))
		{
			DoBandDrop(CPoint(::GetMessagePos()));
		}

		return 0;
	}

	LRESULT OnTopReBarLayoutChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		UpdateLayout(TRUE);
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(!m_strAppRegKey.IsEmpty())
			SaveLayout((LPCTSTR)m_strAppRegKey);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		if(!m_bRestoredLayout && !m_strAppRegKey.IsEmpty())
		{
			LPWINDOWPOS pwp = (LPWINDOWPOS)lParam;
			if((pwp->flags & SWP_SHOWWINDOW) != 0)
			{
				m_bRestoredLayout = true;
				bHandled = RestoreLayout((LPCTSTR)m_strAppRegKey);
			}
		}
		return 0;
	}

	LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		this->SendMessageToDescendants(uMsg, wParam, lParam);
		this->DefWindowProc(uMsg, wParam, lParam);
		if(wParam == SPI_SETNONCLIENTMETRICS)
		{
			for(UINT idRebar = IDW_REBAR_FIRST; idRebar <= IDW_REBAR_LAST; idRebar++)
			{
				ResizeReBarBands(idRebar);
			}
			UpdateLayout();
		}
		return 0;
	}

//Overrides
	void UpdateLayout(BOOL bResizeBars = TRUE)
	{
		RECT rect = { 0 };
		GetClientRect(&rect);

		// position bars and offset their dimensions
		UpdateBarsPosition(rect, bResizeBars);

		// resize client window
		if(m_hWndClient != NULL)
		{
			::SetWindowPos(m_hWndClient, NULL, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void UpdateBarsPosition(RECT& rect, BOOL bResizeBars = TRUE)
	{
		// resize top rebar
		HWND hWndBar = GetDlgItem(IDW_REBAR_TOP); 
		if(( hWndBar!= NULL) && (((DWORD)::GetWindowLong(hWndBar, GWL_STYLE) & WS_VISIBLE)))
		{
			if(bResizeBars)
			{
				::SendMessage(hWndBar, WM_SIZE, 0, 0);
			}
			
			RECT rectTB = { 0 };
			::GetWindowRect(hWndBar, &rectTB);
			rect.top += rectTB.bottom - rectTB.top;	
		}

		// resize status bar
		if(m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
		{
			if(bResizeBars)
				::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
			RECT rectSB = { 0 };
			::GetWindowRect(m_hWndStatusBar, &rectSB);
			rect.bottom -= rectSB.bottom - rectSB.top;
		}

		//resize bottom rebar
		hWndBar = GetDlgItem(IDW_REBAR_BOTTOM); 
		if(( hWndBar!= NULL) && (((DWORD)::GetWindowLong(hWndBar, GWL_STYLE) & WS_VISIBLE)))
		{
			RECT rectTB = { 0 };
			if(bResizeBars)
			{
				::GetWindowRect(hWndBar, &rectTB);	
				::SetWindowPos(hWndBar, NULL, rect.left, rect.bottom - (rectTB.bottom - rectTB.top), 
					rect.right - rect.left, rectTB.bottom - rectTB.top, SWP_NOZORDER);
				if((rect.right - rect.left) > (rectTB.right - rectTB.left))
				{ //erase old border
					RECT rcTemp = {(rectTB.right - rectTB.left),0,rect.right - rect.left,
						rectTB.bottom - rectTB.top};
					::InvalidateRect(hWndBar, &rcTemp, TRUE);
					::UpdateWindow(hWndBar);
				}
			}
			else
			{
				::GetWindowRect(hWndBar, &rectTB);	
			}
				
			rect.bottom -= rectTB.bottom - rectTB.top;
		}

		//resize left rebar
		hWndBar = GetDlgItem(IDW_REBAR_LEFT); 
		if(( hWndBar!= NULL) && (((DWORD)::GetWindowLong(hWndBar, GWL_STYLE) & WS_VISIBLE)))
		{
			RECT rectTB = { 0 };
			if(bResizeBars)
			{
				::SendMessage(hWndBar, WM_SIZE, 0, 0);
				::GetWindowRect(hWndBar, &rectTB);	
				::SetWindowPos(hWndBar, NULL, rect.left, rect.top, 
					rectTB.right - rectTB.left, rect.bottom - rect.top, SWP_NOZORDER);
				if((rect.bottom - rect.top) > (rectTB.bottom - rectTB.top))
				{// erase old border
					RECT rcTemp = {0, (rectTB.bottom - rectTB.top - 4),
						rectTB.right - rectTB.left, rect.bottom - rect.top};
					::InvalidateRect(hWndBar, &rcTemp, TRUE);
					::UpdateWindow(hWndBar);
				}
			}
			else
			{
				::GetWindowRect(hWndBar, &rectTB);	
			}
				
			rect.left += rectTB.right - rectTB.left;
		}

		//resize right rebar
		hWndBar = GetDlgItem(IDW_REBAR_RIGHT); 
		if(( hWndBar!= NULL) && (((DWORD)::GetWindowLong(hWndBar, GWL_STYLE) & WS_VISIBLE)))
		{
			RECT rectTB = { 0 };
			if(bResizeBars)
			{
				::SendMessage(hWndBar, WM_SIZE, 0, 0);
				::GetWindowRect(hWndBar, &rectTB);	
				::SetWindowPos(hWndBar, NULL, rect.right - (rectTB.right - rectTB.left), 
					rect.top,  rectTB.right - rectTB.left, rect.bottom - rect.top, 
					SWP_NOZORDER);
				if((rect.bottom - rect.top) > (rectTB.bottom - rectTB.top))
				{ // erase old border
					RECT rcTemp = {0, (rectTB.bottom - rectTB.top - 4),
						rectTB.right - rectTB.left, rect.bottom - rect.top};
					::InvalidateRect(hWndBar, &rcTemp, TRUE);
					::UpdateWindow(hWndBar);
				}
			}
			else
			{
				::GetWindowRect(hWndBar, &rectTB);	
			}
				
			rect.right -= rectTB.right - rectTB.left;
		}
	}

#if (_WIN32_IE >= 0x0500)
	// PrepareChevronMenu is overridden to support vertically oriented rebars/bands.
	bool PrepareChevronMenu(_ChevronMenuInfo& cmi)
	{
		// get rebar and toolbar
		REBARBANDINFO rbbi = { 0 };
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_CHILD;
		BOOL bRet = (BOOL)::SendMessage(cmi.lpnm->hdr.hwndFrom, RB_GETBANDINFO, cmi.lpnm->uBand, (LPARAM)&rbbi);
		ATLASSERT(bRet);

		// assume the band is a toolbar
		ATL::CWindow wnd = rbbi.hwndChild;
		int nCount = (int)wnd.SendMessage(TB_BUTTONCOUNT);
		if(nCount <= 0)   // probably not a toolbar
			return false;

		// check if it's a command bar
		CMenuHandle menuCmdBar = (HMENU)wnd.SendMessage(CBRM_GETMENU);
		cmi.bCmdBar = (menuCmdBar.m_hMenu != NULL);

		// build a menu from hidden items
		CMenuHandle menu;
		bRet = menu.CreatePopupMenu();
		ATLASSERT(bRet);
		RECT rcClient = { 0 };
		bRet = wnd.GetClientRect(&rcClient);
		ATLASSERT(bRet);
		for(int i = 0; i < nCount; i++)
		{
			TBBUTTON tbb = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETBUTTON, i, (LPARAM)&tbb);
			ATLASSERT(bRet);
			// skip hidden buttons
			if((tbb.fsState & TBSTATE_HIDDEN) != 0)
				continue;
			RECT rcButton = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETITEMRECT, i, (LPARAM)&rcButton);
			ATLASSERT(bRet);
			bool bEnabled = ((tbb.fsState & TBSTATE_ENABLED) != 0);
			UINT idRebar = ::GetDlgCtrlID(cmi.lpnm->hdr.hwndFrom);
			bool bVertical = (idRebar == IDW_REBAR_LEFT) || (idRebar == IDW_REBAR_RIGHT); 
			if((!bVertical  && (rcButton.right > rcClient.right))
				|| bVertical  && (rcButton.bottom > rcClient.bottom))
			{
				if(tbb.fsStyle & BTNS_SEP)
				{
					if(menu.GetMenuItemCount() > 0)
						menu.AppendMenu(MF_SEPARATOR);
				}
				else if(cmi.bCmdBar)
				{
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					CMenuItemInfo mii;
					mii.fMask = MIIM_TYPE | MIIM_SUBMENU;
					mii.dwTypeData = szBuff;
					mii.cch = cchBuff;
					bRet = menuCmdBar.GetMenuItemInfo(i, TRUE, &mii);
					ATLASSERT(bRet);
					// Note: CmdBar currently supports only drop-down items
					ATLASSERT(::IsMenu(mii.hSubMenu));
					bRet = menu.AppendMenu(MF_STRING | MF_POPUP | (bEnabled ? MF_ENABLED : MF_GRAYED), (UINT_PTR)mii.hSubMenu, mii.dwTypeData);
					ATLASSERT(bRet);
				}
				else
				{
					// get button's text
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					LPTSTR lpstrText = szBuff;
					TBBUTTONINFO tbbi = { 0 };
					tbbi.cbSize = sizeof(TBBUTTONINFO);
					tbbi.dwMask = TBIF_TEXT;
					tbbi.pszText = szBuff;
					tbbi.cchText = cchBuff;
					if(wnd.SendMessage(TB_GETBUTTONINFO, tbb.idCommand, (LPARAM)&tbbi) == -1 || lstrlen(szBuff) == 0)
					{
						// no text for this button, try a resource string
						lpstrText = _T("");
#if (_ATL_VER >= 0x0700)
						int nRet = ::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), tbb.idCommand, szBuff, cchBuff);
#else //!(_ATL_VER >= 0x0700)
						int nRet = ::LoadString(_Module.GetResourceInstance(), tbb.idCommand, szBuff, cchBuff);
#endif //!(_ATL_VER >= 0x0700)
						for(int n = 0; n < nRet; n++)
						{
							if(szBuff[n] == _T('\n'))
							{
								lpstrText = &szBuff[n + 1];
								break;
							}
						}
					}
					bRet = menu.AppendMenu(MF_STRING | (bEnabled ? MF_ENABLED : MF_GRAYED), tbb.idCommand, lpstrText);
					ATLASSERT(bRet);
				}
			}
		}

		if(menu.GetMenuItemCount() == 0)   // no hidden buttons after all
		{
			menu.DestroyMenu();
			::MessageBeep((UINT)-1);
			return false;
		}

		cmi.hMenu = menu;
		return true;
	}
#endif //(_WIN32_IE >= 0x0500)

	BOOL CanFloatBand(UINT idBand)
	{
		return idBand == ATL_IDW_COMMAND_BAR ? FALSE: TRUE;
	}

	BOOL CanDockBand(UINT idBand, UINT idReBar)
	{
		return idBand == ATL_IDW_COMMAND_BAR ? FALSE: TRUE;	
	}

	BOOL CanHideBand(UINT idBand)
	{
		return idBand == ATL_IDW_COMMAND_BAR ? FALSE: TRUE;
	}

};

}; //namespace WTL

#endif // __DOCKBARFRAME_H__
