#ifndef _T_COMX_DOCUMENT_H
#define _T_COMX_DOCUMENT_H

template<typename TProgressBar>
class TComxDoc :public IUiProgressBarDriver,
	public TFactory
{
protected:
	IUiProgressBar *p_ui_progressbar;
private:
	DWORD dwCookieSerializeProgress;
private:
	int _step;
	TProgressBar *p_progress_bar;
public:
	inline TComxDoc();
	inline ~TComxDoc();
public:
	BEGIN_IMPLEMENT_NO_COUNT_ROOT()
		IMPLEMENT_INTERFACE(IUiProgressBarDriver)
		IMPLEMENT_INTERFACE_ROOT(IUiProgressBarDriver)
	END_IMPLEMENT_ROOT()
public:
	inline virtual TStatus STDCALL UpdateUI(const int &step, const char *msg);
	inline virtual TStatus STDCALL Startup();
	inline virtual TStatus STDCALL Closure();
};

template<typename TProgressBar>
TComxDoc<TProgressBar>::TComxDoc()
{
	TFactory::QueryService(CLSID_IUiProgressBar , IID_IUiProgressBar , (void**)&p_ui_progressbar);
	connect(p_ui_progressbar,this,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

	p_progress_bar =NULL;
}

template<typename TProgressBar>
TComxDoc<TProgressBar>::~TComxDoc()
{
	disconnect(p_ui_progressbar,IID_IUiProgressBarDriver,dwCookieSerializeProgress);

	p_ui_progressbar->Release();
}

template<typename TProgressBar>
TStatus STDCALL TComxDoc<TProgressBar>::UpdateUI(const int &step, const char *msg)
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
TStatus STDCALL TComxDoc<TProgressBar>::Startup()
{
	if (!p_progress_bar)
	{
		p_progress_bar = new TProgressBar(_T("Progress"), 100, 100);
		assert(p_progress_bar);
	}
	return M_OK;
}

template<typename TProgressBar>
TStatus STDCALL TComxDoc<TProgressBar>::Closure()
{
	if (p_progress_bar)
	{
		p_progress_bar->SetPos(100);
		delete p_progress_bar;
		p_progress_bar = NULL;
	}
	return M_OK;
}

#endif