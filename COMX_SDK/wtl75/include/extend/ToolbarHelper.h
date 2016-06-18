#pragma once

// Define various toolbar button styles in case they are missing
#ifndef TBSTYLE_EX_MIXEDBUTTONS
#define TBSTYLE_EX_MIXEDBUTTONS			0x00000008
#endif

#ifndef BTNS_SHOWTEXT
#define BTNS_SHOWTEXT				0x0040
#endif

#define ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX	(ATL_SIMPLE_TOOLBAR_PANE_STYLE|TBSTYLE_LIST)

/////////////////////////////////////////////////////////////////////////////
// CComboBoxXPCtrl - The ComboBox control

#ifdef __ATLGDIX_H__

template< class T, class TBase = CComboBox, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CComboBoxXPImpl : 
	public CWindowImpl< T, TBase, TWinTraits >,
	public CMouseHover< T >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	CContainedWindowT<CEdit> m_ctrlEdit;
	bool m_fMouseOverEdit;
	COLORREF m_clrHighlight;
	COLORREF m_clrHighlightDark;
	COLORREF m_clrBorder;

	CComboBoxXPImpl() : 
	m_ctrlEdit(this, 1), 
		m_fMouseOverEdit(false)
	{
	}

	// Operations

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd==NULL);
		ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
		// Check class
		TCHAR szBuffer[16] = { 0 };
		if( ::GetClassName(hWnd, szBuffer, (sizeof(szBuffer)/sizeof(TCHAR))-1) ) {
			ATLASSERT(::lstrcmpi(szBuffer, TBase::GetWndClassName())==0);
		}
#endif
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		if( bRet ) _Init();
		return bRet;
	}

	// Implementation

	void _Init()
	{
		ATLASSERT(::IsWindow(m_hWnd));

		// No borders...
		ATLASSERT((GetStyle() & WS_BORDER)==0);
		ATLASSERT((GetExStyle() & WS_EX_CLIENTEDGE)==0);

		// Calculate XP-like colours
		CWindowDC dc(NULL);
		int nBitsPerPixel = dc.GetDeviceCaps(BITSPIXEL);
		m_clrBorder = ::GetSysColor(COLOR_HIGHLIGHT);
		if( nBitsPerPixel > 8 ) {
			m_clrHighlight = BlendRGB(m_clrBorder, ::GetSysColor(COLOR_WINDOW), 70);
			m_clrHighlightDark = BlendRGB(m_clrBorder, ::GetSysColor(COLOR_WINDOW), 60);
		}
		else {
			m_clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
			m_clrHighlightDark = ::GetSysColor(COLOR_HIGHLIGHT);
		}

		// Subclass child (edit) control if present
#if WINVER >= 0x0500
		COMBOBOXINFO cbi = { 0 };
		cbi.cbSize = sizeof(cbi);
		::GetComboBoxInfo(m_hWnd, &cbi);
		HWND hWndChild = cbi.hwndItem != cbi.hwndCombo ? cbi.hwndItem : NULL;
#else
		HWND hWndChild = GetWindow(GW_CHILD);
#endif
		if( hWndChild != NULL ) m_ctrlEdit.SubclassWindow(hWndChild);
	}

	// Message map and handlers

	BEGIN_MSG_MAP(CComboBoxXPImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_CLOSEUP, OnFocusChange);
		REFLECTED_COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnFocusChange);
		REFLECTED_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnFocusChange);
		CHAIN_MSG_MAP( CMouseHover< T > )
			DEFAULT_REFLECTION_HANDLER()
			ALT_MSG_MAP(1)
			MESSAGE_HANDLER(WM_MOUSEMOVE, OnEditMouseMove)
			MESSAGE_HANDLER(WM_MOUSELEAVE, OnEditMouseLeave)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
		{
			LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
			_Init();
			return lRes;
		}
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		{
			CPaintDC dc(m_hWnd);

			RECT rcClient = { 0 };
			GetClientRect(&rcClient);

			int cxButton = ::GetSystemMetrics(SM_CXHTHUMB);
			RECT rcButton = { rcClient.right - cxButton, rcClient.top, rcClient.right, rcClient.bottom };
			ValidateRect(&rcButton);

#if WINVER >= 0x0500
			COMBOBOXINFO cbi = { 0 };
			cbi.cbSize = sizeof(cbi);
			::GetComboBoxInfo(m_hWnd, &cbi);
			RECT rcItem = cbi.rcItem;
#else
			int cxBorder = ::GetSystemMetrics(SM_CXEDGE);
			RECT rcItem = { rcClient.left + cxBorder, rcClient.top + cxBorder, rcButton.left - cxBorder, rcClient.bottom - cxBorder };
#endif

			LRESULT lRes = DefWindowProc(WM_PRINTCLIENT, (WPARAM) (HDC) dc, PRF_CLIENT);

			COLORREF clrBorder = ::GetSysColor(COLOR_WINDOW);
			COLORREF clrBack = ::GetSysColor(COLOR_3DFACE);
			if( IsWindowEnabled() ) {
				if( m_fMouseOver || 
					m_fMouseOverEdit ||
					::GetFocus() == m_hWnd || 
					::GetParent(::GetFocus()) == m_hWnd )
				{
					clrBorder = m_clrBorder;
					clrBack = m_clrHighlight;
				}
				if( GetDroppedState() ) {
					clrBorder = m_clrBorder;
					clrBack = m_clrHighlightDark;
				}
			}

			// Draw the border
			CPen penBorder;
			penBorder.CreatePen(PS_SOLID, 1, clrBorder);
			HPEN hOldPen = dc.SelectPen(penBorder);
			HBRUSH hOldBrush = dc.SelectBrush(::GetSysColorBrush(COLOR_WINDOW));
			// HACK: Apparently the combo draws remnants of its borders as well
			//       so we'll erase it but need to exclude the text-area first.
			dc.ExcludeClipRect(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
			dc.Rectangle(&rcClient);
			dc.SelectClipRgn(NULL);

			// Paint the button
			CBrush brush;
			brush.CreateSolidBrush(clrBack);
			dc.SelectBrush(brush);
			dc.Rectangle(&rcButton);

			// Draw dropdown arrow
			// Need this because all the OEM bitmaps render grey backgrounds...
			RECT rcArrow = { rcButton.left + 5, rcButton.top + 8, rcButton.right - 5, rcButton.top + 11 };
			POINT points[3] = 
			{
				{ rcArrow.left, rcArrow.top },
				{ rcArrow.right, rcArrow.top },
				{ rcArrow.left + ((rcArrow.right - rcArrow.left) / 2), rcArrow.bottom }
			};
			int iFillMode = dc.SetPolyFillMode(WINDING);
			dc.SelectStockPen(BLACK_PEN);
			dc.SelectStockBrush(BLACK_BRUSH);
			dc.Polygon(points, 3);
			dc.SetPolyFillMode(iFillMode);

			dc.SelectPen(hOldPen);
			dc.SelectBrush(hOldBrush);

			return lRes;
		}
		LRESULT OnFocusChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		{
			LRESULT lRes = DefWindowProc();
			Invalidate();
			return lRes;
		}

		// Edit control

		LRESULT OnEditMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
		{
			if( !m_fMouseOverEdit )   {
				m_fMouseOverEdit = true;
				::InvalidateRect(m_hWnd, NULL, FALSE);
				::UpdateWindow(m_hWnd);
				// Let us know when the mouse leaves
				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = m_ctrlEdit;
				_TrackMouseEvent(&tme);
			}
			bHandled = FALSE;
			return 0;
		}
		LRESULT OnEditMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
		{
			if( m_fMouseOverEdit ) {
				m_fMouseOverEdit = false;
				::InvalidateRect(m_hWnd, NULL, FALSE);
				::UpdateWindow(m_hWnd);
			}
			bHandled = FALSE;
			return 0;
		}
};

class CComboBoxXPCtrl : public CComboBoxXPImpl<CComboBoxXPCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_ComboBoxXP"), GetWndClassName())  
};


#endif // __ATLGDIX_H__

/// Class used to expost useful toolbar functionality to a WTL CFrameWnd-derived class
template <class T>
class CToolBarHelper
{
private:
	/// Wrapper class for the Win32 TBBUTTONINFO structure.
	class CTBButtonInfo : public TBBUTTONINFO
	{
	public:
		/// Constructor
		CTBButtonInfo(DWORD dwInitialMask = 0)
		{
			memset(this, 0, sizeof(TBBUTTONINFO));
			cbSize = sizeof(TBBUTTONINFO);
			dwMask = dwInitialMask;
		}
	};

	/// Wrapper class for the Win32 TBBUTTON structure.
	class CTBButton : public TBBUTTON
	{
	public:
		/// Constructor
		CTBButton()
		{
			memset(this, 0, sizeof(TBBUTTON));
		}
	};
private:
	CFont m_fontCombo;			///< Font to use for comboboxes
	CSimpleMap<UINT, UINT> m_mapMenu;	///< Map of command IDs -> menu IDs
public:
	/// Message map
	BEGIN_MSG_MAP(CToolBarHelper<T>)
		COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnSelChangeToolBarCombo)
		NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolbarDropDown)
	END_MSG_MAP()

	/// Modify a toolbar button to have a drop-down button
	void AddToolBarDropDownMenu(HWND hWndToolBar, UINT nButtonID, UINT nMenuID)
	{
		ATLASSERT(hWndToolBar != NULL);
		ATLASSERT(nButtonID > 0);
		// Use built-in WTL toolbar wrapper class
		CToolBarCtrl toolbar(hWndToolBar);
		// Add the necessary style bit (TBSTYLE_EX_DRAWDDARROWS) if
		// not already present
		if ((toolbar.GetExtendedStyle() & TBSTYLE_EX_DRAWDDARROWS) != TBSTYLE_EX_DRAWDDARROWS)
			toolbar.SetExtendedStyle(toolbar.GetExtendedStyle() | TBSTYLE_EX_DRAWDDARROWS);
		// Get existing button style
		CTBButtonInfo tbi(TBIF_STYLE);
		if (toolbar.GetButtonInfo(nButtonID, &tbi) != -1)
		{
			// Modify the button
			tbi.fsStyle |= TBSTYLE_DROPDOWN;
			toolbar.SetButtonInfo(nButtonID, &tbi);
			// We need to remember that this menu ID is associated with the button ID
			// so use a basic map for this.
			m_mapMenu.Add(nButtonID, nMenuID);
		}
	}

	LRESULT OnToolbarDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		// Get the toolbar data
		NMTOOLBAR* ptb = reinterpret_cast<NMTOOLBAR*>(pnmh);
		// See if the button ID has an asscociated menu ID
		UINT nMenuID = m_mapMenu.Lookup(ptb->iItem);
		if (nMenuID)
		{
			// Get the toolbar control
			CToolBarCtrl toolbar(pnmh->hwndFrom);
			// Get the button rect
			CRect rect;
			toolbar.GetItemRect(toolbar.CommandToIndex(ptb->iItem), &rect);
			// Create a point
			CPoint pt(rect.left, rect.bottom);
			// Map the points
			toolbar.MapWindowPoints(HWND_DESKTOP, &pt, 1);
			// Load the menu
			CMenu menu;
			if (menu.LoadMenu(nMenuID))
			{
				CMenuHandle menuPopup = menu.GetSubMenu(0);
				ATLASSERT(menuPopup != NULL);
				T* pT = static_cast<T*>(this);
				// Allow the menu items to be initialised (for example,
				// new items could be added here for example)
				pT->PrepareToolBarMenu(nMenuID, menuPopup);
				// Display the menu				
				// Using command bar TrackPopupMenu method means menu icons are displayed
				pT->m_CmdBar.TrackPopupMenu(menuPopup, TPM_RIGHTBUTTON|TPM_VERTICAL, pt.x, pt.y);
			}
		}
		return 0;
	}

	/// Override this Allow the menu items to be enabled/checked/etc.
	virtual void PrepareToolBarMenu(UINT /*nMenuID*/, HMENU /*hMenu*/)
	{
	}

	/// Add text to a toolbar button
	void AddToolbarButtonText(HWND hWndToolBar, UINT nID, LPCTSTR lpsz)
	{
		// Use built-in WTL toolbar wrapper class
		CToolBarCtrl toolbar(hWndToolBar);
		// Set extended style
		if ((toolbar.GetExtendedStyle() & TBSTYLE_EX_MIXEDBUTTONS) != TBSTYLE_EX_MIXEDBUTTONS)
			toolbar.SetExtendedStyle(toolbar.GetExtendedStyle() | TBSTYLE_EX_MIXEDBUTTONS);
		// Get the button index
		int nIndex = toolbar.CommandToIndex(nID);
		CTBButton tb;
		toolbar.GetButton(nIndex, &tb);
		int nStringID = toolbar.AddStrings(lpsz);
		// Alter the button style
		tb.iString = nStringID;
		tb.fsStyle |= TBSTYLE_AUTOSIZE|BTNS_SHOWTEXT;
		// Delete and re-insert the button
		toolbar.DeleteButton(nIndex);
		toolbar.InsertButton(nIndex, &tb);
	}

	/// Add resource string to a toolbar button
	void AddToolbarButtonText(HWND hWndToolBar, UINT nID, UINT nStringID)
	{
		CString str;
		if (str.LoadString(nStringID))
			AddToolbarButtonText(hWndToolBar, nID, str);
	}

	/// Add text to a toolbar button (using tooltip text)
	void AddToolbarButtonText(HWND hWndToolBar, UINT nID)
	{
		TCHAR sz[256];
		if (AtlLoadString(nID, sz, 256) > 0)
		{
			// Add the text following the '\n'
			TCHAR* psz = _tcsrchr(sz, '\n');
			if (psz != NULL)
			{
				// Skip to first character of the tooltip
				psz++;
				// The tooltip text may include the accelerator, i.e.
				//	Open (Ctrl+O)
				// So look for an open brace
				TCHAR* pBrace = _tcschr(psz, '(');
				if (pBrace != NULL)
					*(pBrace - 1) = '\0';
				AddToolbarButtonText(hWndToolBar, nID, psz);
			}
		}
	}	

	/// Create a combobox on a toolbar
	HWND CreateToolbarComboBox( HWND hWndToolBar, UINT nID, UINT nWidth = 16, UINT nHeight = 16,
				   DWORD dwComboStyle = CBS_DROPDOWNLIST)
	{
		T* pT = static_cast<T*>(this);
		// Use built-in WTL toolbar wrapper class
		CToolBarCtrl toolbar(hWndToolBar);
		// Get the size of the combobox font
		CreateComboFont();
		CSize sizeFont = GetComboFontSize();
		// Compute the width and height
		UINT cx = (nWidth + 8) * sizeFont.cx;
		UINT cy = nHeight * sizeFont.cy;
		// Set the button width
		CTBButtonInfo tbi(TBIF_SIZE|TBIF_STATE|TBIF_STYLE);
		// Make sure the underlying button is disabled
		tbi.fsState = 0;
		// BTNS_SHOWTEXT will allow the button size to be altered
		tbi.fsStyle = BTNS_SHOWTEXT;
		tbi.cx = static_cast<WORD>(cx);
		toolbar.SetButtonInfo(nID, &tbi);
		// Get the index of the toolbar button
		int nIndex = toolbar.CommandToIndex(nID);
		// Get the button rect
		CRect rc;
		toolbar.GetItemRect(nIndex, rc);
		rc.bottom = cy;
		// Create the combobox
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBS_HASSTRINGS|CBS_OWNERDRAWFIXED|dwComboStyle;
		CComboBox combo;
		combo.Create(pT->m_hWnd, rc, NULL, dwStyle, 0, nID);
		combo.SetFont(m_fontCombo);
		combo.SetParent(toolbar);
		// The combobox might not be centred vertically, and we won't know the
		// height until it has been created.  Get the size now and see if it
		// needs to be moved.
		CRect rectToolBar;
		CRect rectCombo;
		toolbar.GetClientRect(&rectToolBar);		
		combo.GetWindowRect(rectCombo);
		// Get the different between the heights of the toolbar and
		// the combobox
		int nDiff = rectToolBar.Height() - rectCombo.Height();
		// If there is a difference, then move the combobox
		if (nDiff > 1)
		{
			toolbar.ScreenToClient(&rectCombo);
			combo.MoveWindow(rectCombo.left, rc.top + (nDiff / 2), rectCombo.Width(), rectCombo.Height());
		}
		return combo;
	}

	/// Create the font to use for comboboxes
	void CreateComboFont()
	{
		if (m_fontCombo == NULL)
		{
			NONCLIENTMETRICS ncm;
			ncm.cbSize = sizeof(NONCLIENTMETRICS);
			::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
			// Create menu font
			m_fontCombo.CreateFontIndirect(&ncm.lfMenuFont);
			ATLASSERT(m_fontCombo != NULL);
		}
	}

	/// Get the size of the default GUI font
	CSize GetComboFontSize()
	{
		ATLASSERT(m_fontCombo != NULL);
		// We need a temporary DC
		const T* pT = static_cast<const T*>(this);
		CClientDC dc(pT->m_hWnd);
		// Select in the menu font
		CFontHandle fontOld = dc.SelectFont(m_fontCombo);
		// Get the font size
		TEXTMETRIC tm;
		dc.GetTextMetrics(&tm);
		// Done with the font
		dc.SelectFont(fontOld);
		// Return the width and height
		return CSize(tm.tmAveCharWidth, tm.tmHeight + tm.tmExternalLeading);
	}

	LRESULT OnSelChangeToolBarCombo(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		// Get the newly selected item index
		CComboBox combo(hWndCtl);
		int nSel = combo.GetCurSel();
		// Get the item text
		CString strItemText;
		combo.GetLBText(nSel, strItemText);
		// Get the item data
		DWORD dwItemData = combo.GetItemData(nSel);
		// Call special function to handle the selection change
		pT->OnToolBarCombo(combo, wID, nSel, strItemText, dwItemData);
		// Set focus to the main window
		pT->SetFocus();
		return TRUE;
	}
};