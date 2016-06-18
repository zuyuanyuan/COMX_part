// DialogMessageHook.h: interface for the CDialogMessageHook class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _WTL_CDIALOG_MESSAGE_HOOK_H
#define _WTL_CDIALOG_MESSAGE_HOOK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <set>

typedef std::set<HWND> THWNDCollection;

// CDialogMessageHook makes it easy to properly process tab and accelerator keys in
// ATL modeless dialogs
class CDialogMessageHook  
{
public:
    // set a dialog message hook for the specified modeless dialog
    static HRESULT InstallHook(HWND hWnd)
	{
		HHOOK &m_hHook = GetHook();
		THWNDCollection &m_aWindows = GetHWNDCollection();
		// make sure the hook is installed
		if (m_hHook == NULL)
		{
			m_hHook = ::SetWindowsHookEx(WH_GETMESSAGE,
				GetMessageProc,
				_Module.m_hInst,
				GetCurrentThreadId());
			
			// is the hook set?
			if (m_hHook == NULL)
			{
				return E_UNEXPECTED;
			}
		}
		
		// add the window to our list of managed windows
		if (m_aWindows.find(hWnd) == m_aWindows.end())
			m_aWindows.insert(hWnd);
		
		return S_OK;
	}
    static HRESULT UninstallHook(HWND hWnd)
	{
		HRESULT hr = S_OK;
		
		HHOOK &m_hHook = GetHook();
		THWNDCollection &m_aWindows = GetHWNDCollection();
		// was the window found?
		if (m_aWindows.erase(hWnd) == 0)
			return E_INVALIDARG;
		
		// is this the last window? if so, then uninstall the hook
		if (m_aWindows.size() == 0 && m_hHook)
		{
			if (!::UnhookWindowsHookEx(m_hHook))
				hr = HRESULT_FROM_WIN32(::GetLastError());
			
			m_hHook = NULL;
		}
		
		return hr;
	}

private:
    // the hook function
    static LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		HHOOK &m_hHook = GetHook();
		THWNDCollection &m_aWindows = GetHWNDCollection();

		// If this is a keystrokes message, pass it to IsDialogMessage for tab
		// and accelerator processing
		LPMSG lpMsg = (LPMSG) lParam;
		
		if ((nCode >= 0) &&
			PM_REMOVE == wParam &&
			(lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST))
		{
			HWND hWnd, hActiveWindow = GetActiveWindow();
			THWNDCollection::iterator it = m_aWindows.begin();
			
			// check each window we manage to see if the message is meant for them
			while (it != m_aWindows.end())
			{
				hWnd = *it;
				
				if (::IsWindow(hWnd) &&
					::IsDialogMessage(hWnd, lpMsg))
				{
					// The value returned from this hookproc is ignored, and it cannot
					// be used to tell Windows the message has been handled. To avoid
					// further processing, convert the message to WM_NULL before
					// returning.
					lpMsg->hwnd = NULL;
					lpMsg->message = WM_NULL;
					lpMsg->lParam = 0L;
					lpMsg->wParam = 0;
					
					break;
				}
				
				it++;
			}
		}
		
		// Passes the hook information to the next hook procedure in
		// the current hook chain.
		return ::CallNextHookEx(m_hHook, nCode, wParam, lParam);
	}
	
    // the hook handle
    //static HHOOK m_hHook;
	static HHOOK &GetHook(){static HHOOK m_hHook = NULL; return m_hHook;}

    // the set of HWNDs we are hooking
    //static THWNDCollection m_aWindows;
	static THWNDCollection &GetHWNDCollection(){static THWNDCollection m_aWindows; return m_aWindows;}
};

//HHOOK CDialogMessageHook::m_hHook = NULL;
//THWNDCollection CDialogMessageHook::m_aWindows;

#endif // _WTL_CDIALOG_MESSAGE_HOOK_H