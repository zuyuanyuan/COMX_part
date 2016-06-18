#ifndef __BEVELLINE_H__
#define __BEVELLINE_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CBevelLine - Gradient label control implementation
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2000.
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
  #error BevelLine.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error BevelLine.h requires atlctrls.h to be included first
#endif

#if (_WIN32_IE < 0x0400)
  #error BevelLine.h requires _WIN32_IE >= 0x0400
#endif

#include <wtlwindowfont.h>

static const WTL::CString	s_strTwoSpaces	=	_T("  ");
static const CPoint	s_pnt11			=	CPoint(1,1);

template< class T, class TBase = CWindow, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CBevelLineImpl : public CWindowImpl< T, TBase, TWinTraits >
{
public:
  bool m_bSunken;
  int m_iLineWidth;
  int m_iLineHeight;

  CWindowFont m_fontItalic;

  CBevelLineImpl() : 
    m_bSunken(true),
    m_iLineWidth(1),
    m_iLineHeight(1)
  { };

  ~CBevelLineImpl()
  {
  }

  // Operations

  BOOL SubclassWindow(HWND hWnd)
  {
    ATLASSERT(m_hWnd == NULL);
    ATLASSERT(::IsWindow(hWnd));
    BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);

	if(m_fontItalic.Create(hWnd, CWindowFont::typeItalic | CWindowFont::typeBold))
		SetFont(m_fontItalic);

	WTL::CString strLogText = " Kingmesh";
	
	::SetWindowText(hWnd, strLogText);

	::EnableWindow(m_hWnd, FALSE);

    if( bRet ) Init();
    return bRet;
  }

  // Implementation

  void Init()
  {
    ATLASSERT(::IsWindow(m_hWnd));

    // Check if we should paint a label
    TCHAR lpszBuffer[10] = { 0 };
    if(::GetClassName(m_hWnd, lpszBuffer, 8)) {
      if(::lstrcmpi(lpszBuffer, _T("static")) == 0) {
        ModifyStyle(0, SS_NOTIFY);  // we need this
        DWORD dwStyle = GetStyle() & 0x000000FF;
        if(dwStyle == SS_ICON || dwStyle == SS_BLACKRECT || dwStyle == SS_GRAYRECT || 
            dwStyle == SS_WHITERECT || dwStyle == SS_BLACKFRAME || dwStyle == SS_GRAYFRAME || 
            dwStyle == SS_WHITEFRAME || dwStyle == SS_OWNERDRAW || 
            dwStyle == SS_BITMAP || dwStyle == SS_ENHMETAFILE)
          ATLASSERT("Invalid static style for bevel control"==NULL);
      }
    }
  }

  // Message map and handlers

  BEGIN_MSG_MAP(CBevelLineImpl)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    Init();
	return 0;
  }

  LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
  {
    T* pT = static_cast<T*>(this);
    if(wParam != NULL)
    {
      pT->DoPaint((HDC)wParam);
    }
    else
    {
      CPaintDC dc(m_hWnd);
      pT->DoPaint(dc.m_hDC);
    }
    return 0;
  }
  
  static inline CSize textExtent(CDCHandle & dc, WTL::CString s)
  {
	  CRect	r(0,10,0,10);
	  CSize	sz;
	  sz.cy	=	dc.DrawText(s,s.GetLength(),r,DT_LEFT|DT_SINGLELINE|DT_NOCLIP|DT_CALCRECT);
	  sz.cx	=	r.Width();
	  return sz;
  }

  static inline CRect makeTextRect(const CRect & rectClient, const CSize & sz, DWORD dwType, int iAngle)
  {
	  CRect	rectText;

	  switch(iAngle)
	  {
	  default:	assert(false);
	  case	0:

		  rectText.top		=	rectClient.top - sz.cy/2 - 2;
		  rectText.bottom	=	rectText.top + sz.cy;

		  switch(dwType)
		  {
		  default			:	assert(false);
		  case	SS_LEFT	:	rectText.left	=	rectClient.left;
			  rectText.right	=	rectClient.left + sz.cx;
			  break;
		  case	SS_CENTER:	rectText.left	=	rectClient.left + (rectClient.Width() - sz.cx) / 2;
			  rectText.right	=	rectText.left + sz.cx;
			  break;
		  case	SS_RIGHT	:	rectText.left	=	rectClient.right - sz.cx;
			  rectText.right	=	rectClient.right;
			  break;
		  }
		  break;

	  case	90:

		  rectText.left		=	rectClient.left - sz.cy/2 - 1;
		  rectText.right		=	rectText.left + sz.cy;

		  switch(dwType)
		  {
		  default			:	assert(false);
		  case	SS_RIGHT	:	rectText.top		=	rectClient.top;
			  rectText.bottom	=	rectClient.top + sz.cx;
			  break;
		  case	SS_CENTER:	rectText.top		=	rectClient.top + (rectClient.Height() - sz.cx) / 2;
			  rectText.bottom	=	rectText.top + sz.cx;
			  break;
		  case	SS_LEFT	:	rectText.top		=	rectClient.bottom - sz.cx;
			  rectText.bottom	=	rectClient.bottom;
			  break;
		  }
		  break;

	  case	270:

		  rectText.left		=	rectClient.left - sz.cy/2 + 1;
		  rectText.right		=	rectText.left + sz.cy;

		  switch(dwType)
		  {
		  default			:	assert(false);
		  case	SS_LEFT	:	rectText.top		=	rectClient.top;
			  rectText.bottom	=	rectClient.top + sz.cx;
			  break;
		  case	SS_CENTER:	rectText.top		=	rectClient.top + (rectClient.Height() - sz.cx) / 2;
			  rectText.bottom	=	rectText.top + sz.cx;
			  break;
		  case	SS_RIGHT	:	rectText.top		=	rectClient.bottom - sz.cx;
			  rectText.bottom	=	rectClient.bottom;
			  break;
		  }
		  break;
	  }

	  return rectText;
  }

  static inline void drawText(CDCHandle & dc, WTL::CString s, const CRect & r, const CPoint & pnt11, bool bDisabled)
  {
	  COLORREF	txtCol	=	dc.GetTextColor();
	  int		md			=	dc.SetBkMode(TRANSPARENT);

	  if(bDisabled)
	  {
		  dc.SetTextColor(::GetSysColor(COLOR_3DHIGHLIGHT));
		  dc.DrawText(s,s.GetLength(),r + pnt11,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		  dc.SetTextColor(::GetSysColor(COLOR_3DSHADOW));
	  }
	  else
		  dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));

	  dc.DrawText(s,s.GetLength(),(RECT *)(const RECT *)r,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	  dc.SetTextColor(txtCol);
	  dc.SetBkMode(md);
  }

  void DoPaint(CDCHandle dc)
  {
	  CRect	rectClient;
	  GetClientRect(rectClient);

	  COLORREF	oldBkCol		=	dc.GetBkColor();
	  HFONT		pOldFont	=	dc.SelectFont(GetFont());
	  COLORREF	hiCol			=	::GetSysColor(!m_bSunken ? COLOR_3DHIGHLIGHT : COLOR_3DSHADOW),
		  loCol			=	::GetSysColor(m_bSunken ? COLOR_3DHIGHLIGHT : COLOR_3DSHADOW);

	  dc.FillSolidRect(rectClient,::GetSysColor(COLOR_BTNFACE));

	  // draw bevel

	  CRect		rectBevel;
	  bool		bHorz;

	  if(rectClient.Width() > rectClient.Height())
	  {
		  rectBevel.left		=	rectClient.left;
		  rectBevel.right	=	rectClient.right;
		  rectBevel.top		=	rectClient.top + rectClient.Height() / 2;
		  rectBevel.bottom	=	rectBevel.top + 2;
		  bHorz					=	true;
	  }
	  else
	  {
		  rectBevel.left		=	rectClient.left + rectClient.Width() / 2;
		  rectBevel.right	=	rectBevel.left + 2;
		  rectBevel.top		=	rectClient.top;
		  rectBevel.bottom	=	rectClient.bottom;
		  bHorz					=	false;
	  }

	  dc.Draw3dRect(rectBevel,hiCol,loCol);

	  // add some text

	  char szText[1024];
	  ::GetWindowText(m_hWnd , szText , 1024);
      WTL::CString	s(szText);

	  if(!s.IsEmpty())
	  {
		  // interpret my ~ stuff (even for horizontals where we cut it off only)

		  int	iAngle;

		  if(s[0] == _T('~'))
		  {
			  s			=	s.Mid(1);
			  iAngle	=	bHorz ? 0 : 270;
		  }
		  else
			  iAngle	=	bHorz ? 0 : 90;

		  // get proper style

		  DWORD	dwType	=	GetStyle() & SS_TYPEMASK;

		  switch(dwType)
		  {
		  default:				dwType	=	SS_LEFT;
		  case	SS_LEFT	:	break;
		  case	SS_RIGHT	:	break;
		  case	SS_CENTER:	break;
		  }

		  if(dwType != SS_LEFT)
			  s	=	s_strTwoSpaces + s;
		  if(dwType != SS_RIGHT)
			  s	+=	s_strTwoSpaces;

		  // get proper rect

		  CSize	sz			=	textExtent(dc,s);
		  CRect	rectText	=	makeTextRect(rectBevel,sz,dwType,iAngle);

		  ++rectText.bottom;
		  if(dwType == SS_RIGHT)		// ensure disabled text is drawn properly
			  --rectText.left;

		  dc.IntersectClipRect(rectText);
		  dc.FillSolidRect(rectText,::GetSysColor(COLOR_BTNFACE));

		  if(bHorz)
		  {
			  drawText(dc,s,rectText,s_pnt11,(GetStyle() & WS_DISABLED) != 0);
		  }
	  }

	  dc.SelectFont(pOldFont);
	  dc.SetBkColor(oldBkCol);
  }
};


class CBevelLine : public CBevelLineImpl<CBevelLine>
{
public:
  DECLARE_WND_CLASS(_T("WTL_BevelLine"))
};


#endif //__BEVELLINE_H__
