#ifndef _KMAS_COMX_CLASS_MAIN_FRAME_H
#define _KMAS_COMX_CLASS_MAIN_FRAME_H

template<typename TProgressBar>
class TComxMainFrame :public IUiProgressBarDriver,
                      public IUiToolbarStatusDriver,
                      public TFactory
{
protected:
     IUiProgressBar *p_ui_progressbar;
     IUiToolbarStatus *p_ui_common;
private:
     DWORD dwCookieSerializeProgress;
     DWORD dwCookieUiCommonDriver;
private:
     int _step;
     TProgressBar *p_progress_bar;
public:
     inline TComxMainFrame();
     inline ~TComxMainFrame();
public:
     BEGIN_IMPLEMENT_NO_COUNT_ROOT()
          IMPLEMENT_INTERFACE(IUiProgressBarDriver)
          IMPLEMENT_INTERFACE_ROOT(IUiProgressBarDriver)
          IMPLEMENT_INTERFACE(IUiToolbarStatusDriver)
     END_IMPLEMENT_ROOT()
public:
     inline virtual TStatus STDCALL UpdateUI(const int &step, const char *msg);
     inline virtual TStatus STDCALL Startup();
     inline virtual TStatus STDCALL Closure();
public:
     virtual TStatus STDCALL UpdateUIDriver() = 0;
};

template<typename TProgressBar>
TComxMainFrame<TProgressBar>::TComxMainFrame()
{
     TFactory::QueryService(CLSID_IUiProgressBar , IID_IUiProgressBar , (void**)&p_ui_progressbar);
     connect(p_ui_progressbar,(IUiProgressBarDriver*)this,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

     TFactory::QueryService(CLSID_IUiToolbarStatus , IID_IUiToolbarStatus , (void**)&p_ui_common);
     connect(p_ui_common , (IUiToolbarStatusDriver*)this , IID_IUiToolbarStatusDriver , dwCookieUiCommonDriver);

     p_progress_bar =NULL;
}

template<typename TProgressBar>
TComxMainFrame<TProgressBar>::~TComxMainFrame()
{
     disconnect(p_ui_common , IID_IUiToolbarStatusDriver , dwCookieUiCommonDriver);
     disconnect(p_ui_progressbar,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

     p_ui_common -> Release();
     p_ui_progressbar->Release();
}

template<typename TProgressBar>
TStatus STDCALL TComxMainFrame<TProgressBar>::UpdateUI(const int &step, const char *msg)
{
     if (p_progress_bar)
     {
          char szMsg[1024] = "";
          sprintf(szMsg , "%s %d%%",msg,step);

          p_progress_bar->SetText(szMsg);
          p_progress_bar->SetPos(step);
     }
     return M_OK;
}

template<typename TProgressBar>
TStatus STDCALL TComxMainFrame<TProgressBar>::Startup()
{
     if (!p_progress_bar)
     {
          p_progress_bar = new TProgressBar(_T("Progress"), 100, 100);
          assert(p_progress_bar);
     }
     return M_OK;
}

template<typename TProgressBar>
TStatus STDCALL TComxMainFrame<TProgressBar>::Closure()
{
     if (p_progress_bar)
     {
          p_progress_bar->SetPos(100);
          delete p_progress_bar;
          p_progress_bar = NULL;
     }
     return M_OK;
}

#ifdef _WTL

template<typename T>
class TComxMainFrameWTL :public IUiProgressBarDriver,
	public IUiToolbarStatusDriver,
	public TFactory
{
protected:
	IUiProgressBar *p_ui_progressbar;
	IUiToolbarStatus *p_ui_common;
private:
	DWORD dwCookieSerializeProgress;
	DWORD dwCookieUiCommonDriver;
public:
	inline TComxMainFrameWTL();
	inline ~TComxMainFrameWTL();
public:
	BEGIN_IMPLEMENT_NO_COUNT_ROOT()
		IMPLEMENT_INTERFACE(IUiProgressBarDriver)
		IMPLEMENT_INTERFACE_ROOT(IUiProgressBarDriver)
		IMPLEMENT_INTERFACE(IUiToolbarStatusDriver)
	END_IMPLEMENT_ROOT()
public:
	inline virtual TStatus STDCALL UpdateUI(const int &step, const char *msg);
	inline virtual TStatus STDCALL Startup(bool is_ind = false);
	inline virtual TStatus STDCALL Closure();
public:
	virtual TStatus STDCALL UpdateUIDriver() = 0;
private:
	WTL::CString m_strOriginCueLine;
    bool _is_ind;
};

template<typename T>
TComxMainFrameWTL<T>::TComxMainFrameWTL()
{
	TFactory::QueryService(CLSID_IUiProgressBar , IID_IUiProgressBar , (void**)&p_ui_progressbar);
	connect(p_ui_progressbar,(IUiProgressBarDriver*)this,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

	TFactory::QueryService(CLSID_IUiToolbarStatus , IID_IUiToolbarStatus , (void**)&p_ui_common);
	connect(p_ui_common , (IUiToolbarStatusDriver*)this , IID_IUiToolbarStatusDriver , dwCookieUiCommonDriver);

    _is_ind = false;
}

template<typename T>
TComxMainFrameWTL<T>::~TComxMainFrameWTL()
{
	disconnect(p_ui_common , IID_IUiToolbarStatusDriver , dwCookieUiCommonDriver);
	disconnect(p_ui_progressbar,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

	p_ui_common -> Release();
	p_ui_progressbar->Release();
}

template<typename T>
TStatus STDCALL TComxMainFrameWTL<T>::UpdateUI(const int &step, const char *msg)
{
	T *pT = static_cast<T*>(this);
	pT->m_StatusBar.ProgSetPos(step);

	if (msg == NULL)
	{
		return M_OK;
	}
	if (strlen(msg) != 0)
	{
		WTL::CString str = WTL::CString("Cue Line: ") + msg;
		pT->m_StatusBar.SetPaneText(0, str);
	}
	
	return M_OK;
}

template<typename T>
TStatus STDCALL TComxMainFrameWTL<T>::Startup(bool is_ind)
{
	T *pT = static_cast<T*>(this);

    pT->m_StatusBar.EnableIndeterminate(is_ind);
	if (!pT->m_StatusBar.ProgCreate(1, 0, 100, WS_CHILD | WS_VISIBLE))
	{
		return M_FAIL;
	}

	char szOriginCueLine[1024] = "";
	pT->m_StatusBar.GetPaneText(0, szOriginCueLine);
	m_strOriginCueLine = szOriginCueLine;

    _is_ind = is_ind;

	return M_OK;
}

template<typename T>
TStatus STDCALL TComxMainFrameWTL<T>::Closure()
{
	T *pT = static_cast<T*>(this);
	pT->m_StatusBar.ProgDestroyWindow();
	pT->m_StatusBar.SetPaneText(0, m_strOriginCueLine);

	return M_OK;
}

#endif

#endif
