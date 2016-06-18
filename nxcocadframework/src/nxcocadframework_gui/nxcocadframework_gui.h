// nxcocadframework_gui.h : main header file for the NXCOCADFRAMEWORK_GUI DLL
//

#if !defined(AFX_NXCOCADFRAMEWORK_GUI_H__FE957A0E_6F96_4D4B_A490_D4775836F653__INCLUDED_)
#define AFX_NXCOCADFRAMEWORK_GUI_H__FE957A0E_6F96_4D4B_A490_D4775836F653__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNxcocadframework_guiApp
// See nxcocadframework_gui.cpp for the implementation of this class
//

class CNxcocadframework_guiApp : public CWinApp
{
public:
	CNxcocadframework_guiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNxcocadframework_guiApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNxcocadframework_guiApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NXCOCADFRAMEWORK_GUI_H__FE957A0E_6F96_4D4B_A490_D4775836F653__INCLUDED_)
