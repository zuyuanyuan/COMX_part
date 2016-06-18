/////////////////////////////////////////////////////////////////////////////////////////
// 
//	MultiPaneStatusBarEx.h
//
//  Author: Pablo Aliskevicius.
//  Copyright (C) 2004 Pablo Aliskevicius.
//
//  The code and information  is provided  by the  author and  copyright  holder 'as-is',
//  and any express or implied  warranties,  including,  but not  limited to, the implied
//  warranties of  merchantability and  fitness for a particular purpose  are disclaimed.
//  In no event shall the author or copyright holders be liable for any direct, indirect,
//  incidental, special, exemplary, or consequential damages (including,  but not limited
//  to, procurement of substitute goods or services;  loss of use,  data, or profits;  or
//  business  interruption)  however caused  and on any  theory of liability,  whether in
//  contract, strict liability,  or tort  (including negligence or otherwise)  arising in
//  any way out of the use of this software,  even if advised of the  possibility of such
//  damage.
//
/////////////////////////////////////////////////////////////////////////////////////////
//
// Classes in this file:
//
// CProgressBarInPaneImpl<T> - CMPSBarWithProgress, 
//							   CMPSBarWithProgressAndBMP, 
//                            [CMPSBarWithProgressAndAnimation - not implemented],
//							   CMPSBarWithAll
// CBitmapInPaneImpl<T>     -  CMPSBarWithBitmaps,
//                     		   CMPSBarWithProgressAndBMP,
//           				  [CMPSBarWithAnimAndBMP - not implemented ],
//							   CMPSBarWithAll
// CAnimationInPaneImpl<T>   - CMPSBarWithAnimation,
//                            [CMPSBarWithProgressAndAnimation - not implemented],
//           				  [CMPSBarWithAnimAndBMP - not implemented ],
//                             CMPSBarWithAll

// MultiPaneStatusBarWithProgress.h: interface for the CMPSBarWithProgress class.
//
// This class extends WTL::CMultiPaneStatusBarCtrl to support creating and updating a 
// progress bar in one of its panes, and moving that progress bar around.
//
// Usage:
// ======
//
//    Wherever you'd create a CMultiPaneStatusBarCtrl, create one of the CMPSBarWithXXXXX
//    classes instead (the one which provides the functionality you'll use).
//
//    Showing progress bars:
//	  ----------------------
//
//	     Show a progress bar using ProgCreate()	
//		 Hide it using ProgDestroyWindow()
//		 Display progress using ProgSetPos(), or ProgStepIt() after	ProgSetStep()
//		 All other functions of WTL::CProgressBarCtrl, version 7.1, are also exposed.
//
//    Showing bitmaps:
//	  ----------------
//
//		 Use SetBitmap() to display a bitmap. There are two versions of the function.
//		 Use DestroyBitmap() to hide the bitmap, and optionally call DestroyObject() 
//		 on its handle.
//
//    Showing animations:
//	  -------------------
//
//		 Use AnimCreate() to display an animation.
//	  	 Use AnimDestroyWindow() to close an animation.
//
//    Showing anything (I don't like it, so there's an #ifdef):
//	  ---------------------------------------------------------
//
//       #define MULTI_PANE_WITH_ANY before including this file.
//
//		 Use CMPSBarWithAll as your member variable.
//
//		 Use AttachWindow() to attach an HWND to a pane.
//		 Use DetachWindow() to detach that HWND from that pane.
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_)
#define AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ATLCTRLS_H__
#error You have to include <ATLCTRLS.H> for CProgressBarCtrl, CStatic and CAnimateCtrl.
#endif
#ifndef __ATLCTRLX_H__
#error You have to include <ATLCTRLX.H> for CMultiPaneStatusBarCtrlImpl<T>.
#endif

#define	IDT_INDETERMINATE		100
#define	IND_BAND_WIDTH			15

// Funtion prototypes.
inline COLORREF LightenColor(const COLORREF crColor, BYTE byIncreaseVal);
inline COLORREF DarkenColor(const COLORREF crColor, BYTE byReduceVal);

//-------------------------------------------------------------------
//
inline COLORREF LightenColor(const COLORREF crColor, BYTE byIncreaseVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Lightens a color by increasing the RGB values by the given number.
//
{
    BYTE byRed = GetRValue(crColor);
    BYTE byGreen = GetGValue(crColor);
    BYTE byBlue = GetBValue(crColor);

    if ((byRed + byIncreaseVal) <= 255)
        byRed = BYTE(byRed + byIncreaseVal);
    if ((byGreen + byIncreaseVal)	<= 255)
        byGreen = BYTE(byGreen + byIncreaseVal);
    if ((byBlue + byIncreaseVal) <= 255)
        byBlue = BYTE(byBlue + byIncreaseVal);

    return RGB(byRed, byGreen, byBlue);
}	// LightenColorref

//-------------------------------------------------------------------
//
inline COLORREF DarkenColor(const COLORREF crColor, BYTE byReduceVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Darkens a color by reducing the RGB values by the given number.
//
{
    BYTE byRed = GetRValue(crColor);
    BYTE byGreen = GetGValue(crColor);
    BYTE byBlue = GetBValue(crColor);

    if (byRed >= byReduceVal)
        byRed = BYTE(byRed - byReduceVal);
    if (byGreen >= byReduceVal)
        byGreen = BYTE(byGreen - byReduceVal);
    if (byBlue >= byReduceVal)
        byBlue = BYTE(byBlue - byReduceVal);

    return RGB(byRed, byGreen, byBlue);
}	// DarkenColorref


///////////////////////////////////////////////////////////////////////////////
// CProgressBarCtrl

template <class TBase>
class CIndProgressBarCtrlT : public CWindowImpl<CIndProgressBarCtrlT<TBase> ,CProgressBarCtrl>
{
public:
    typedef CWindowImpl<CIndProgressBarCtrlT<TBase> ,CProgressBarCtrl> baseClass;
public:
    // Constructors
    CIndProgressBarCtrlT(HWND hWnd = NULL)
    {
        m_hWnd = hWnd;
        m_bIndeterminate = true;
        m_nIndOffset = 0;
        m_crColor = RGB(50, 50, 255);//::GetSysColor(COLOR_HIGHLIGHT);
        GetColors();
        CreatePens();
    }

    ~CIndProgressBarCtrlT()
    {
        DeletePens();
    }

    CIndProgressBarCtrlT< TBase >& operator =(HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

public:
    BEGIN_MSG_MAP(baseClass)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
        //MESSAGE_HANDLER(WM_TIMER, OnTimer)
    END_MSG_MAP()
public:
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        static bool flag = false;
        if (!flag)
        {
            flag = true;
            EnableIndeterminate(m_bIndeterminate);
        }

        if (!m_bIndeterminate)
        {
            bHandled = FALSE;//CProgressCtrl::OnPaint();
            return 0;
        }

        CPaintDC dcPaint(m_hWnd); // device context for painting
        // TODO: 在此处添加消息处理程序代码
        // 不为绘图消息调用 CProgressCtrl::OnPaint()

        CRect rect, rectClient;
        GetClientRect(rectClient);
        CRect oldRect = rectClient;
        rectClient.DeflateRect(1, 1, 1, 1);

        rectClient.left += 1;
        rectClient.top += 1;
        rectClient.bottom += 1;
        rectClient.right += 1;

        rect = rectClient;
        BOOL bVertical = GetStyle() & PBS_VERTICAL;

        // Create a memory DC for drawing.
        CDC dc;
        dc.CreateCompatibleDC(dcPaint);
        int nSavedDC = dc.SaveDC();
        CBitmap bmp;
        bmp.CreateCompatibleBitmap(dcPaint, rect.Width(), rect.Height());
        HBITMAP pOldBmp = dc.SelectBitmap(bmp);

        CBrush br1;
        br1.CreateSolidBrush(m_crColorLightest);
        CBrush br2;
        br2.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
        dc.FillRect(rect, br2);

        int nLower, nUpper;
        GetRange(nLower, nUpper);

        // Determine the size of the bar and draw it.
        if (bVertical)
        {
            if (!m_bIndeterminate)
                rect.top = rect.bottom - int(((float)rect.Height() * float(GetPos() - nLower)) / float(nUpper - nLower));
            dc.FillRect(rect, br1);
            DrawVerticalBar(&dc, rect);
        }
        else
        {
            if (!m_bIndeterminate)
                rect.right = int(((float)rect.Width() * float(GetPos() - nLower)) / float(nUpper - nLower));
            /*rect.bottom -= 1;
            rect.top += 1;
            rect.left += 1;
            rect.right -= 1;*/
            dc.FillRect(rect, br1);
            DrawHorizontalBar(&dc, rect);

        }

        dcPaint.Rectangle(oldRect);

        dcPaint.BitBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), 
            dc, rectClient.left, rectClient.top, SRCCOPY);

        CBrush br;
        br.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
        dcPaint.FrameRect(oldRect, br);

        dc.SelectBitmap(pOldBmp);
        dc.RestoreDC(nSavedDC);
        dc.DeleteDC();

        bHandled = TRUE;
        return 0;
    }
    LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if (!m_bIndeterminate)
        {
            bHandled = FALSE;//CProgressCtrl::OnEraseBkgnd(pDC);
        }
        
        return 0;
    }
    //LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
    //{
    //    UINT nIDEvent = (UINT)wParam;
    //    if (nIDEvent == IDT_INDETERMINATE)
    //    {
    //        KillTimer(nIDEvent);

    //        if (++m_nIndOffset > IND_BAND_WIDTH - 1)
    //            m_nIndOffset = 0;
    //        /*if (--m_nIndOffset < 0)
    //        m_nIndOffset = IND_BAND_WIDTH - 1;*/
    //        RedrawWindow();

    //        SetTimer(IDT_INDETERMINATE, 25, NULL);
    //    }

    //    bHandled = TRUE;
    //    return 0;
    //}
private:
    int m_nIndOffset;
    bool m_bIndeterminate;
    CPen m_penColor;
    CPen m_penColorLight;
    CPen m_penColorLighter;
    CPen m_penColorDark;
    CPen m_penColorDarker;
    CPen m_penDkShadow;
    CPen m_penShadow;
    CPen m_penLiteShadow;
    COLORREF m_crColor;
    COLORREF m_crColorLight;
    COLORREF m_crColorLighter;
    COLORREF m_crColorLightest;
    COLORREF m_crColorDark;
    COLORREF m_crColorDarker;
    COLORREF m_crDkShadow;
    COLORREF m_crShadow;
    COLORREF m_crLiteShadow;
private:
    void DrawHorizontalBar(CDC *pDC, const CRect rect)
    {
        if (!rect.Width())
            return;

        int nLeft = rect.left;
        int nTop = rect.top;
        int nBottom = rect.bottom;

        // Assume we're not drawing the indeterminate state.
        HPEN pOldPen = pDC->SelectPen(m_penColorLight);

        if (m_bIndeterminate)
        {
            pOldPen = pDC->SelectPen(m_penColor);
            int nNumBands = (rect.Width() / IND_BAND_WIDTH) + 2;
            int nHeight = rect.Height() + 1;

            int nAdjust = nLeft - IND_BAND_WIDTH + m_nIndOffset;
            int nXpos = 0;
            int nYpos1 = nTop + 1;
            int nYpos2 = nBottom - 2;

            for (int i = 0; i < nNumBands; i++)
            {
                nXpos = nAdjust + (i * IND_BAND_WIDTH);

                pDC->SelectPen(m_penColorDarker);
                pDC->MoveTo(nXpos + 1, nTop);
                pDC->LineTo(nXpos + nHeight, nBottom);

                pDC->SelectPen(m_penColorDark);
                pDC->MoveTo(nXpos + 2, nTop);
                pDC->LineTo(nXpos + nHeight + 1, nBottom);
                pDC->MoveTo(nXpos + 10, nTop);
                pDC->LineTo(nXpos + nHeight + 9, nBottom);

                pDC->SelectPen(m_penColor);
                pDC->MoveTo(nXpos + 3, nTop);
                pDC->LineTo(nXpos + nHeight + 2, nBottom);
                pDC->MoveTo(nXpos + 9, nTop);
                pDC->LineTo(nXpos + nHeight + 8, nBottom);

                pDC->SelectPen(m_penColorLight);
                pDC->MoveTo(nXpos + 4, nTop);
                pDC->LineTo(nXpos + nHeight + 3, nBottom);
                pDC->MoveTo(nXpos + 8, nTop);
                pDC->LineTo(nXpos + nHeight + 7, nBottom);

                pDC->SelectPen(m_penColorLighter);
                pDC->MoveTo(nXpos + 5, nTop);
                pDC->LineTo(nXpos + nHeight + 4, nBottom);
                pDC->MoveTo(nXpos + 7, nTop);
                pDC->LineTo(nXpos + nHeight + 6, nBottom);

                pDC->SelectPen(m_penColorLighter);
                pDC->MoveTo(nXpos + 6, nTop);
                pDC->LineTo(nXpos + nHeight + 5, nBottom);
                pDC->MoveTo(nXpos + 6, nTop);
                pDC->LineTo(nXpos + nHeight + 5, nBottom);

            }	// for the number of bands
        }	// if indeterminate
        else
        {
            int nRight = rect.right;

            pDC->MoveTo(nLeft + 2, nBottom - 4);
            pDC->LineTo(nRight - 2, nBottom - 4);
            pDC->MoveTo(nLeft + 2, nTop + 2);
            pDC->LineTo(nRight - 2, nTop + 2);
            pDC->SetPixel(nLeft + 1, nBottom - 3, m_crColorLight);
            pDC->SetPixel(nLeft + 1, nTop + 1, m_crColorLight);

            pDC->SelectPen(m_penColorLighter);
            pDC->MoveTo(nLeft + 2, nBottom - 5);
            pDC->LineTo(nRight - 3, nBottom - 5);
            pDC->LineTo(nRight - 3, nTop + 3);
            pDC->LineTo(nLeft + 1, nTop + 3);
            pDC->SetPixel(nLeft + 1, nBottom - 4, m_crColorLighter);
            pDC->SetPixel(nLeft + 1, nTop + 2, m_crColorLighter);

            pDC->SelectPen(m_penColor);
            pDC->MoveTo(nLeft, nBottom - 1);
            pDC->LineTo(nLeft, nTop);
            pDC->LineTo(nLeft + 2, nTop);
            pDC->SetPixel(nLeft + 1, nBottom - 2, m_crColor);
            pDC->MoveTo(nLeft + 2, nBottom - 3);
            pDC->LineTo(nRight - 2, nBottom - 3);
            pDC->MoveTo(nLeft + 2, nTop + 1);
            pDC->LineTo(nRight - 1, nTop + 1);

            pDC->SelectPen(m_penColorDark);
            pDC->MoveTo(nLeft + 2, nBottom - 2);
            pDC->LineTo(nRight - 2, nBottom - 2);
            pDC->LineTo(nRight - 2, nTop + 1);
            pDC->MoveTo(nLeft + 2, nTop);
            pDC->LineTo(nRight, nTop);
            pDC->SetPixel(nLeft + 1, nBottom - 1, m_crColorDark);

            pDC->SelectPen(m_penColorDarker);
            pDC->MoveTo(nLeft + 2, nBottom - 1);
            pDC->LineTo(nRight - 1, nBottom - 1);
            pDC->LineTo(nRight - 1, nTop);

            pDC->SelectPen(m_penShadow);
            pDC->MoveTo(nRight, nTop);
            pDC->LineTo(nRight, nBottom);

            pDC->SelectPen(m_penLiteShadow);
            pDC->MoveTo(nRight + 1, nTop);
            pDC->LineTo(nRight + 1, nBottom);
        }	// if not indeterminate

        pDC->SelectPen(pOldPen);
    }
    void DrawVerticalBar(CDC *pDC, const CRect rect)
    {
        int nHeight = rect.Height();
        if (!nHeight)
            return;

        int nLeft = rect.left;
        int nTop = rect.top;
        int nRight = rect.right;
        int nBottom = rect.bottom;

        HPEN pOldPen = pDC->SelectPen(m_penColor);

        if (m_bIndeterminate)
        {
            int nNumBands = (nHeight / IND_BAND_WIDTH) + 2;
            int nHeight = rect.Width() + 1;

            int nAdjust = nBottom - m_nIndOffset;
            int nXpos1 = nLeft;
            int nXpos2 = nRight + 1;
            int nYpos = nTop + 1;

            for (int i = 0; i < nNumBands; i++)
            {
                nYpos = nAdjust - (i * IND_BAND_WIDTH);

                pDC->SelectPen(m_penColorDarker);
                pDC->MoveTo(nXpos1, nYpos);
                pDC->LineTo(nXpos2, nYpos + nHeight);

                pDC->SelectPen(m_penColorDark);
                pDC->MoveTo(nXpos1, nYpos + 1);
                pDC->LineTo(nXpos2, nYpos + nHeight + 1);
                pDC->MoveTo(nXpos1, nYpos + 9);
                pDC->LineTo(nXpos2, nYpos + nHeight + 9);

                pDC->SelectPen(m_penColor);
                pDC->MoveTo(nXpos1, nYpos + 2);
                pDC->LineTo(nXpos2, nYpos + nHeight + 2);
                pDC->MoveTo(nXpos1, nYpos + 8);
                pDC->LineTo(nXpos2, nYpos + nHeight + 8);

                pDC->SelectPen(m_penColorLight);
                pDC->MoveTo(nXpos1, nYpos + 3);
                pDC->LineTo(nXpos2, nYpos + nHeight + 3);
                pDC->MoveTo(nXpos1, nYpos + 7);
                pDC->LineTo(nXpos2, nYpos + nHeight + 7);

                pDC->SelectPen(m_penColorLighter);
                pDC->MoveTo(nXpos1, nYpos + 4);
                pDC->LineTo(nXpos2, nYpos + nHeight + 4);
                pDC->MoveTo(nXpos1, nYpos + 6);
                pDC->LineTo(nXpos2, nYpos + nHeight + 6);

            }	// for the number of bands
        }	// if indeterminate
        else
        {
            if (nHeight > 3)
            {
                pDC->MoveTo(nLeft, nTop + 1);
                pDC->LineTo(nLeft, nTop);
                pDC->LineTo(nRight, nTop);
                pDC->MoveTo(nLeft + 1, nBottom - 2);
                pDC->LineTo(nLeft + 1, nTop + 1);
                pDC->MoveTo(nRight - 3, nBottom - 3);
                pDC->LineTo(nRight - 3, nTop + 1);
                pDC->SetPixel(nRight - 2, nTop + 1, m_crColor);

                pDC->SelectPen(m_penColorLight);
                pDC->MoveTo(nLeft + 2, nBottom - 3);
                pDC->LineTo(nLeft + 2, nTop + 1);
                pDC->MoveTo(nRight - 4, nBottom - 3);
                pDC->LineTo(nRight - 4, nTop + 1);
                pDC->SetPixel(nLeft + 1, nTop + 1, m_crColorLight);
                pDC->SetPixel(nRight - 3, nTop + 1, m_crColorLight);

                pDC->SelectPen(m_penColorLighter);
                pDC->MoveTo(nLeft + 3, nBottom - 3);
                pDC->LineTo(nLeft + 3, nTop + 1);
                pDC->MoveTo(nRight - 5, nBottom - 3);
                pDC->LineTo(nRight - 5, nTop + 1);
                pDC->SetPixel(nLeft + 2, nTop + 1, m_crColorLighter);
                pDC->SetPixel(nRight - 4, nTop + 1, m_crColorLighter);

                pDC->SelectPen(m_penColorDark);
                pDC->MoveTo(nLeft, nBottom - 1);
                pDC->LineTo(nLeft, nTop + 1);
                pDC->MoveTo(nLeft + 2, nBottom - 2);
                pDC->LineTo(nRight - 2, nBottom - 2);
                pDC->LineTo(nRight - 2, nTop + 1);
                pDC->SetPixel(nRight - 1, nTop + 1, m_crColorDark);

                pDC->SelectPen(m_penColorDarker);
                pDC->MoveTo(nLeft + 1, nBottom - 1);
                pDC->LineTo(nRight - 1, nBottom - 1);
                pDC->LineTo(nRight - 1, nTop + 1);
            }
            else
            {
                CBrush br;
                br.CreateSolidBrush(m_crColor);
                HBRUSH pOldBrush = pDC->SelectBrush(br);
                pDC->SelectPen(m_penColorDark);
                pDC->Rectangle(rect);
                pDC->SelectBrush(pOldBrush);
            }
        }	// if not indeterminate

        pDC->SelectPen(pOldPen);
    }
    void DeletePens()
    {
        if (m_penColorLight.m_hPen)
            m_penColorLight.DeleteObject();
        if (m_penColorLighter.m_hPen)
            m_penColorLighter.DeleteObject();
        if (m_penColor.m_hPen)
            m_penColor.DeleteObject();
        if (m_penColorDark.m_hPen)
            m_penColorDark.DeleteObject();
        if (m_penColorDarker.m_hPen)
            m_penColorDarker.DeleteObject();
        if (m_penDkShadow.m_hPen)
            m_penDkShadow.DeleteObject();
        if (m_penShadow.m_hPen)
            m_penShadow.DeleteObject();
        if (m_penLiteShadow.m_hPen)
            m_penLiteShadow.DeleteObject();
    }
    void CreatePens()
    {
        DeletePens();

        m_penColorLight.CreatePen(PS_SOLID, 1, m_crColorLight);
        m_penColorLighter.CreatePen(PS_SOLID, 1, m_crColorLighter);
        m_penColor.CreatePen(PS_SOLID, 1, m_crColor);
        m_penColorDark.CreatePen(PS_SOLID, 1, m_crColorDark);
        m_penColorDarker.CreatePen(PS_SOLID, 1, m_crColorDarker);
        m_penDkShadow.CreatePen(PS_SOLID, 1, m_crDkShadow);
        m_penShadow.CreatePen(PS_SOLID, 1, m_crShadow);
        m_penLiteShadow.CreatePen(PS_SOLID, 1, m_crLiteShadow);
    }
    void GetColors()
    {
        m_crColorLight = LightenColor(m_crColor, 51);
        m_crColorLighter = LightenColor(m_crColorLight, 51);
        m_crColorLightest = LightenColor(m_crColorLighter, 51);
        m_crColorDark = DarkenColor(m_crColor, 51);
        m_crColorDarker = DarkenColor(m_crColorDark, 51);
        m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
        m_crLiteShadow = ::GetSysColor(COLOR_3DSHADOW);

        // Get a color halfway between COLOR_3DDKSHADOW and COLOR_3DSHADOW
        BYTE byRed3DDkShadow = GetRValue(m_crDkShadow);
        BYTE byRed3DLiteShadow = GetRValue(m_crLiteShadow);
        BYTE byGreen3DDkShadow = GetGValue(m_crDkShadow);
        BYTE byGreen3DLiteShadow = GetGValue(m_crLiteShadow);
        BYTE byBlue3DDkShadow = GetBValue(m_crDkShadow);
        BYTE byBlue3DLiteShadow = GetBValue(m_crLiteShadow);

        m_crShadow = RGB(byRed3DLiteShadow + ((byRed3DDkShadow - byRed3DLiteShadow) >> 1),
            byGreen3DLiteShadow + ((byGreen3DDkShadow - byGreen3DLiteShadow) >> 1),
            byBlue3DLiteShadow + ((byBlue3DDkShadow - byBlue3DLiteShadow) >> 1));
    }
public:
    bool IsIndeterminate()
    {
        return m_bIndeterminate;
    }
    void EnableIndeterminate(bool bIndeterminate = true)
    {
        m_bIndeterminate = bIndeterminate;

        /*if (m_bIndeterminate)
        {
            CRect rect;
            GetClientRect(rect);
            m_nIndOffset = 0;

            RedrawWindow();
            SetTimer(IDT_INDETERMINATE, 125, NULL);
        }
        else
        {
            KillTimer(IDT_INDETERMINATE);
            RedrawWindow();
        }*/
    }
    COLORREF GetColor()
    {
        return m_crColor;
    }
    void SetColor(COLORREF crColor)
    {
        m_crColor = crColor;
        GetColors();
        CreatePens();
        RedrawWindow();
    }
public:
    int SetPos(int nPos)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int ret = baseClass::SetPos(nPos);

        if (m_bIndeterminate)
        {
            m_nIndOffset = baseClass::GetPos();
            m_nIndOffset %= IND_BAND_WIDTH;
            RedrawWindow();
        }

        return ret;
    }

    int OffsetPos(int nPos)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int ret = baseClass::OffsetPos(nPos);

        if (m_bIndeterminate)
        {
            m_nIndOffset = baseClass::GetPos();
            m_nIndOffset %= IND_BAND_WIDTH;
            RedrawWindow();
        }

        return ret;
    }

    int SetStep(int nStep)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int ret = baseClass::SetStep(nStep);

        if (m_bIndeterminate)
        {
            m_nIndOffset = baseClass::GetPos();
            m_nIndOffset %= IND_BAND_WIDTH;
            RedrawWindow();
        }

        return ret;
    }
    int StepIt()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int ret = baseClass::StepIt();

        if (m_bIndeterminate)
        {
            m_nIndOffset = baseClass::GetPos();
            m_nIndOffset %= IND_BAND_WIDTH;
            RedrawWindow();
        }

        return ret;
    }
};

typedef CIndProgressBarCtrlT<ATL::CWindow>   CIndProgressBarCtrl;

/////////////////////////////////////////////////////////////////////////////////////////
// Implementation classes, meant to be inherited from (mixins).
/////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class CProgressBarInPaneImpl
{
	public:
		typedef CProgressBarInPaneImpl<T> CThisClass;
		// Constructor, destructor...
		CProgressBarInPaneImpl<T>(): m_iProgressPane(-1) 
		{
		}
		virtual ~CProgressBarInPaneImpl<T>() 
		{
			ProgDestroyWindow();
		}
		// UpdatePanesLayout() override, to handle resizing the progress bar whenever relevant
		BOOL UpdatePanesLayout(void)
		{
			if (m_iProgressPane != -1)
			{
				T* pt = static_cast<T*>(this);
				RECT rc;
				pt->GetRect(m_iProgressPane, &rc);
				// ::InflateRect(&rc, -1, -1); 
				m_Progress.MoveWindow(&rc);
			}
			return TRUE; // Mixed function.
		}
		
		BEGIN_MSG_MAP(CProgressBarInPaneImpl<T>)
			MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
            /*MESSAGE_HANDLER(WM_PAINT, OnPaint)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
            MESSAGE_HANDLER(WM_TIMER, OnTimer)*/
            //CHAIN_MSG_MAP_MEMBER(m_Progress)
		END_MSG_MAP()
			
		LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			if (::IsWindow(m_Progress.m_hWnd))
				m_Progress.ShowWindow(wParam ? SW_HIDE: SW_SHOW);

			bHandled = FALSE;
			return 0;
		}

        /*LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
            if (::IsWindow(m_Progress.m_hWnd))
                m_Progress.OnPaint(uMsg, wParam, lParam, bHandled);

            bHandled = FALSE;
            return 0;
        }

        LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
            if (::IsWindow(m_Progress.m_hWnd))
                m_Progress.OnEraseBkgnd(uMsg, wParam, lParam, bHandled);

            bHandled = FALSE;
            return 0;
        }

        LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
            if (::IsWindow(m_Progress.m_hWnd))
                m_Progress.OnTimer(uMsg, wParam, lParam, bHandled);

            bHandled = FALSE;
            return 0;
        }*/
		//////////////////////////////////////////////////////////////////////////////////
		// Prog* functions enable access to the contained CProgressBarCtrl. 
		// We'll expose most of its functionality, and a few additional bits.
		//////////////////////////////////////////////////////////////////////////////////

		BOOL ProgCreate(int iPane,         // Status pane where we'll create the progress bar.
			int nMin = 0, int nMax = 100,  // Progress bar initial range
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | PBS_SMOOTH, // Progress bar styles
			DWORD dwExStyle = 0
			) 
		{
			// Check there is such a pane
			T* pt = static_cast<T*>(this);
			ATLASSERT(::IsWindow(pt->m_hWnd));
			if (iPane >= pt->m_nPanes)
				return FALSE;
			// Check there is not a progress bar already open.
			if (::IsWindow(m_Progress.m_hWnd))
				return FALSE;
			// Get the pane's rectangle
			RECT rc;
			pt->GetRect( iPane, &rc );
			 //::InflateRect(&rc, -1, -3); 

			if (iPane == pt->m_nPanes - 1)
			{
				rc.right += 16;
			}
			
			// Create the window, using the status bar (this) as a parent.
            
			m_OriginProgress.Create ( pt->m_hWnd, rc, NULL, dwStyle,  dwExStyle);
            m_Progress.SubclassWindow(m_OriginProgress);
			// Set the progress bar's range and position
			m_Progress.SetRange ( nMin, nMax ); 
			m_Progress.SetPos ( nMin );   
			m_Progress.SetStep ( 1 );
			// Hold this, we'll need it to move around.
			m_iProgressPane = iPane;
			return TRUE;
		}
		// This function can be used to close a progress bar, after ending whatever 
		// lengthy operation justified opening it to begin with.
		void ProgDestroyWindow(void)
		{
			if (::IsWindow(m_Progress.m_hWnd))
			{
                /*m_OriginProgress.ShowWindow(SW_HIDE);
                m_OriginProgress.DestroyWindow();*/

				m_Progress.ShowWindow(SW_HIDE);
				m_Progress.DestroyWindow();
			}
			m_iProgressPane = -1;
            m_OriginProgress.m_hWnd = NULL;
			m_Progress.m_hWnd = NULL;
		}
		// Just in case. 
		int   ProgGetPane() const                 { return m_iProgressPane; }
		//////////////////////////////////////////////////////////////////////////////////
		// CProgressBarCtrl functionality (WTL version 7.1):
		// CWindow functionality in CProgressBarCtrl is hidden by design.
		//
		DWORD ProgSetRange(int nLower, int nUpper)  { return m_Progress.SetRange(nLower, nUpper); }
		int   ProgSetPos(int nPos)                  { return m_Progress.SetPos(nPos); }
		int   ProgOffsetPos(int nPos)               { return m_Progress.OffsetPos(nPos); }
		int   ProgSetStep(int nStep)                { return m_Progress.SetStep(nStep); }
		UINT  ProgGetPos() const                    { return m_Progress.GetPos(); }
		
		void  ProgGetRange(PPBRANGE pPBRange) const { m_Progress.GetRange(pPBRange); } 
		int   ProgGetRangeLimit(BOOL bLimit)  const { return m_Progress.GetRangeLimit(bLimit); }
		DWORD ProgSetRange32(int nMin, int nMax)    { return m_Progress.SetRange32(nMin, nMax); }
		
#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
		COLORREF ProgSetBarColor(COLORREF clr)      { return m_Progress.SetBarColor(clr); }
		COLORREF ProgSetBkColor(COLORREF clr)       { return m_Progress.SetBkColor(clr); }
#endif //(_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
		
#if (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
		BOOL ProgSetMarquee(BOOL bMarquee, UINT uUpdateTime = 0U)
													{ return m_Progress.SetMarquee(bMarquee, uUpdateTime); }
#endif //(_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
		int ProgStepIt()                            { return m_Progress.StepIt(); }
		
	protected:
	//////////////////////////////////////////////////////////////////////////////////
	// Member variables (of course, protected)
	//
		CProgressBarCtrl m_OriginProgress;  // This is the contained control.
        CIndProgressBarCtrl m_Progress;
		int m_iProgressPane;          // Pane ordinal where the progress bar resides, or -1 when off.
public:
    void EnableIndeterminate(const bool flag = true){m_Progress.EnableIndeterminate(flag);}
}; // CProgressBarInPaneImpl


// This one also handles bitmaps

template <class T> class CBitmapInPaneImpl
{
public:
	// You can have up to 15 bitmaps per status bar.
	enum { MAX_MANAGED = 15 };

	BEGIN_MSG_MAP(CBitmapInPaneImpl<T>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
	
	// Show bitmaps when the status bar is in multi pane state, hide them when it's simple.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int sw = (wParam) ? SW_HIDE: SW_SHOW;
		// Hide all children (static bitmaps)
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
				::ShowWindow(m_Manager[i].hw, sw);
		}
		bHandled = false;
		return 0;
	}
	
	// Move bitmaps around to stay on top of their panes.
	BOOL UpdatePanesLayout(void)
	{
		RECT rc;	
		T *pT = static_cast<T*>(this);
		for (int i = 0;   i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
			{
				pT->GetRect( m_Manager[i].iPane, &rc );
				// ::InflateRect(&rc, -1, -1); 
				CStatic cs(m_Manager[i].hw);
				cs.MoveWindow(&rc);
				cs.Invalidate();
			}
		} // for
		return TRUE;
	}
	// Add a bitmap to the status bar, in a pane chosen by ordinal.
	BOOL SetBitmap(int iPane,    // Zero based ordinal (not resource ID) of the chosen pane.
		           HBITMAP hbmp, // Handle to a bitmap.
		           bool bManage = false) // If true, DestroyObject() will be called on the HBITMAP.
	{
		if (NULL == hbmp)
			return false;
		// If the pane was in use, release it.
		if (!DestroyBitmap(iPane))
			return FALSE;
		// Use the first available array entry to put the selected bitmap.
		T *pT = static_cast<T *>(this);
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (!m_Manager[i].hw)
			{
				RECT rct;
				pT->GetRect( iPane, &rct);
				// ::InflateRect(&rct, -1, -1);
				CStatic cs;
				cs.Create(pT->m_hWnd, rct,  NULL,
					WS_CHILDWINDOW | WS_VISIBLE | WS_DISABLED | SS_BITMAP | SS_LEFT | SS_SUNKEN, 
					WS_EX_NOPARENTNOTIFY, WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
				cs.SetBitmap(hbmp);
				m_Manager[i].hw = cs.m_hWnd;
				m_Manager[i].iPane = iPane;
				m_Manager[i].hbmp = hbmp;
				m_Manager[i].bManage = bManage;
				UpdatePanesLayout();
				return TRUE;
			}
		}
		return FALSE;
	}
	// Same as the former, but loads the bitmap from a resource.
	BOOL SetBitmap(int iPane, int iBmpResource)
	{
		HBITMAP hb = ::LoadBitmap(_Module.m_hInstResource, MAKEINTRESOURCE(iBmpResource));
		if (NULL == hb)
			return FALSE;
		// The caller has no access to the bitmap's handle, we MUST manage it from here.
		return SetBitmap(iPane, hb, true);
	}
	// If there is a bitmap in the selected pane, destroy it.
	BOOL DestroyBitmap(int iPane)
	{
		T *pT = static_cast<T *>(this);
		// Check we're really talking about a pane.
		if (iPane < 0 || iPane >= pT->m_nPanes)
			return FALSE;
		// Find the pane in the array.
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].iPane == iPane)
			{
				// If the pane is in use, release whatever is there.
				if (m_Manager[i].bManage)
					::DeleteObject(m_Manager[i].hbmp);
				::DestroyWindow(m_Manager[i].hw);
				// Initialize array entry to suitable values.        
				m_Manager[i].Clear();
				break;
			}
		}
		return TRUE; // The pane is free.
	}
	CBitmapInPaneImpl<T>()
	{
	}
	
	virtual ~CBitmapInPaneImpl<T>()
	{
		// Destroy all windows, release all managed bitmaps.
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
				::DestroyWindow(m_Manager[i].hw);
			if (m_Manager[i].bManage)
				::DeleteObject(m_Manager[i].hbmp);
		}
	}
	
   protected:
		// This class ties one bitmap/HWND/pane.	   
	   class CBmpManager
	   {
	   public:
		   HWND hw;
		   HBITMAP hbmp;
		   int iPane;
		   bool bManage; 
		   CBmpManager(): hw(NULL), hbmp(NULL), bManage(false), iPane(-1)
		   {}
		   void Clear(void)
		   {
				hbmp = NULL;
				hw = NULL;
				bManage = false;
				iPane = -1;
		   }
	   };  // class CBmpManager
	   CBmpManager m_Manager[MAX_MANAGED]; // 15 are more than enough, I hope.
	   
}; // class CBitmapInPaneImpl


template <class T>
class CAnimationInPaneImpl
{
public:
	CAnimationInPaneImpl<T>(): m_iAnimPane(-1) {}
	virtual ~CAnimationInPaneImpl<T>()
	{
		AnimDestroyWindow();
	}
	void AnimDestroyWindow(void)
	{
		if (::IsWindow(m_Anim.m_hWnd))
		{
			m_Anim.Close();
			m_Anim.ShowWindow(SW_HIDE);
			m_Anim.DestroyWindow();
		}
		m_Anim.m_hWnd = NULL;
		m_iAnimPane = -1;
	}
	BEGIN_MSG_MAP(CMPSBarWithBitmaps<TParent>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
		
	// The animation should be displayed only when the status bar is in multiple pane mode.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (::IsWindow(m_Anim.m_hWnd))
			m_Anim.ShowWindow(wParam ? SW_HIDE: SW_SHOW);

		bHandled = FALSE;
		return 0;
	}

	// Move the animation around when the pane on top of which it lies also moves.
    BOOL UpdatePanesLayout(void)
	{
		if (m_iAnimPane != -1)
		{
			T* pt = static_cast<T*>(this);
			RECT rc;
			pt->GetRect(m_iAnimPane, &rc);
			::InflateRect(&rc, -1, -1); 
			m_Anim.MoveWindow(&rc);
		}
		return TRUE; // Mixed function.
	}
	// Create an animation.
	BOOL AnimCreate(int iPane,         // Status pane where we'll create the progress bar.
#if _WTL_VER >= 0x0710	// It's nice when WTL stuff makes it into ATL, we can keep hoping...  ;-)
		ATL::_U_STRINGorID FileName,   // Path to an AVI file, or resouce holding it.
#else
		WTL::_U_STRINGorID FileName,   // Path to an AVI file, or resouce holding it.
#endif
		DWORD dwStyle =	ACS_TRANSPARENT | ACS_CENTER | ACS_AUTOPLAY, // OR to this if adding styles
		DWORD dwExStyle = 0
		) 
	{
		// Check there is such a pane
		T* pt = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pt->m_hWnd));
		if (0 > iPane || iPane >= pt->m_nPanes)
			return FALSE;
		// Check there is not an animation already open.
		if (::IsWindow(m_Anim.m_hWnd))
			return FALSE;
		// Get the pane's rectangle
		RECT rc;
		pt->GetRect( iPane, &rc );
		::InflateRect(&rc, -1, -1); 
	
		// Create the window, using the status bar (this) as a parent.
		m_Anim.Create ( pt->m_hWnd, rc, NULL, WS_VISIBLE | WS_CHILD | dwStyle,  dwExStyle); 
		if (!m_Anim.Open(FileName))
			return FALSE;
		if (!m_Anim.Play(0, -1, -1))
			return FALSE;
		// Hold this, we'll need it to move around.
		m_iAnimPane = iPane;
		return TRUE;
	}
	
	//  Simple containment
	BOOL AnimSeek(UINT nTo)                        { return m_Anim.Seek(nTo); }
	BOOL AnimStop()                                { m_Anim.Stop(); }
	BOOL AnimPlay(UINT nFrom, UINT nTo, UINT nRep) { return m_Anim.Play(nFrom, nTo, nRep); }
	
protected:
	CAnimateCtrl m_Anim;	  // The contained control
    int m_iAnimPane;          // Pane ordinal where the progress bar resides, or -1 when off.
};


template <class T> class CAnyhingInPaneImpl
{
public:
	// You can have up to 15 free-style controls per status bar.
	enum { MAX_ANY = 15 };

	BEGIN_MSG_MAP(CAnyhingInPaneImpl<T>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
	
	// Show bitmaps when the status bar is in multi pane state, hide them when it's simple.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int sw = (wParam) ? SW_HIDE: SW_SHOW;
		// Hide all children (static bitmaps)
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].hw)
				::ShowWindow(m_AnyManager[i].hw, sw);
		}
		bHandled = false;
		return 0;
	}
	
	// Move free-style controls around to stay on top of their panes.
	BOOL UpdatePanesLayout(void)
	{
		RECT rc;	
		T *pT = static_cast<T*>(this);
		for (int i = 0;   i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].hw)
			{
				pT->GetRect( m_AnyManager[i].iPane, &rc );
				::InflateRect(&rc, -1, -1); 
				CWindow cs(m_AnyManager[i].hw);
				cs.MoveWindow(&rc);
				cs.Invalidate();
			}
		} // for
		return TRUE;
	}
	// Add a window to the status bar, in a pane chosen by ordinal.
	BOOL AddAny(int iPane, // Zero based ordinal (not resource ID) of the chosen pane.
		        HWND hw, // Handle to a window, whatever control you like (or a dialog of yours)...
		        bool bManage = false) // If true, DestroyWindow() will be called on the HWND.
	{
		if (!::IsWindow(hw))
			return false;
		// If the pane was in use, release it.
		if (!FreeAny(iPane))
			return FALSE;
		// Use the first available array entry to put the selected bitmap.
		T *pT = static_cast<T *>(this);
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (!m_AnyManager[i].hw)
			{
				RECT rct;
				pT->GetRect( iPane, &rct);
				::InflateRect(&rct, -1, -1);
				CWindow wnd;
				wnd.Attach(hw);
				wnd.MoveWindow(&rct);
				m_AnyManager[i].hw = hw;
				m_AnyManager[i].iPane = iPane;
				m_AnyManager[i].bManage = bManage;
				UpdatePanesLayout();
				return TRUE;
			}
		}
		return FALSE;
	}
	// If there is a window in the selected pane, hide it and unlink it from the pane.
	BOOL FreeAny(int iPane)
	{
		T *pT = static_cast<T *>(this);
		// Check we're really talking about a pane.
		if (iPane < 0 || iPane >= pT->m_nPanes)
			return FALSE;
		// Find the pane in the array.
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].iPane == iPane)
			{
				// If the pane is in use, release whatever is there.
				if (m_AnyManager[i].bManage && ::IsWindow(m_AnyManager[i].hw))
					::DestroyWindow(m_AnyManager[i].hw);
				::ShowWindow(m_AnyManager[i].hw, SW_HIDE);
				// Initialize array entry to suitable values.        
				m_AnyManager[i].Clear();
				break;
			}
		}
		return TRUE; // The pane exists, and is free.
	}
	CAnyhingInPaneImpl<T>()
	{
	}
	
	virtual ~CAnyhingInPaneImpl<T>()
	{
		// Destroy all windows, release all managed bitmaps.
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].bManage && ::IsWindow(m_AnyManager[i].hw))
				::DestroyWindow(m_AnyManager[i].hw);
		}
	}
	
   protected:
		// This class ties one bitmap/HWND/pane.	   
	   class CAnyManager
	   {
	   public:
		   HWND hw;
		   short iPane;
		   bool bManage; 
		   CAnyManager(): hw(NULL), bManage(false), iPane(-1)
		   {}
		   void Clear(void)
		   {
				hw = NULL;
				bManage = false;
				iPane = -1;
		   }
	   };  // class CBmpManager
	   CAnyManager m_AnyManager[MAX_ANY]; // 15 are more than enough, I hope.
	   
}; // class CAnyhingInPaneImpl


/////////////////////////////////////////////////////////////////////////////////////////
// Concrete classes, you can use them as member variables.
/////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
class CMPSBarXPCtrl
{
public:
	BEGIN_MSG_MAP(CMPSBarXPCtrl)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()
public:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		T *pT = static_cast<T*>(this);

		LRESULT lRes = pT->DefWindowProc();

		if( pT->IsSimple() ) return lRes;

		CClientDC dc(pT->m_hWnd);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		HPEN hOldPen = dc.SelectPen(pen);
		HBRUSH hOldBrush = dc.SelectStockBrush(NULL_BRUSH);
		int nCount = (int) pT->GetParts(0, NULL);
		for( int i = 0; i < nCount; i++ ) {
			RECT rcItem;
			pT->GetRect(i, &rcItem);
			if (i == nCount - 1)
				rcItem.right += 20;
			dc.Rectangle(&rcItem); 
		}
		dc.SelectBrush(hOldBrush);
		dc.SelectPen(hOldPen);

		return lRes;
	}
};

// This class adds progress bar functionality to a multi pane status bar
class CMPSBarWithProgress: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>,
	  public CProgressBarInPaneImpl<CMPSBarWithProgress>,
	  public CMPSBarXPCtrl<CMPSBarWithProgress>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithProgress"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithProgress)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithProgress>)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithProgress>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithProgress>::UpdatePanesLayout();
		return ret;
	}
};	// class CMPSBarWithProgress


// This class adds bitmap functionality (up to 15) to a multi pane status bar
class CMPSBarWithBitmaps:
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>,
	  public CBitmapInPaneImpl<CMPSBarWithBitmaps>,
	  public CMPSBarXPCtrl<CMPSBarWithBitmaps>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithBitmaps"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithBitmaps)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithBitmaps>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithBitmaps>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithBitmaps>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithBitmaps

// This class adds progress bar and bitmap functionality to a multi pane status bar
class CMPSBarWithProgressAndBMP: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>,
	  public CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>,
	  public CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>,
	  public CMPSBarXPCtrl<CMPSBarWithProgressAndBMP>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithProgressAndBMP"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithProgressAndBMP)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithProgressAndBMP>)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithProgressAndBMP

// This class adds animation functionality to a multi pane status bar
class CMPSBarWithAnimation: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>,
	  public CAnimationInPaneImpl<CMPSBarWithAnimation>,
	  public CMPSBarXPCtrl<CMPSBarWithAnimation>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithAnimation"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithAnimation)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithAnimation>)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithAnimation>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>::UpdatePanesLayout();
		CAnimationInPaneImpl<CMPSBarWithAnimation>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAnimation

// This class adds progress bar, bitmap and animation functionality to a multi pane status bar
class CMPSBarWithPrg_BMP_Anim: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CMPSBarXPCtrl<CMPSBarWithPrg_BMP_Anim>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithPrg_BMP_Anim"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithPrg_BMP_Anim)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
 		CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAll

// This class adds progress bar, bitmap and animation functionality to a multi pane status bar
class CMPSBarWithAll: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>,
	  public CProgressBarInPaneImpl<CMPSBarWithAll>,
	  public CBitmapInPaneImpl<CMPSBarWithAll>,
	  public CAnimationInPaneImpl<CMPSBarWithAll>,
	  public CAnyhingInPaneImpl<CMPSBarWithAll>,
	  public CMPSBarXPCtrl<CMPSBarWithAll>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithAll"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithAll)
		CHAIN_MSG_MAP(CMPSBarXPCtrl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CAnyhingInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>::UpdatePanesLayout();
		CAnyhingInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
 		CAnimationInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAll

#endif // !defined(AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_)
