#if !defined(AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_)
#define AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// ChoiceBar - A Choice Bar control
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// Add the following macro to the parent's message map:
//   REFLECT_NOTIFICATIONS()
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error ChoiceBar.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error ChoiceBar.h requires atlctrls.h to be included first
#endif

#include "resource.h"

#define CHBN_FIRST               (0U-1880U)       // choice bar
#define CHBN_LAST                (0U-1899U)

#define CHBN_INITIALIZE          (CHBN_FIRST - 0)
#define CHBN_BUTTONCLICK         (CHBN_FIRST - 1)
#define CHBN_ITEMSELECTING       (CHBN_FIRST - 2)
#define CHBN_ITEMSELECTED        (CHBN_FIRST - 3)
#define CHBN_CLOSEUP             (CHBN_FIRST - 4)
#define CHBN_CUSTOMIZE           (CHBN_FIRST - 5)

// Defined in Platform SDK on > Win5
#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT        0x0040
#endif

typedef struct tagNMCHOICEBAR
{
   NMHDR hdr;
   int   iItem;
} NMCHOICEBAR, FAR* LPNMCHOICEBAR;


#define CHB_EX_DEFAULT   0x00000001  // Bar has default button
#define CHB_EX_CUSTOM    0x00000002  // Bar has custom button
#define CHB_EX_TOOLTIP   0x00000004  // Tooltips for buttons enabled
#define CHB_EX_FLOAT     0x00000008  // Bar is a floating window
#define CHB_EX_FLAT      0x00000010  // Flat button look

#define CHB_ITEM_DEFAULT (int)0x4000
#define CHB_ITEM_CUSTOM  (int)0x4001

template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CChoiceBarCtrl : 
   public CWindowImpl< T, TBase, TWinTraits >
{
public:
   int   m_nButtons;    // Number of buttons
   int   m_iCurFocus;   // The hot-tracked button
   int   m_iCurSel;     // The selected button (not including DEFAULT and CUSTOM button)
   int   m_iCurBtnSel;  // The pressed button (all buttons included)
   DWORD m_dwExtStyle;  // Extended style
   //
   SIZE  m_sizeButton;  // Size of buttons
   SIZE  m_sizeMargin;  // Gap between button border and ownerdraw area
   SIZE  m_sizePadding; // Padding between buttons
   SIZE  m_sizeMatrix;  // Rows and columns defintion
   //
   CSimpleArray<RECT> m_rcButtons;
   RECT m_rcDefaultButton;
   RECT m_rcCustomButton;
   CFont m_font;
   bool m_bMouseTracked;
   CToolTipCtrl m_tip;
   HIMAGELIST m_hImageList;
   CChoiceBarCtrl()
   {
      m_dwExtStyle = 0;
      m_nButtons = 1;
      m_sizeMatrix.cx = m_sizeMatrix.cy = 1;
      m_sizeButton.cx = m_sizeButton.cy = 22;
      m_sizeMargin.cx = m_sizeMargin.cy = 3;
      m_sizePadding.cx = m_sizePadding.cy = 1;
   }

   BEGIN_MSG_MAP(CChoiceBarCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
      MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnToolTipDispInfo);
   END_MSG_MAP()

   // Operations

   int SetCurSel(int iIndex)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      int iOldSel = m_iCurSel;
      _SelectButton(iIndex, false);
      return iOldSel;
   }

   int GetCurSel() const
   {
      return m_iCurSel;
   }

   void SetExtStyle(DWORD dwStyle)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_dwExtStyle = dwStyle;
      // Create tooltip if nessecary
      if( m_dwExtStyle & CHB_EX_TOOLTIP && !m_tip.IsWindow() ) {
         m_tip.Create(m_hWnd);
         ATLASSERT(m_tip.IsWindow());
         m_tip.Activate(TRUE);
      }
      // Resize control
      _AutoSize();
   }

   DWORD GetExtStyle() const
   {
      return m_dwExtStyle;
   }

   void SetButtonCount(int nButtons)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(nButtons>0);
      if( nButtons<=0 ) m_nButtons = 1;
      m_nButtons = nButtons;
      m_sizeMatrix.cy = (m_nButtons / m_sizeMatrix.cx) + (((m_sizeMatrix.cx % m_nButtons)!=0) & 1);
      // Recreate tooltip buttons
      if( m_dwExtStyle & CHB_EX_TOOLTIP ) {
         TOOLINFO ti = { 0 };
         ti.cbSize = sizeof(ti);
         ti.hwnd = m_hWnd;
         int nCount = m_tip.GetToolCount();
         while( nCount-- >=0 ) {
            ti.uId = nCount;
            m_tip.DelTool(&ti);
         }
         RECT rc = { 0,0,0,0 };
         nCount = nButtons;
         while( nCount-- >=0 ) {
            ti.uId = nCount;
            ti.uFlags = TTF_SUBCLASS;
            ti.rect = rc;
            ti.lpszText = LPSTR_TEXTCALLBACK;
            m_tip.AddTool(&ti);
         }
      }
      _AutoSize();
   }

   int GetButtonCount() const
   {
      return m_nButtons;
   }

   void SetButtonSize(LONG cx, LONG cy)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(cx>0 && cy>0);
      m_sizeButton.cx = cx;
      m_sizeButton.cy = cy;
      _AutoSize();
   }

   void SetPadding(LONG cx, LONG cy)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(cx>=0 && cy>=0);
      m_sizePadding.cx = cx;
      m_sizePadding.cy = cy;
      _AutoSize();
   }

   void SetColumns(UINT nCols)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(nCols!=0);
      m_sizeMatrix.cx = nCols;
      m_sizeMatrix.cy = (m_nButtons / nCols) + (((nCols % m_nButtons)!=0) & 1);
      _AutoSize();
   }

   UINT GetColumns() const
   {
      return m_sizeMatrix.cx;
   }

   int HitTest(POINT pt) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      for( int i=0; i<m_nButtons; i++ ) {
         if( ::PtInRect(&m_rcButtons[i], pt) ) return i;
      }
      if( ::PtInRect(&m_rcDefaultButton, pt) ) return CHB_ITEM_DEFAULT;
      if( ::PtInRect(&m_rcCustomButton, pt) ) return CHB_ITEM_CUSTOM;
      return -1;
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ModifyStyle(0, SS_NOTIFY); // We need this for mouse-clicks

      m_iCurFocus = m_iCurSel = m_iCurBtnSel = -1;
      m_bMouseTracked = false;

      if( !m_font.IsNull() ) m_font.DeleteObject();
      NONCLIENTMETRICS ncm;
      ncm.cbSize = sizeof(ncm);
      ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
      ncm.lfMenuFont.lfHeight = 13; // To match the button size
      m_font.CreateFontIndirect(&ncm.lfMenuFont);

      NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_INITIALIZE };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
   }

   void _AutoSize()
   {
      T* pT = static_cast<T*>(this);
      pT->UpdateLayout();
   }

   void _InvalidateRect(int iIndex)
   {
      if( iIndex>=0 && iIndex<m_nButtons ) {
         InvalidateRect(&m_rcButtons[iIndex]);
      }
      else if( (iIndex==CHB_ITEM_DEFAULT) && (m_dwExtStyle & CHB_EX_DEFAULT) ) {
         InvalidateRect(&m_rcDefaultButton);
      }
      else if( (iIndex==CHB_ITEM_CUSTOM) && (m_dwExtStyle & CHB_EX_CUSTOM) ) {
         InvalidateRect(&m_rcCustomButton);
      }
   }

   void _DrawButton(HDC hdc, RECT &rc, int i, DRAWITEMSTRUCT &dis) const
   {
      bool bSelected = (i==m_iCurSel) || (i==m_iCurBtnSel);
      bool bFocus = i==m_iCurFocus;
      // Draw borders
      if( m_dwExtStyle & CHB_EX_FLAT ) {
		CDCHandle dc(hdc);
		if( bSelected) 
		{
			
			COLORREF clr = RGB(245, 245, 225);
			CBrush brush;
			CPen pen;
			brush.CreateSolidBrush(clr);

			pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
			HPEN hOldPen = dc.SelectPen(pen);
			HBRUSH hOldBrush = dc.SelectBrush(brush);
			CRect rcItemSub = rc;
			::InflateRect(&rcItemSub, -1, -1);
			dc.Rectangle(&rcItemSub);

			dc.SelectBrush(hOldBrush);
			dc.SelectPen(hOldPen);
		 }
		 else if ( bFocus ) 
		 {
			 COLORREF clr = ::GetSysColor(COLOR_WINDOW);
			 CBrush brush;
			 CPen pen;
			 brush.CreateSolidBrush(clr);

			 pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
			 HPEN hOldPen = dc.SelectPen(pen);
			 HBRUSH hOldBrush = dc.SelectBrush(brush);
			 CRect rcItemSub = rc;
			 ::InflateRect(&rcItemSub, -1, -1);
			 dc.Rectangle(&rcItemSub);

			 dc.SelectBrush(hOldBrush);
			 dc.SelectPen(hOldPen);
		 }
		 
      }
      else {
         UINT iFlags = 0;
         if( bSelected ) iFlags = EDGE_SUNKEN;
         else if( bFocus ) iFlags = EDGE_RAISED;
         if( iFlags!=0 ) ::DrawEdge(hdc, &rc, iFlags, BF_RECT);
/*
         UINT iFlags = DFCS_BUTTONPUSH | DFCS_FLAT;
         if( i==m_iCurSel ) iFlags |= DFCS_PUSHED;
         ::DrawFrameControl(hdc, &rc, DFC_BUTTON, iFlags);
*/
      }
      // Shrink rectangle so it doesn't include borders
      ::InflateRect(&rc, -m_sizeMargin.cx, -m_sizeMargin.cy);
      // Update DRAWITEMSTRUCT
      dis.itemID = i;
      dis.rcItem = rc;
      dis.itemAction = ODA_SELECT|ODA_FOCUS;
      dis.itemState = 0;
      if( i==CHB_ITEM_DEFAULT ) dis.itemState |= ODS_DEFAULT;
      if( bSelected ) dis.itemState |= ODS_SELECTED;
      if( bFocus ) dis.itemState |= ODS_HOTLIGHT;
   }

   void _SelectButton(int iIndex, bool bPostProcess=true)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(::IsWindow(GetParent()));
      // Custom button selected?
      if( iIndex==CHB_ITEM_CUSTOM ) {
         NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_CUSTOMIZE };
         ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
         return;
      }
      // Ask parent
      NMCHOICEBAR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_ITEMSELECTING, iIndex };
      if( ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh)!=0 ) return;;
      // Update buttons
      _InvalidateRect(iIndex);
      if( iIndex>=0 && iIndex<m_nButtons ) {
         _InvalidateRect(m_iCurBtnSel);
         _InvalidateRect(m_iCurSel);
         _InvalidateRect(CHB_ITEM_DEFAULT); // The Default button is likely to reflect the selected button
         m_iCurSel = m_iCurBtnSel = iIndex;
      }
      // Notify parent
      nmh.hdr.code = CHBN_ITEMSELECTED;
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
      if( bPostProcess ) {
         // Destroy bar?
         if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
      }
   }

   UINT _GetDlgCtrlID() const
   {
      // HACK: WTL messaging requires a control ID
      //       and Window Identifiers are not officially supported by
      //       top-level windows. We need to fake one.
      UINT ID = GetDlgCtrlID();
      if( ID==0 ) ID = 1234; 
      return ID;
   }

   void LoadImage(UINT uImageID, int nBtnWidth)
   {
	   m_hImageList = GenerateImageListFromID(uImageID, nBtnWidth);
   }
   // Message Handlers

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
      _Init();
      return lRes;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_CLOSEUP };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Click outside a bar and it closes...
      if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
      return 0;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc((HDC)wParam);
      RECT rc;
      GetClientRect(&rc);
	  CBrush brush;
	  COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
	  brush.CreateSolidBrush(clr);
      dc.FillRect(&rc, brush);
      return 1;
   }

   LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      return DefWindowProc(uMsg, wParam, lParam) | DLGC_WANTARROWS;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      T* pT = static_cast<T*>(this);
      if( wParam != NULL ) {
         pT->DoPaint((HDC)wParam);
      }
      else {
         PAINTSTRUCT m_ps;
         HDC hDC = ::BeginPaint(m_hWnd, &m_ps);
         pT->DoPaint(hDC);
      }
      return 0;
   }

   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iIndex==-1 ) return 0;
      // Update button to show as pressed
      _InvalidateRect(iIndex);
      _InvalidateRect(m_iCurBtnSel);
      m_iCurBtnSel = iIndex;
      // Capture the mouse so we can be sure to get WM_LBUTTONUP
      SetCapture();
      return 0;
   }

   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( ::GetCapture()==m_hWnd ) ReleaseCapture();

      int iCurBtnSel = m_iCurBtnSel;
      _InvalidateRect(m_iCurBtnSel);
      m_iCurBtnSel = -1;
      
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iCurBtnSel==iIndex ) {
         // Notify parent
         NMCHOICEBAR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_BUTTONCLICK, iIndex };
         ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
         // Select item
         _SelectButton(iIndex);
      }
      return 0;
   }

   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( ::GetCapture()==m_hWnd ) return 0; // Not as long as we are tracking

      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iIndex != m_iCurFocus ) {
         _InvalidateRect(iIndex);
         _InvalidateRect(m_iCurFocus);
         m_iCurFocus = iIndex;
      }

      if( !m_bMouseTracked ) {
         TRACKMOUSEEVENT tme = { 0 };
         tme.cbSize = sizeof(tme);
         tme.hwndTrack = m_hWnd;
         tme.dwFlags = TME_LEAVE;
         tme.dwHoverTime = HOVER_DEFAULT;
         _TrackMouseEvent(&tme);
         m_bMouseTracked = true;
      }
      return 0;
   }

   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_bMouseTracked = false;
      _InvalidateRect(m_iCurFocus);
      m_iCurFocus = -1;
      return 0;
   }

   LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      switch( wParam ) {
      case VK_RETURN:
      case VK_SPACE:
         _SelectButton(m_iCurBtnSel);
         return 0;
      case VK_ESCAPE:
         if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
         return 0;
      case VK_LEFT:
      case VK_RIGHT:
      case VK_UP:
      case VK_DOWN:
         // TODO: Add keyboard navigation
         return 0;
      }
      return 0;
   }

   LRESULT OnToolTipDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      // Forward notification to my owner instead
      pnmh->hwndFrom = m_hWnd;
      pnmh->idFrom = _GetDlgCtrlID();
      return ::SendMessage(GetParent(), WM_NOTIFY, pnmh->idFrom, (LPARAM)pnmh);
   }

   // Overridables

   void DoPaint(HDC hdc)
   {
      HWND hwndParent = GetParent();
      ATLASSERT(::IsWindow(hwndParent));
      UINT iDlgCtrl = _GetDlgCtrlID();

      // Save current DC selections
      int save = ::SaveDC(hdc);
      ATLASSERT(save!=0);

      // Prepare DC
      ::SelectObject(hdc, m_font);

      DRAWITEMSTRUCT dis = { 0 };
      dis.CtlType = ODT_BUTTON;
      dis.CtlID = iDlgCtrl;
      dis.hwndItem = m_hWnd;
      dis.hDC = hdc;

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         RECT rc = m_rcDefaultButton;
         _DrawButton(hdc, rc, CHB_ITEM_DEFAULT, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
      }
      int i=0;
      while( i<m_nButtons ) {
         RECT rc = m_rcButtons[i];
         _DrawButton(hdc, rc, i, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
         i++;
      }
      if( m_dwExtStyle & CHB_EX_CUSTOM ) {
         RECT rc = m_rcCustomButton;
         _DrawButton(hdc, rc, CHB_ITEM_CUSTOM, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
      }

      ::RestoreDC(hdc, save);
   }

   void UpdateLayout()
   {
      SIZE sizeInset = { 4, 4 };
      POINT ptOrg = { sizeInset.cx, sizeInset.cy };
      POINT pt = ptOrg;
      int cx = 0;
      int cy = 0;
      int iLargeButtonPadding = max(3,m_sizePadding.cy);

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         int iHeight = m_sizeButton.cy + (m_sizeMargin.cy*2);
         RECT rc = { ptOrg.x, ptOrg.y, ptOrg.x, ptOrg.y + iHeight };
         m_rcDefaultButton = rc;
         pt.y += iHeight + iLargeButtonPadding;
         cy += iHeight + iLargeButtonPadding;
      }

      m_rcButtons.RemoveAll();
      int i=0;
      int nRow=0;
      while( i<m_nButtons ) {
         RECT rc = { pt.x, pt.y, pt.x + m_sizeButton.cx, pt.y + m_sizeButton.cy };
         m_rcButtons.Add(rc);
         i++;
         pt.x += m_sizeButton.cx + m_sizePadding.cx;
         if( nRow==0 ) cx += m_sizeButton.cx + m_sizePadding.cx;
         if( (i<m_nButtons) && ((i % m_sizeMatrix.cx )==0) ) {
            pt.x = ptOrg.x;
            pt.y += m_sizeButton.cy + m_sizePadding.cy;
            cy += m_sizeButton.cy + m_sizePadding.cy;
            nRow++;
         }
      }
      pt.y += m_sizeButton.cy;
      cy += m_sizeButton.cy;
      cx -= m_sizePadding.cx;

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         m_rcDefaultButton.right += cx;
      }
      if( m_dwExtStyle & CHB_EX_CUSTOM ) {
         pt.y += iLargeButtonPadding;
         RECT rc = { ptOrg.x, pt.y, ptOrg.x + cx, pt.y + m_sizeButton.cy };
         m_rcCustomButton = rc;
         cy += m_sizeButton.cy + iLargeButtonPadding;
      }
      cy += m_sizePadding.cy;

      // Resize window according to the new (cx,cy)
      RECT rcWin;
      GetWindowRect(&rcWin);
      rcWin.right = rcWin.left + cx + (sizeInset.cx*2);
      rcWin.bottom = rcWin.top + cy + (sizeInset.cy*2);
      ::AdjustWindowRectEx(&rcWin, GetStyle(), FALSE, GetExStyle());
      SetWindowPos(HWND_TOP, &rcWin, SWP_NOMOVE);

      // Set button rectangles for tooltip
      if( m_dwExtStyle & CHB_EX_TOOLTIP && m_tip.IsWindow() ) {
         TOOLINFO ti = { 0 };
         ti.cbSize = sizeof(ti);
         ti.hwnd = m_hWnd;
         for( int i=0; i<m_nButtons; i++ ) {
            ti.uId = i;
            ti.rect = m_rcButtons[i];
            m_tip.SetToolRect(&ti);
         }
      }
   }

};


/////////////////////////////////////////////////////////////////////////////
//
// The sample Choice Bar controls
//


class CPaletteSelectCtrl : 
   public CChoiceBarCtrl<CPaletteSelectCtrl>,
   public COwnerDraw<CPaletteSelectCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_PaletteSelectCtrl"))

   enum { NUM_COLORS = 21 };
   PALETTEENTRY m_colors[NUM_COLORS];

   BEGIN_MSG_MAP(CLineStyleCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      CHAIN_MSG_MAP(CChoiceBarCtrl<CPaletteSelectCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CPaletteSelectCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   COLORREF GetColor(int iIndex) const
   {
      ATLASSERT(iIndex>=0 && iIndex<NUM_COLORS);
      if( iIndex<0 || iIndex>=NUM_COLORS ) return RGB(0,0,0); 
      return RGB(m_colors[iIndex].peRed, m_colors[iIndex].peGreen, m_colors[iIndex].peBlue);
   }

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      HPALETTE hPal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
      ATLASSERT(hPal!=NULL);
      ::GetPaletteEntries(hPal, 0, NUM_COLORS, m_colors);
      
      SetExtStyle(CHB_EX_DEFAULT|CHB_EX_CUSTOM|CHB_EX_FLAT);
      SetButtonCount(NUM_COLORS);
      SetButtonSize(18,18);
      SetPadding(0,0);
      SetColumns(6);
      SetCurSel(0);
      return 0;
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      int iIndex = (int)lpDrawItemStruct->itemID;
      RECT &rc = lpDrawItemStruct->rcItem;

      if( iIndex >= 0 && iIndex<GetButtonCount() ) {
         ATLASSERT(GetButtonCount()<=NUM_COLORS);
         dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
         ::InflateRect(&rc, -1, -1);
         dc.FillSolidRect(&rc, GetColor(iIndex));
      }
      else if( iIndex==CHB_ITEM_DEFAULT ) {
         dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
         ::InflateRect(&rc, -3, -3);
         int i = GetCurSel();
         ATLASSERT(i>=0);
         dc.FillSolidRect(&rc, GetColor(i));
      }
      else if( iIndex==CHB_ITEM_CUSTOM ) {
         // If selected, move the text down a bit
         if( lpDrawItemStruct->itemState & ODS_SELECTED ) {
            rc.top += 2;
            rc.left += 2;
         }
         // Load button caption from resource
         TCHAR szText[64];
         ::LoadString(_Module.GetResourceInstance(), IDS_CUSTOMIZE, szText, sizeof(szText)/sizeof(TCHAR));
         dc.SetBkMode(TRANSPARENT);
         dc.DrawText(szText, -1, &rc, DT_CENTER | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);
      }
   }
};


class CBMChoiceBarCtrl : 
	public CChoiceBarCtrl<CBMChoiceBarCtrl>,
	public COwnerDraw<CBMChoiceBarCtrl>
{
public:
	DECLARE_WND_CLASS(_T("WTL_CBMChoiceBarCtrl"))

	enum { NUM_COLORS = 7 };

	BEGIN_MSG_MAP(CBMChoiceBarCtrl)
		REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
		CHAIN_MSG_MAP(CChoiceBarCtrl<CBMChoiceBarCtrl>)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CBMChoiceBarCtrl>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		SetButtonCount(NUM_COLORS);
		SetButtonSize(29,29);
		SetPadding(0,0);
		SetColumns(2);
		return 0;
	}


	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		CDCHandle dc(lpDrawItemStruct->hDC);
		int iIndex = (int)lpDrawItemStruct->itemID;
		RECT &rc = lpDrawItemStruct->rcItem;

		if( iIndex >= 0 && iIndex<GetButtonCount() ){
		ImageList_Draw(m_hImageList, iIndex, dc, rc.left, rc.top, ILD_TRANSPARENT);
		}
	}
};

#endif // !defined(AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_)

