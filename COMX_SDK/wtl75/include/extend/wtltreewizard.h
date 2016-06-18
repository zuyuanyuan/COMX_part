#ifndef _KMAS_COMX_WTL_EXTEND_TREE_WIZARD_H
#define _KMAS_COMX_WTL_EXTEND_TREE_WIZARD_H

#define WIZ_IDBACK						0x3023
#define WIZ_IDNEXT                      0x3024
#define WIZ_IDFINISH                    0x3025
#define WIZ_IDSEPERATER				    0x3026
#define WIZ_IDHEAD                      0x3027
#define WIZ_IDTABCONTROL                0x3020
#define WIZ_IDCANCEL					0x0002
#define WIZ_IDHELP  					0x0009
#define WIZ_IDAPPLY						0x9888
#define WIZ_TREE						0x9889

#include <wtlvistatreectrl.h>

class CWizardTreeCtrl : public CWindowImpl< CWizardTreeCtrl, CTreeViewCtrl >,
                        public CCustomDraw< CWizardTreeCtrl >,
						public CDoubleBufferImpl< CTreeCtrl >
{
	COLORREF m_rgbBackground;
	COLORREF m_rgbItemText;
	COLORREF m_rgbSelectOuter;
	COLORREF m_rgbSelectInner;
	COLORREF m_rgbSelectTop;
	COLORREF m_rgbSelectBottom;
	COLORREF m_rgbNoFocusTop;
	COLORREF m_rgbNoFocusBottom;
	COLORREF m_rgbNoFocusOuter;
	COLORREF m_rgbNoFocusInner;
public:
	BOOL LoadSettings()
	{
		m_rgbBackground = GetSysColor( COLOR_INFOBK );
		m_rgbItemText = GetSysColor( COLOR_WINDOWTEXT );
		m_rgbSelectOuter = RGB( 170, 200, 245 );
		m_rgbSelectInner = RGB( 230, 250, 250 );
		m_rgbSelectTop = RGB( 255, 255, 255 );
		m_rgbSelectBottom = RGB( 185, 215, 250 );
		m_rgbNoFocusTop = RGB( 250, 250, 250 );
		m_rgbNoFocusBottom = RGB( 235, 235, 235 );
		m_rgbNoFocusOuter = RGB( 220, 220, 220 );
		m_rgbNoFocusInner = RGB( 245, 245, 245 );

		return TRUE;
	}
	BOOL Initialise()
	{
		// load interface settings
		if ( !LoadSettings() )
			return FALSE;

		// give control a static border
		ModifyStyle( WS_BORDER, WS_CLIPCHILDREN | TVS_SHOWSELALWAYS, SWP_FRAMECHANGED );
		ModifyStyleEx( WS_EX_CLIENTEDGE, WS_EX_STATICEDGE, SWP_FRAMECHANGED );

		return TRUE;
	}

	BOOL SubclassWindow( HWND hWnd )
	{
		return CWindowImpl< CWizardTreeCtrl, CTreeViewCtrl >::SubclassWindow( hWnd ) ? Initialise() : FALSE;
	}
public:
	CWizardTreeCtrl(){LoadSettings();}
public:
     BEGIN_MSG_MAP(CWizardTreeCtrl)
          CHAIN_MSG_MAP_ALT(CCustomDraw< CWizardTreeCtrl >, 1)
		  CHAIN_MSG_MAP(CDoubleBufferImpl< CTreeCtrl >)
		  DEFAULT_REFLECTION_HANDLER()
     END_MSG_MAP()
public:
     void DoPaint( CDCHandle dcPaint )
          {
               // default paint handlers will call custom draw
               DefWindowProc( WM_ERASEBKGND, (WPARAM)dcPaint.m_hDC, 0 );
               DefWindowProc( WM_PAINT, (WPARAM)dcPaint.m_hDC, 0 );
          }
	
     DWORD OnPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCustomDraw )
          {		
               return CDRF_NOTIFYITEMDRAW;
          }
	
     DWORD OnItemPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCustomDraw )
          {
               return CDRF_NOTIFYPOSTPAINT;
          }
     DWORD OnItemPostPaint( int idCtrl, LPNMCUSTOMDRAW lpNMCustomDraw )
          {
               LPNMLVCUSTOMDRAW lpCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>( lpNMCustomDraw );
		
               CRect rcItem;
               if ( !GetItemRect( (HTREEITEM)lpCustomDraw->nmcd.dwItemSpec, rcItem, TRUE ) )
                    return CDRF_DODEFAULT;
		
               CDCHandle dcPaint( lpCustomDraw->nmcd.hdc );
               int nContextState = dcPaint.SaveDC();
		
               dcPaint.SetBkColor( m_rgbBackground );
               dcPaint.ExtTextOut( rcItem.left, rcItem.top, ETO_OPAQUE, rcItem, _T( "" ), 0, NULL );
		
               // only one item is selected at any one time
               BOOL bSelected = ( GetItemState( (HTREEITEM)lpCustomDraw->nmcd.dwItemSpec, TVIS_SELECTED | TVIS_DROPHILITED ) & ( TVIS_SELECTED | TVIS_DROPHILITED ) );
               HTREEITEM hHighlightItem = GetDropHilightItem();
               if ( hHighlightItem != NULL && hHighlightItem != (HTREEITEM)lpCustomDraw->nmcd.dwItemSpec )
                    bSelected = FALSE;
		
               // draw selected background and border
               if ( bSelected )
               {
                    BOOL bControlFocus = true/*( GetFocus() == m_hWnd )*/;
                    CRect rcSelect( rcItem );
			
                    CPen penBorder;
                    penBorder.CreatePen( PS_SOLID, 1, bControlFocus ? m_rgbSelectOuter : m_rgbNoFocusOuter );
                    CBrush bshInterior;
                    bshInterior.CreateSolidBrush( m_rgbBackground );
			
                    dcPaint.SelectPen( penBorder );
                    dcPaint.SelectBrush( bshInterior );
			
                    dcPaint.RoundRect( rcSelect, CPoint( 5, 5 ) );
                    rcSelect.DeflateRect( 1, 1 );
			
                    CPen penInnerBorder;
                    penInnerBorder.CreatePen( PS_SOLID, 1, bControlFocus ? m_rgbSelectInner : m_rgbNoFocusInner );
                    dcPaint.SelectPen( penInnerBorder );
			
                    dcPaint.RoundRect( rcSelect, CPoint( 2, 2 ) );
                    rcSelect.DeflateRect( 1, 1 );
			
                    GRADIENT_RECT grdRect = { 0, 1 };
                    TRIVERTEX triVertext[ 2 ] = {
                         rcSelect.left,
                         rcSelect.top,
                         GetRValue( bControlFocus ? m_rgbSelectTop : m_rgbNoFocusTop ) << 8,
                         GetGValue( bControlFocus ? m_rgbSelectTop : m_rgbNoFocusTop ) << 8,
                         GetBValue( bControlFocus ? m_rgbSelectTop : m_rgbNoFocusTop ) << 8,
                         0x0000,			
                         rcSelect.right,
                         rcSelect.bottom,
                         GetRValue( bControlFocus ? m_rgbSelectBottom : m_rgbNoFocusBottom ) << 8,
                         GetGValue( bControlFocus ? m_rgbSelectBottom : m_rgbNoFocusBottom ) << 8,
                         GetBValue( bControlFocus ? m_rgbSelectBottom : m_rgbNoFocusBottom ) << 8,
                         0x0000
                    };
			
                    dcPaint.GradientFill( triVertext, 2, &grdRect, 1, GRADIENT_FILL_RECT_V );
               }
		
               CString strItemText;
               GetItemText( (HTREEITEM)lpCustomDraw->nmcd.dwItemSpec, strItemText );
		
               if ( !strItemText.IsEmpty() )
               {
                    CRect rcItemText( rcItem );
                    rcItemText.DeflateRect( 3, 0, 2, 0 );
			
                    //dcPaint.SelectFont( m_fntTreeFont );
                    dcPaint.SetTextColor( m_rgbItemText );
                    dcPaint.SetBkMode( TRANSPARENT );
                    dcPaint.DrawText( strItemText, strItemText.GetLength(), rcItemText, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
               }
		
               dcPaint.RestoreDC( nContextState );
		
               return CDRF_DODEFAULT;
          }
};

template <class T>
class ATL_NO_VTABLE CTreeWizard97SheetImpl : public CWizard97SheetImpl< T >
{
protected:
	CStatic m_ctlSeparator;
	CStatic m_ctlSeparator1;
	CWizardTreeCtrl m_ctlTree;
	CButton m_ctlApplyButton;
protected:
	// Typedefs
	typedef CTreeWizard97SheetImpl< T > thisClass;
	typedef CWizard97SheetImpl< T > baseClass;
private:
	static const int TREE_WIDTH = 170;
public:
	CTreeWizard97SheetImpl(ATL::_U_STRINGorID title, ATL::_U_STRINGorID headerBitmap, ATL::_U_STRINGorID watermarkBitmap, UINT uStartPage = 0, HWND hWndParent = NULL) :
	  baseClass(title, headerBitmap, watermarkBitmap, uStartPage, hWndParent){}

	  // Message Handling
	  BEGIN_MSG_MAP(thisClass)
		  MESSAGE_HANDLER(WM_TIMER, OnTimer)
		  CHAIN_MSG_MAP(baseClass)
		  REFLECT_NOTIFICATIONS()
	  END_MSG_MAP()

	  void OnSheetInitialized()
	  {
		  baseClass::OnSheetInitialized();
		  CRect rcTabCtrl = MoveTabWindow();

		  ModifyStyle(0,WS_MINIMIZEBOX, 0);
		  ModifyStyleEx(WS_EX_CONTEXTHELP, 0, 0);

		  CMenu sysmenu = GetSystemMenu( FALSE );
		  sysmenu.InsertMenu( -1, MF_BYPOSITION | MF_STRING,SC_ICON, "Minimize" );

		  SetTimer(0x9889, 10);
	  }

	  LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	  {
		  if ((UINT)wParam == 0x9889)
		  {
			  CreateTreeCtrl();
			  KillTimer(0x9889);
		  }

		  bHandled = FALSE;

		  return 0;
	  }

	  CRect MoveTabWindow()
	  {
		  CWindow wndTab = GetDlgItem(WIZ_IDTABCONTROL);

		  CRect rc;
		  wndTab.GetWindowRect(&rc);
		  ScreenToClient(&rc);

		  CRect rcBackup = rc;

		  rc.left += TREE_WIDTH;
		  wndTab.MoveWindow(&rc);

		  return rcBackup;
	  }

	  void CreateTreeCtrl()
	  {
		  CRect rcHeadCtrl, rcSeparaterCtrl;

		  CWindow wndHead = GetDlgItem(WIZ_IDHEAD);
		  wndHead.GetWindowRect(&rcHeadCtrl);

		  CWindow wndSeparater = GetDlgItem(WIZ_IDSEPERATER);
		  wndSeparater.GetWindowRect(&rcSeparaterCtrl);

		  GetDlgItem(WIZ_IDCANCEL).SetWindowText(_T("Close"));
		  GetDlgItem(WIZ_IDBACK).SetWindowText(_T("Back"));
		  GetDlgItem(WIZ_IDNEXT).SetWindowText(_T("Next"));
		  GetDlgItem(WIZ_IDFINISH).SetWindowText(_T("Finish"));
		  GetDlgItem(WIZ_IDHELP).SetWindowText(_T("Help"));
		  //GetDlgItem(WIZ_IDHELP).EnableWindow(false);

		  //Move Back Button
		  CRect rcBack;
		  GetDlgItem(WIZ_IDBACK).GetWindowRect(&rcBack);
		  ScreenToClient(&rcBack);

		  int widthBack = rcBack.Width() + 5;
		  rcBack.left -= widthBack;
		  rcBack.right -= widthBack;

		  GetDlgItem(WIZ_IDBACK).MoveWindow(rcBack);

		  //Move Next Button.
		  CRect rcNext;
		  GetDlgItem(WIZ_IDNEXT).GetWindowRect(&rcNext);
		  ScreenToClient(&rcNext);

		  int widthNext = rcNext.Width() + 5;
		  rcNext.left -= widthNext;
		  rcNext.right -= widthNext;

		  GetDlgItem(WIZ_IDNEXT).MoveWindow(rcNext);
		  GetDlgItem(WIZ_IDFINISH).MoveWindow(rcNext);

          //Create Apply Button
		  CRect rcCancel;
		  GetDlgItem(WIZ_IDCANCEL).GetWindowRect(rcCancel);
		  ScreenToClient(&rcCancel);

		  int widthCancel = rcCancel.Width();
		  rcCancel.left -= widthCancel;
		  rcCancel.right -= widthCancel;

		  m_ctlApplyButton.Create(m_hWnd, rcCancel, "Apply", WS_CHILD | WS_VISIBLE, 0, 0x9888, NULL);
		  m_ctlApplyButton.SetFont(GetFont());

		 // m_ctlTree.SetFont(GetFont());

		  ScreenToClient(&rcHeadCtrl);
		  ScreenToClient(&rcSeparaterCtrl);

		  CRect rc;

		  rc.left = 2;
		  rc.right = TREE_WIDTH - 1;
		  rc.top = rcHeadCtrl.bottom - 2;
		  rc.bottom = rcSeparaterCtrl.bottom - 3/* - 15*/;

		  m_ctlTree.Create(m_hWnd, rc, "TreeWizard", WS_VISIBLE | WS_TABSTOP | WS_CHILD | /*TVS_HASBUTTONS |*/ TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_SINGLEEXPAND,
			  NULL/*WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST*/, (HMENU)168, NULL);

		  //m_ctlTree.Initialise();
		  
		  m_ctlTree.SetFont(GetFont());
		  m_ctlTree.SetBkColor(::GetSysColor(COLOR_INFOBK));

		  rc.left = 0;
		  rc.right  = rcSeparaterCtrl.right;
		  rc.bottom = rcSeparaterCtrl.bottom;

		  m_ctlSeparator.Create(m_hWnd, rc, "", WS_CHILD|WS_VISIBLE|SS_ETCHEDFRAME|SS_ETCHEDHORZ    ,NULL, 0x1005, NULL);
		  m_ctlSeparator.SetFont(GetFont());

		  rc.left = TREE_WIDTH;
		  rc.right  = rcSeparaterCtrl.right;

		  m_ctlSeparator1.Create(m_hWnd, rc, "", WS_CHILD|WS_VISIBLE|SS_ETCHEDFRAME|SS_ETCHEDHORZ    ,NULL, 0x1005, NULL);
		  m_ctlSeparator1.SetFont(GetFont());

		  T *pT = static_cast<T*>(this);

		  pT->InitTreeCtrlContent();
	  }

	  void InitTreeCtrlContent()
	  {
	  }
};

#endif
