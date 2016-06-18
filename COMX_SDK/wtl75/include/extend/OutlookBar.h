#if !defined(AFX_OUTLOOKBAR_H__20020722_ABF8_8A80_D684_0080AD509054__INCLUDED_)
#define AFX_OUTLOOKBAR_H__20020722_ABF8_8A80_D684_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutlookBarCtrl - MS Outlook Navigation Bar
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2002-2003 Bjarke Viksoe.
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
  #error This control requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error This control requires atlctrls.h to be included first
#endif

#ifndef __ATLGDIX_H__
  #error This control requires my atlgdix.h to be included first
#endif

#if (_WTL_VER < 0x0700)
   #error This control requires WTL version 7.0 or higher
#endif

#if (_WIN32_IE < 0x0400)
  #error This control requires _WIN32_IE >= 0x0400
#endif


// From Platform SDK
#ifndef ODS_HOTLIGHT
   #define ODS_HOTLIGHT        0x0040
#endif



/////////////////////////////////////////////////////////////////////////////
// The Outlook Bar Pane

// Extended Pane styles
#define OPS_FIXEDTOPINDEX   0x00000001

typedef CWinTraitsOR<LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | LBS_NOTIFY | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0> COutlookPaneCtrlTraits;

template< class T, class TBase = CListBox, class TWinTraits = COutlookPaneCtrlTraits >
class ATL_NO_VTABLE COutlookPaneImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public COffscreenDrawRect< T >,
   public CMouseHover< T >,
   public COwnerDraw< T >
{
public:
   typedef COutlookPaneImpl< T , TBase, TWinTraits > thisClass;

   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   enum
   {
      _MaxItemTextLength = 128,
   };

   typedef struct tagITEMDATA 
   {
      UINT nCmd;      // Command Identifier associated with item
      int iImage;     // Image to use from ImageLists
      LPARAM lParam;  // Optional parameter
   } ITEMDATA;

   CContainedWindow m_wndParent;
   CImageList m_LargeImages;
   CImageList m_SmallImages;
   COLORREF m_clrBack;
   COLORREF m_clrText;
   UINT m_iViewStyle;
   SIZE m_szPadding;
   int m_iLastMouseOver;
   DWORD m_dwExtStyle;

   // Operations

   COutlookPaneImpl() : m_wndParent(this, 1)
   {
   }
   virtual ~COutlookPaneImpl()
   {
      if( m_wndParent.IsWindow() ) m_wndParent.UnsubclassWindow();
   }

   BOOL SetViewStyle(UINT uStyle)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      // Change style
      m_iViewStyle = uStyle;
      // Repaint
      Invalidate();
      // Force items to recalculate their height
      _RecalcItemHeights();
      // Hmmm, this is a bit nasty. But to trigger a layout
      // update when this control is embedded in a COutlookPagerCtrl
      // we resize the parent - hoping a WM_SIZE will cause a
      // layout update!!!
      RECT rcClient;
      ::GetClientRect(GetParent(), &rcClient);
      LPARAM lParam = MAKELPARAM(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
      ::SendMessage(GetParent(), WM_SIZE, SIZE_RESTORED, lParam);
      return TRUE;
   }
   UINT GetViewStyle() const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      return m_iViewStyle;
   }
   void SetExtendedPaneStyle(DWORD dwStyle)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_dwExtStyle = dwStyle;
      Invalidate();
   }
   DWORD GetExtendedPaneStyle() const
   {
      return m_dwExtStyle;
   }
   BOOL SetTextColor(COLORREF clrText)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_clrText = clrText;
      Invalidate();
      return TRUE;
   }
   BOOL SetBkColor(COLORREF clrBack)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_clrBack = clrBack;
      Invalidate();
      return TRUE;
   }
   BOOL GetViewRect(RECT* pRC) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(pRC);
      ::ZeroMemory(pRC, sizeof(RECT));
      RECT rcClient;
      GetClientRect(&rcClient);
      pRC->right = rcClient.right - rcClient.left;
      for( int i = 0; i < GetCount(); i++ ) {
         pRC->bottom += GetItemHeight(i);
      }
      return AdjustWindowRectEx(pRC, GetStyle(), FALSE, GetExStyle());
   }
   CImageList SetImageList(HIMAGELIST hImages, int nImageList)
   {
      CImageList hOldImages;
      switch( nImageList ) {
      case LVSIL_NORMAL:
         hOldImages = m_LargeImages;
         m_LargeImages = hImages;
         break;
      case LVSIL_SMALL:
         hOldImages = m_SmallImages;
         m_SmallImages = hImages;
         break;
      default:
         ATLASSERT(false);
      }
      return hOldImages;
   }
   int InsertItem(int iIndex, UINT nCmd, LPCTSTR pstrName, int iImage, LPARAM lParam = 0)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(!::IsBadStringPtr(pstrName, -1));
      int idx = TBase::InsertString(iIndex, pstrName);
      if( idx == LB_ERR ) return -1;
      ITEMDATA* pItemData;
      ATLTRY(pItemData= new ITEMDATA);
      ATLASSERT(pItemData);
      if( pItemData == NULL ) return -1;
      pItemData->nCmd = nCmd;
      pItemData->iImage = iImage;
      pItemData->lParam = lParam;
      TBase::SetItemDataPtr(idx, pItemData);
      return idx;
   }
   BOOL DeleteItem(int nIndex)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      return TBase::DeleteString((UINT)iIndex) != LB_ERR;
   }
   void DeleteAllItems()
   {
      ATLASSERT(::IsWindow(m_hWnd));
      TBase::ResetContent();
   }
   LPARAM GetItemData(int iIndex)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ITEMDATA* pItemData = (ITEMDATA*) TBase::GetItemData(iIndex);
      if( pItemData == NULL || pItemData == (LPVOID)-1 ) return 0;
      return pItemData->lParam;
   }
   BOOL SetItemData(int iIndex, LPARAM lParam)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ITEMDATA* pItemData = (ITEMDATA*) TBase::GetItemData(iIndex);
      if( pItemData == NULL || pItemData == (LPVOID)-1 ) return FALSE;
      pItemData->lParam = lParam;
      return TRUE;
   }
   int ItemFromPos(POINT pt)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      for( int i = 0; i < GetCount(); i++ ) {
         RECT rc;
         GetItemRect(i, &rc);
         if( ::PtInRect(&rc, pt) ) {
            if( m_iViewStyle == LVS_ICON ) {
               // In icon mode, the actual area of the item
               // is slightly smaller (narrowed around the icon).
               int iMiddle = (rc.right - rc.left) / 2;
               RECT rcItem = { rc.left + iMiddle - 50, rc.top, rc.left + iMiddle + 50, rc.bottom };
               if( ::PtInRect(&rcItem, pt) ) {
                  return i;
               }
            }
            else {
               // In report mode the GetItemRect() is correct...
               return i;
            }
         }
      }
      return -1;
   }
   void InvalidateItem(int iIndex)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex == -1 ) return;
      RECT rc = { 0 };
      GetItemRect(iIndex, &rc);
      InvalidateRect(&rc);
   }

   // Unsupported methods

   int AddString(LPCTSTR /*lpszItem*/)
   {
      ATLASSERT(false);
      return -1;
   }
   int InsertString(int /*nIndex*/, LPCTSTR /*lpszItem*/)
   {
      ATLASSERT(false);
      return -1;
   }
   int Dir(UINT /*attr*/, LPCTSTR /*lpszWildCard*/)
   {
      ATLASSERT(false);
      return -1;
   }
   int AddFile(LPCTSTR /*lpstrFileName*/)
   {
      ATLASSERT(false);
      return -1;
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(GetStyle() & LBS_OWNERDRAWVARIABLE);      // Needs this
      ATLASSERT(GetStyle() & LBS_HASSTRINGS);             // and this
      ATLASSERT(GetStyle() & LBS_NOTIFY);                 // and this!
      ModifyStyle(LBS_MULTIPLESEL | LBS_USETABSTOPS, 0);  // Don't like these!
      m_dwExtStyle = 0L;
      m_clrBack = ::GetSysColor(COLOR_BTNSHADOW);
      m_clrText = ::GetSysColor(COLOR_BTNHIGHLIGHT);
      m_szPadding.cx = 3;
      m_szPadding.cy = 5;
      m_iLastMouseOver = -1;
      SetViewStyle(LVS_ICON);
      SetFont(AtlGetDefaultGuiFont());

      m_wndParent.SubclassWindow(GetParent());
   }
   BOOL _RecalcItemHeights()
   {
      // Force each item to recalculate its height
      MEASUREITEMSTRUCT mis = { 0 };
      mis.CtlID = GetDlgCtrlID();
      mis.CtlType = ODT_LISTBOX;
      int cnt = GetCount();
      for( int i = 0; i < cnt; i++ ) {
         mis.itemID = i;
         mis.itemData = GetItemData(i);
         mis.itemWidth = mis.itemHeight = 0;
         ::SendMessage(GetParent(), WM_MEASUREITEM, mis.CtlID, (LPARAM) &mis);
         SetItemHeight(i, mis.itemHeight);
      }
      return TRUE;
   }
   void _DrawEdge(CDCHandle& dc, UINT uState, RECT& rcEdge)
   {
      if( uState & ODS_SELECTED ) {
         dc.DrawEdge(&rcEdge, EDGE_SUNKEN, BF_RECT);
      }
      else if( uState & ODS_HOTLIGHT ) {
         dc.DrawEdge(&rcEdge, EDGE_RAISED, BF_RECT);
      }
      else {
         dc.FillSolidRect(&rcEdge, m_clrBack);
      }
   }

   // Message map and handlers

   BEGIN_MSG_MAP( thisClass )
      CHAIN_MSG_MAP( CMouseHover< T > )
      CHAIN_MSG_MAP( COffscreenDrawRect< T > )
      CHAIN_MSG_MAP_ALT( COwnerDraw< T >, 1 )
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      DEFAULT_REFLECTION_HANDLER()
   ALT_MSG_MAP(1)
      if( uMsg == WM_DRAWITEM || uMsg == WM_MEASUREITEM || uMsg == WM_DELETEITEM ) {
         if( wParam == (WPARAM) GetDlgCtrlID() ) {
#if _ATL_VER >= 0x0700
            // FIX: Reflect these message properly in ATL 7!
            //      Thanks to Yuri Basin <Yuri.Basin@pkware.com> for reporting this!
            bHandled = TRUE;
            lResult = m_wndParent.ReflectNotifications(uMsg, wParam, lParam, bHandled);
            if( bHandled ) return TRUE;
#else
            CHAIN_MSG_MAP( COwnerDraw< T > )
#endif
         }
      }
      if( uMsg == WM_COMMAND && m_hWnd == (HWND) lParam ) {
         COMMAND_CODE_HANDLER(LBN_SELCHANGE, OnItemSelected)
      }
   END_MSG_MAP()
 
   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      _Init();
      return lRes;
   }
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      // FIX: We must remove all items to correctly destroy this window
      //      Fixed by Dennis Shtyrov.
      DeleteAllItems();
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Hmm, it should be possible to remove this? No?
      // Well, without it a Pager control it doesn't repaint right
      // when resizing the client?!
      Invalidate();
      return 0;
   }
   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Poor man's implementation of item hotlight in the standard listbox.
      // First we find the item we're hovering over...
      POINT pt;
      ::GetCursorPos(&pt);
      ScreenToClient(&pt);
      int idx = ItemFromPos(pt);
      // Did the current item change?
      if( idx != m_iLastMouseOver ) {
         InvalidateItem(idx);
         InvalidateItem(m_iLastMouseOver);
         m_iLastMouseOver = idx;
      }
      return 0;
   }
   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      InvalidateItem(m_iLastMouseOver);
      m_iLastMouseOver = -1;
      return 0;
   }

   LRESULT OnItemSelected(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      int idx = GetCurSel();
      if( idx == -1 ) return 0;
      // To fix a problem when keyboard scrolling and the Pager control
      // using the OPS_FIXEDTOPINDEX style might be a solution
      if( m_dwExtStyle & OPS_FIXEDTOPINDEX ) SetTopIndex(0);
      // Retrieve item data
      ITEMDATA* pItemData = static_cast<ITEMDATA*>(TBase::GetItemDataPtr(idx));
      if( pItemData == NULL || pItemData == (LPVOID) -1 ) return 0;
      // Send WM_COMMAND notification to top-level window
      ::SendMessage(GetTopLevelParent(), WM_COMMAND, MAKEWPARAM(pItemData->nCmd, 0), (LPARAM) m_hWnd);
      return 0;
   }

   // Overridables

   void DoPaint(CDCHandle dc, RECT rc)
   {
      dc.FillSolidRect(&rc, m_clrBack);
      DefWindowProc(WM_PRINTCLIENT, (WPARAM) (HDC) dc, PRF_CHILDREN | PRF_CLIENT);
   }

   // Custom draw methods

   void DrawItem(LPDRAWITEMSTRUCT lpDIS)
   {
      ATLASSERT(lpDIS->CtlType==ODT_LISTBOX);
      if( lpDIS->itemID == -1 ) return;
      if( m_iLastMouseOver == (int) lpDIS->itemID ) lpDIS->itemState |= ODS_HOTLIGHT;

      CDCHandle dc(lpDIS->hDC);
      RECT rcItem = lpDIS->rcItem;

      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(m_clrText);

      TCHAR szText[_MaxItemTextLength+1] = { 0 };
      GetText(lpDIS->itemID, szText);

      ITEMDATA* pItemData = reinterpret_cast<ITEMDATA*>(lpDIS->itemData);
      ATLASSERT(pItemData);

      if( m_iViewStyle == LVS_ICON ) {
         ::InflateRect(&rcItem, -m_szPadding.cx, -m_szPadding.cy);
         
         // Figure out where the centered icon should be
         SIZE szIcon;
         m_LargeImages.GetIconSize(szIcon);
         int iMiddle = rcItem.left + (rcItem.right - rcItem.left)/2;
         RECT rcIcon;
         rcIcon.left = iMiddle - (szIcon.cx)/2;
         rcIcon.top = rcItem.top + 4;
         rcIcon.right = rcIcon.left + szIcon.cx; 
         rcIcon.bottom = rcIcon.top + szIcon.cy;

         // Paint edge and icon
         RECT rcEdge = rcIcon;
         ::InflateRect(&rcEdge, 6, 6);
         _DrawEdge(dc, lpDIS->itemState, rcEdge);
         if( lpDIS->itemState & ODS_HOTLIGHT ) ::OffsetRect(&rcIcon, 0, -1);
         m_LargeImages.Draw(dc, pItemData->iImage, rcIcon.left, rcIcon.top, ILD_TRANSPARENT);

         // Draw text
         rcItem.top += szIcon.cy + 8 + 2;
         dc.DrawText(szText, -1, &rcItem, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
      }
      else {
         // Paint edge and icon
         _DrawEdge(dc, lpDIS->itemState, rcItem);
         ::InflateRect(&rcItem, -m_szPadding.cx, -m_szPadding.cy);
         m_SmallImages.Draw(dc, pItemData->iImage, rcItem.left, rcItem.top, ILD_TRANSPARENT);

         // Draw text
         SIZE szIcon;
         m_SmallImages.GetIconSize(szIcon);
         rcItem.left += szIcon.cx + 4; // Text padding
         dc.DrawText(szText, -1, &rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
      }
   }
   void MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
   {
      int cy = 0;
      if( m_iViewStyle == LVS_ICON ) {
         const int cxTextWidth = 100;  // The width we're trying to fit the text into
         // Get icon size
         SIZE szIcon;
         m_LargeImages.GetIconSize(szIcon);
         cy += szIcon.cy;
         cy += 4;                      // Padding between icon and top
         cy += 4;                      // Padding between icon and text
         // Calculate text size
         CClientDC dc(m_hWnd);
         ATLASSERT(GetTextLen(lpMIS->itemID) < _MaxItemTextLength);
         TCHAR szText[_MaxItemTextLength+1] = { 0 };
         GetText(lpMIS->itemID, szText);
         RECT rcClient = { 0, 0, cxTextWidth, 0 };
         dc.DrawText(szText, -1, &rcClient, DT_WORDBREAK | DT_CALCRECT);
         cy += rcClient.bottom - rcClient.top;
         cy += m_szPadding.cy * 2;
      }
      else {
         SIZE size;
         m_SmallImages.GetIconSize(size);
         cy = size.cy + (m_szPadding.cy * 2);
      }
      lpMIS->itemHeight = cy;
   }
   void DeleteItem(LPDELETEITEMSTRUCT lpDIS)
   {
      ATLASSERT(lpDIS->itemData);
      delete (ITEMDATA*) lpDIS->itemData;
   }
};

class COutlookPaneCtrl : public COutlookPaneImpl<COutlookPaneCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_OutlookPane"), GetWndClassName())  
};


/////////////////////////////////////////////////////////////////////////////
// The Outlook Pager control

typedef CWinTraitsOR<WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0> COutlookPagerCtrlTraits;

template< class T, class TBase = CWindow, class TWinTraits = COutlookPagerCtrlTraits >
class ATL_NO_VTABLE COutlookPagerImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public COwnerDraw< T >
{
public:
   typedef COutlookPagerImpl< T , TBase, TWinTraits > thisClass;

   DECLARE_WND_CLASS(NULL)

   enum 
   {
      ID_UP = 0x1234,              // The (hopefully) unique Control IDs
      ID_DOWN = ID_UP + 1,         // of the Up- and Down-button.
   };

   CWindow m_wndClient;
   CButton m_ctrlUp;
   CButton m_ctrlDown;
   COLORREF m_clrBack;
   int m_iPos;

   // Operations

   BOOL SetChild(HWND hWnd)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(::IsWindow(hWnd));
      m_wndClient = hWnd;
      m_wndClient.SetParent(m_hWnd);
      return SetPos(0);
   }
   BOOL SetPos(int iPos)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_iPos = iPos;
      _RecalcLayout();
      Invalidate();
      return TRUE;
   }
   int GetPos() const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      return m_iPos;
   }
   void SetBkColor(COLORREF clrBack)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_clrBack = clrBack;
      Invalidate();
   }
   DWORD GetButtonsState(int iButton) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      UINT nID = iButton == PGB_TOPORLEFT ? ID_UP : ID_DOWN;
      CButton ctrlButton = GetDlgItem(nID);
      DWORD dwRes = PGF_NORMAL;
      if( ctrlButton.GetState() & BST_PUSHED ) dwRes = PGF_DEPRESSED;
      if( !ctrlButton.IsWindowEnabled() ) dwRes = PGF_GRAYED;
      if( !ctrlButton.IsWindowVisible() ) dwRes = PGF_INVISIBLE;
      return dwRes;
   }
   BOOL RecalcSize()
   {
      return _RecalcLayout();
   }

   // Message map and handlers

   BEGIN_MSG_MAP( thisClass )
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      COMMAND_ID_HANDLER(ID_UP, OnUpButton)
      COMMAND_ID_HANDLER(ID_DOWN, OnDownButton)
      CHAIN_MSG_MAP( COwnerDraw< T > )
      FORWARD_NOTIFICATIONS()
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      _Init();
      return 0;
   }
   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }
   LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( wParam == SIZE_MINIMIZED ) return 0;
      _RecalcLayout();
      return 0;
   }

   LRESULT OnUpButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      SetPos(m_iPos - 30);
      m_wndClient.SetFocus();
      return 0;
   }
   LRESULT OnDownButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      SetPos(m_iPos + 30);
      m_wndClient.SetFocus();
      return 0;
   }

   // Implementation

   void _Init()
   {
      DWORD dwStyle = BS_OWNERDRAW | WS_CHILD;
      m_ctrlUp.Create(m_hWnd, rcDefault, NULL, dwStyle, 0, ID_UP);
      ATLASSERT(::IsWindow(m_ctrlUp));
      m_ctrlDown.Create(m_hWnd, rcDefault, NULL, dwStyle, 0, ID_DOWN);
      ATLASSERT(::IsWindow(m_ctrlDown));
      m_iPos = 0;
      m_clrBack = ::GetSysColor(COLOR_BTNFACE);
   }
   BOOL _RecalcLayout()
   {
      if( !m_wndClient.IsWindow() ) return 0;

      // Get the client measures
      RECT rcClient;
      GetClientRect(&rcClient);
      int cxClient = rcClient.right - rcClient.left;
      int cyClient = rcClient.bottom - rcClient.top;
      int cxThumb = ::GetSystemMetrics(SM_CXHTHUMB);
      int cyThumb = ::GetSystemMetrics(SM_CYVTHUMB);

      // Ask parent what the recommended size of the view is...
      NMPGCALCSIZE nmpgcs = { 0 };
      nmpgcs.hdr.code = PGN_CALCSIZE;
      nmpgcs.hdr.hwndFrom = m_hWnd;
      nmpgcs.hdr.idFrom = GetDlgCtrlID();
      nmpgcs.dwFlag = PGF_CALCHEIGHT;
      nmpgcs.iWidth = cxClient;
      nmpgcs.iHeight = cyClient;
      ::SendMessage(GetParent(), WM_NOTIFY, nmpgcs.hdr.idFrom, (LPARAM) &nmpgcs);

      // Adjust the scroll position into a valid range
      if( m_iPos + cyClient >= nmpgcs.iHeight ) m_iPos = nmpgcs.iHeight - cyClient;
      if( m_iPos < 0 ) m_iPos = 0;

      // Place the child window      
      RECT rcChild = rcClient;
      rcChild.top -= m_iPos;
      rcChild.bottom = max(rcChild.top + nmpgcs.iHeight, cyClient);
      m_wndClient.MoveWindow(&rcChild);

      // Place the Up button
      bool bShowUp = m_iPos > 0;
      if( cyClient < 80 ) bShowUp = false;
      RECT rcUp = { rcClient.right - (cxThumb + 10), rcClient.top + 10, rcClient.right - 10, rcClient.top + cyThumb + 10 };
      DWORD dwState = SWP_NOACTIVATE;
      dwState |= bShowUp ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
      m_ctrlUp.SetWindowPos(HWND_TOP, &rcUp, dwState);

      // Place the Down button
      bool bShowDown = m_iPos + cyClient < nmpgcs.iHeight;
      if( cyClient < 80 ) bShowDown = false;
      RECT rcDown = { rcClient.right - (cxThumb + 10), rcClient.bottom - (cyThumb + 10), rcClient.right - 10, rcClient.bottom - 10 };
      dwState = SWP_NOACTIVATE;
      dwState |= bShowDown ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
      m_ctrlDown.SetWindowPos(HWND_TOP, &rcDown, dwState);

      return TRUE;
   }

   // Ownerdraw buttons

   void DrawItem(LPDRAWITEMSTRUCT lpDIS)
   {
      if( lpDIS->hwndItem != m_ctrlUp && lpDIS->hwndItem != m_ctrlDown ) return;
      CDCHandle dc( lpDIS->hDC );
      UINT uState = DFCS_SCROLLUP;
      if( lpDIS->hwndItem == m_ctrlDown ) uState = DFCS_SCROLLDOWN;
      if( lpDIS->itemState & ODS_SELECTED ) uState |= DFCS_PUSHED;
      dc.DrawFrameControl(&lpDIS->rcItem, DFC_SCROLL, uState);
   }
};

class COutlookPagerCtrl : public COutlookPagerImpl<COutlookPagerCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_OutlookPager"))
};



/////////////////////////////////////////////////////////////////////////////
// The Outlook Bar control

typedef CWinTraitsOR<RBS_VERTICALGRIPPER | RBS_VARHEIGHT | RBS_FIXEDORDER | CCS_LEFT | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN, WS_EX_CLIENTEDGE> COutlookBarCtrlTraits;

template< class T, class TBase = CReBarCtrl, class TWinTraits = COutlookBarCtrlTraits >
class ATL_NO_VTABLE COutlookBarImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public CCustomDraw< T >
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   typedef COutlookBarImpl< T, TBase, TWinTraits > thisClass;

   enum 
   {
      _MaxButtonTextLength = 128,
   };

   CContainedWindow m_wndParent;

   // Operations

   COutlookBarImpl() : m_wndParent(this, 1)
   {
   }
   virtual ~COutlookBarImpl()
   {
      if( m_wndParent.IsWindow() ) m_wndParent.UnsubclassWindow();
   }

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
      TCHAR szBuffer[32] = { 0 };
      if( ::GetClassName(hWnd, szBuffer, (sizeof(szBuffer)/sizeof(TCHAR))-1) ) {
         ATLASSERT(::lstrcmpi(szBuffer, TBase::GetWndClassName())==0);
      }
#endif
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }
   BOOL InsertBand(int iIndex, LPCTSTR pstrTitle, HWND hWnd)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(!::IsBadStringPtr(pstrTitle, -1));
      // Insert a new ReBar band
      REBARBANDINFO rbi = { 0 };
      rbi.cbSize = sizeof(REBARBANDINFO);  
      rbi.fMask = RBBIM_TEXT | RBBIM_STYLE | RBBIM_CHILD | RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_ID;
      rbi.lpText = const_cast<LPTSTR>(pstrTitle);
      rbi.cx = 42; 
      rbi.cxMinChild = 0;
      rbi.cyMinChild = 10;
      rbi.fStyle = RBBS_GRIPPERALWAYS;
      rbi.hwndChild  = hWnd;
      rbi.wID = GetBandCount();
      return InsertBand(iIndex, &rbi);
   }
   BOOL InsertBand(int iIndex, REBARBANDINFO* pRBI)
   {
      ATLASSERT(!::IsBadReadPtr(pRBI, sizeof(REBARBANDINFO)));
      BOOL bOk = TBase::InsertBand(iIndex, pRBI);
      if( bOk ) {
         // First pane should always be initially maximized
         PostMessage(RB_MAXIMIZEBAND, 0, 0L);
      }
      return bOk;      
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));

      // Initialize and send the REBARINFO structure.
      REBARINFO rbi = { 0 };
      rbi.cbSize = sizeof(REBARINFO);
      rbi.fMask = 0;
      rbi.himl = NULL;
      SetBarInfo(&rbi);

      // Assign rebar colors
      SetTextColor(::GetSysColor(COLOR_3DLIGHT));
      SetBkColor(::GetSysColor(COLOR_3DSHADOW));

      // Subclass parent window (for message reflection)
      m_wndParent.SubclassWindow(GetParent());
   }

   // Message map and handler
   
   BEGIN_MSG_MAP( thisClass )
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      CHAIN_MSG_MAP_ALT( CCustomDraw< T >, 1 )    // Custom draw of rebar
      DEFAULT_REFLECTION_HANDLER()
   ALT_MSG_MAP(1)
      if( uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->hwndFrom == m_hWnd ) {
         NOTIFY_CODE_HANDLER(RBN_BEGINDRAG, OnRebarBeginDrag)
#if _ATL_VER >= 0x0700
         // FIX: Reflect these message properly in ATL 7!
         //      Thanks to Yuri Basin <Yuri.Basin@pkware.com> for reporting this!
         bHandled = TRUE;
         lResult = m_wndParent.ReflectNotifications(uMsg, wParam, lParam, bHandled);
         if( bHandled ) return TRUE;
#else
         CHAIN_MSG_MAP( CCustomDraw< T > )    // Custom draw of rebar
#endif
      }
   END_MSG_MAP()

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
      _Init();
      return lRes;
   }
   LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // HACK: Prevent drag-cursor when clicking on header
      ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
      return TRUE;
   }
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      // Hmm, resizing has proven quite tedious. The rebar and toolbar controls
      // prefer to stick to a container side and takes any oppotunity to resize itself.
      // I guess figuring out the right combination of CCS_xxx flags would solve the problem
      // but for now I'll just force a resize on child controls/buttons...
      int cxClient = (int) LOWORD(lParam);
      int nCount = GetBandCount();
      for( int i = 0; i < nCount; i++ ) {
         REBARBANDINFO rbi = { 0 };
         rbi.cbSize = sizeof(REBARBANDINFO);  
         rbi.fMask = RBBIM_CHILD;
         GetBandInfo(i, &rbi);
         // Resize the pager
         CWindow wndChild = rbi.hwndChild;
         wndChild.ResizeClient(cxClient, -1);
         // Reisze the rebar band
         rbi.fMask = RBBIM_CHILDSIZE;
         rbi.cxMinChild = 0;
         rbi.cyMinChild = cxClient;
         SetBandInfo(i, &rbi);
      }
      return lRes;
   }
   LRESULT OnRebarBeginDrag(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      return 1; // Don't allow drag'n'drop of rebar bands...
   }

   // Custom draw methods

   DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
   {
      return CDRF_NOTIFYITEMDRAW;
   }
   DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
   {
      // Custom paint the gripper bars in the rebar bands
      
      CDCHandle dc( lpNMCustomDraw->hdc );
      RECT& rcItem = lpNMCustomDraw->rc;

      // Need to paint a header-button instead
      dc.DrawFrameControl(&rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);

      // And to draw header text
      REBARBANDINFO rbi = { 0 };
      TCHAR szText[_MaxButtonTextLength+1] = { 0 };
      rbi.cbSize = sizeof(REBARBANDINFO);  
      rbi.fMask = RBBIM_TEXT;
      rbi.lpText = szText;
      rbi.cch = _MaxButtonTextLength;
      GetBandInfo(static_cast<int>(lpNMCustomDraw->dwItemSpec), &rbi);

      HFONT hOldFont = dc.SelectFont(GetFont());
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
      dc.DrawText(szText, -1, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
      dc.SelectFont(hOldFont);

      return CDRF_SKIPDEFAULT;
   }
};

class COutlookBarCtrl : public COutlookBarImpl<COutlookBarCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_OutlookBar"), GetWndClassName())  
};

#endif // !defined(AFX_OUTLOOKBAR_H__20020722_ABF8_8A80_D684_0080AD509054__INCLUDED_)

