#ifndef _T_COMX_VIEW_H
#define _T_COMX_VIEW_H

using namespace KMAS::Die_maker::comx;

template<typename TView>
class TComxView : public TFactory
{
public:
	inline TComxView();
	//inline TComxView(IGlContentConnector *p_cc, IGlSensorToolkit *p_st, IGlPickupToolkit *p_pt);
	inline ~TComxView();
public:
	void ActivateSensor(TSensorType type)
	{
		p_sensor_tool->Activate(type);
	}
protected:
	IGlContentConnector *p_content_connector;
	IGlContentDriver *p_content_driver;
	IGlSensorToolkit *p_sensor_tool;
private:
	DWORD _dwCookiePickupContent;
public:
	void Attach()
	{
		TView  *pView = static_cast<TView*>(this);
		p_content_connector->Attach(::GetDC(pView->m_hWnd));
	}
	void Resize(int cx, int cy)
	{
		p_content_driver->ResizeEvent(cx,cy);
	}
	void Paint()
	{
		p_content_driver->PaintEvent();
	}
	void Mouse(UINT nFlags, int x, int y, TMouseEvent::TAction action)
	{
		if( x > 60000 || y > 60000)
			return;

		//GLint viewport[4];
		//glGetIntegerv(GL_VIEWPORT,viewport);

		TMouseEvent mouse_event(nFlags,x,y,action);
		p_content_driver->MouseEvent(mouse_event);
	}
};

template<typename TView>
TComxView<TView>::TComxView()
{
	TFactory::QueryService(CLSID_IGlContent,IID_IGlContentConnector,(void**)&p_content_connector);
	p_content_connector->QueryInterface(IID_IGlContentDriver,(void**)&p_content_driver);
	TFactory::QueryService(CLSID_IGlSensor,IID_IGlSensorToolkit,(void**)&p_sensor_tool);

	IGlPickupToolkit *p_pickup_toolkit = NULL;
	TFactory::QueryService(CLSID_IGlPickup , IID_IGlPickupToolkit , (void**)&p_pickup_toolkit);
	assert(p_pickup_toolkit);

	IGlContentRender *p_pickup_content_render = NULL;
	p_pickup_toolkit->QueryInterface(IID_IGlContentRender , (void**)&p_pickup_content_render);
	assert(p_pickup_content_render);

	KMAS::Die_maker::comx::connect(p_content_connector , p_pickup_content_render , IID_IGlContentRender , _dwCookiePickupContent);

	p_pickup_content_render->Release();
	p_pickup_toolkit->Release();
}

template<typename TView>
TComxView<TView>::~TComxView()
{
	::disconnect(p_content_connector , IID_IGlContentRender , _dwCookiePickupContent);

	p_sensor_tool->Release();
	p_content_driver->Release();
	p_content_connector->Release();
}

// template<typename TView>
// TComxView<TView>::TComxView(IGlContentConnector *p_cc, IGlSensorToolkit *p_st, IGlPickupToolkit *p_pickup_toolkit)
// {
// 	p_content_connector = p_cc;
// 	p_content_connector->AddRef();
// 
// 	p_content_connector->QueryInterface(IID_IGlContentDriver,(void**)&p_content_driver);
// 
// 	p_sensor_tool = p_st;
// 	p_sensor_tool->AddRef();
// 
// 	IGlContentRender *p_pickup_content_render = NULL;
// 	p_pickup_toolkit->QueryInterface(IID_IGlContentRender , (void**)&p_pickup_content_render);
// 	assert(p_pickup_content_render);
// 	
// 	KMAS::Die_maker::comx::connect(p_content_connector , p_pickup_content_render , IID_IGlContentRender , _dwCookiePickupContent);
// 	
// 	p_pickup_content_render->Release();
// }

#ifdef _WTL

template<typename T>
class TComxView4WTL : public TComxView<T>
{
public:
	BEGIN_MSG_MAP(TComxView4WTL)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMButtonDown)
		MESSAGE_HANDLER(WM_MBUTTONUP, OnMButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	END_MSG_MAP()
private:
    int _current_x;
    int _current_y;
public:
    TComxView4WTL() : _current_x(0), _current_y(0){}
public:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		//Paint();
		bHandled = false;
		return 0;
	}
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Attach();
		bHandled = false;
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);
		
		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);
        
		pT->SetCapture();
		Mouse(nFlags,x,y,TMouseEvent::LPRESS);
		bHandled = false;
		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int  x = LOWORD(lParam), y = HIWORD(lParam);

		::ReleaseCapture();
		Mouse(nFlags,x,y,TMouseEvent::LRELEASE);
		bHandled = false;
		return 0;
	}

	LRESULT OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);

		pT->SetCapture();
		Mouse(nFlags,x,y,TMouseEvent::MPRESS);
		bHandled = false;
		return 0;
	}

	LRESULT OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);

		::ReleaseCapture();
		Mouse(nFlags,x,y,TMouseEvent::MRELEASE);
		bHandled = false;
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);

        _current_x = x;
        _current_y = y;

		Mouse(nFlags,x,y,TMouseEvent::MOTION);
		bHandled = false;
		return 0;
	}
    
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);

		pT->SetCapture();
		Mouse(nFlags,x,y,TMouseEvent::RPRESS);
		bHandled = false;
		return 0;
	}

	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		UINT nFlags = (UINT)wParam;
		int x = LOWORD(lParam), y = HIWORD(lParam);

		::ReleaseCapture();
		Mouse(nFlags,x,y,TMouseEvent::RRELEASE);
		bHandled = false;
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pT = static_cast<T*>(this);

		int cx = LOWORD(lParam), cy = HIWORD(lParam);
		Resize(cx,cy);

		IGlLegend *p_gl_legend = NULL;
		TFactory::QueryService(CLSID_IGlLegend, IID_IGlLegend, (void**)&p_gl_legend);
		
		if (p_gl_legend == NULL)
		{
			return 0;
		}

		bool flag = false;
		p_gl_legend->GetVisible(flag);

		if (flag)
		{
			p_gl_legend->SetAttachment(ATTACHMENT_TOP);
			p_gl_legend->SetAttachment(ATTACHMENT_RIGHT);
			p_gl_legend->SetAttachment(ATTACHMENT_NONE);

			p_gl_legend->Release();
		}
		
		//bHandled = false;
		return 0;
	}

    LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        T *pT = static_cast<T*>(this);

        UINT nFlags = (UINT)wParam;
        int x = _current_x/*LOWORD(lParam)*/, y = _current_y/*HIWORD(lParam)*/;

        if( (int)wParam < 0)   
        {
            Mouse(nFlags,x,y,TMouseEvent::WHEEL_INCREMENT);
        }
        else
        {
            Mouse(nFlags,x,y,TMouseEvent::WHEEL_DECREMENT);
        }
        
        bHandled = false;
        return 0;
    }
};

#endif

#endif