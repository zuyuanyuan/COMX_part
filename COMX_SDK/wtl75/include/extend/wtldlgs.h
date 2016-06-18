#ifndef _KMAS_COMX_WTL_DIALOG_XP_STYLE_DEFINE_H
#define _KMAS_COMX_WTL_DIALOG_XP_STYLE_DEFINE_H

#include <atldlgs.h>
#include "atlmisc.h"

#include <string>
using namespace std;

#include <base/function.hxx>
using namespace KMAS::func;

// Class to support a filter list when using the WTL CFileDialog.
// Allows a filter string delimited with a pipe to be used (instead of a string
// delimited with '\0')
class CFileDialogFilter  
{
private:
	WTL::CString m_strFilter;
public:
	CFileDialogFilter()
	{
	}

	/// nID The ID of a resource string containing the filter
	CFileDialogFilter(UINT nID)
	{
		SetFilter(nID);
	}

	/// lpsz The filter string
	CFileDialogFilter(LPCTSTR lpsz)
	{
		SetFilter(lpsz);
	}

	~CFileDialogFilter()
	{
	}

	inline LPCTSTR GetFilter() const { return m_strFilter; }
	inline operator LPCTSTR() const { return m_strFilter; }

	// Set the filter string to use
	// nID - The ID of a resource string containing the filter
	void SetFilter(UINT nID)
	{
		if (m_strFilter.LoadString(nID) && !m_strFilter.IsEmpty())
			ModifyString();
	}

	// Set the filter string to use
	// lpsz - The filter string
	void SetFilter(LPCTSTR lpsz)
	{		
		m_strFilter = lpsz;
		if (!m_strFilter.IsEmpty())
			ModifyString();
	}
private:
	// Replace '|' with '\0'
	void ModifyString(void)
	{
		// Get a pointer to the string buffer
		LPTSTR psz = m_strFilter.GetBuffer(0);
		// Replace '|' with '\0'
		while ((psz = _tcschr(psz, '|')) != NULL)
			*psz++ = '\0';
	}
};

class CFileDialogXP : public CFileDialogImpl<CFileDialogXP>
{
public:
	CFileDialogXP(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszTitle = NULL,
		LPCTSTR lpszFilter = NULL,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		HWND hWndParent = NULL,
		UINT ext_dlg_id = 0)
		: CFileDialogImpl<CFileDialogXP>(
			bOpenFileDialog, 
			lpszDefExt, 
			lpszFileName, 
			dwFlags, 
			lpszFilter, 
			hWndParent)
	{
		m_ofn.lpstrTitle = lpszTitle;
		m_ofn.Flags |= dwFlags;
		
		if (ext_dlg_id != 0)
		{
			m_ofn.lpTemplateName = MAKEINTRESOURCE(ext_dlg_id);
			m_ofn.Flags |= OFN_ENABLETEMPLATE;
			m_ofn.Flags |= OFN_EXPLORER;
		}
	}

	// Maps
	BEGIN_MSG_MAP(CMyFileDialog)
		CHAIN_MSG_MAP(CFileDialogImpl<CFileDialogXP>)
	END_MSG_MAP()

	LPCTSTR GetFilename()
	{
		_make_filename();

		return m_Filename.c_str();
	}

	// override base class map and references to handlers
	//DECLARE_EMPTY_MSG_MAP()

	void OnInitDone(LPOFNOTIFY lpon)
	{
		GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);
	}
private:
	string m_Filename;
private:
	void _make_filename()
	{
		m_Filename = m_szFileName;

        string ext_fname = get_cur_ext_filename();
		
		if (!m_bOpenFileDialog && !is_ext_filename(m_Filename, ext_fname))
		{
			m_Filename += '.';
			m_Filename += ext_fname;
		}
	}

	string get_cur_ext_filename()
	{
		const char *filter_item = m_ofn.lpstrFilter;
		int index = m_ofn.nFilterIndex;

		int cnt = 0;
		while (cnt++ < index - 1)
		{
			filter_item += (strlen(m_ofn.lpstrFilter) + 1);
			filter_item += (strlen(filter_item) + 1);
		}
		
		string strFilterItem = filter_item;
		
		int spos = strFilterItem.find_last_of('.') + 1;
		int epos = spos;
		
		while (is_char(strFilterItem[epos]) || is_digit(strFilterItem[epos]))
			epos++;
		
		if (spos != epos)
		{
			return strFilterItem.substr(spos, epos - spos);    
		}
		else
		{
			return "";
		}
	}
};

#endif