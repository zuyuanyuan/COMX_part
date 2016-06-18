#ifndef _KMAS_COMX_WTL_EXTEND_COMMON_H
#define _KMAS_COMX_WTL_EXTEND_COMMON_H

#include <atlmisc.h>
#include <assert.h>

inline CImageList GenerateImageListFromID(UINT uBitmapID, int nBtnWidth, HINSTANCE hInst = NULL)
{
	CImageList	cImageList;
	CBitmap		cBitmap;
	BITMAP		bmBitmap;
	CSize		cSize;
	int			nNbBtn;

	if(hInst == NULL)
		hInst = _Module.GetModuleInstance();

	cBitmap.Attach((HBITMAP)LoadImage(hInst,
		MAKEINTRESOURCE(uBitmapID),
		IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE|LR_CREATEDIBSECTION));
	cBitmap.GetBitmap(&bmBitmap);

	cSize  = CSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	nNbBtn = cSize.cx/nBtnWidth;
	RGBTRIPLE* rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF   rgbMask	= RGB(rgb[0].rgbtRed,
		rgb[0].rgbtGreen,
		rgb[0].rgbtBlue);

	if (!cImageList.Create(nBtnWidth, cSize.cy,
		ILC_COLOR24|ILC_MASK,
		nNbBtn, 0))
		assert(false);

	if (cImageList.Add(cBitmap.m_hBitmap, rgbMask) == -1)
		assert(false);

	return cImageList;
}
#endif