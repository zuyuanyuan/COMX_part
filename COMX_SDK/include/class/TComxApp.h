#ifndef _T_COMX_APP_H
#define _T_COMX_APP_H

class TComxApp : public TFactory
{
public:
	TComxApp():TFactory(false)
	{
#ifdef _DEBUG
		LoadKpi("glContent_d.kpi");
		LoadKpi("glSensor_d.kpi");
		LoadKpi("glMaterial_d.kpi");
		LoadKpi("glLight_d.kpi");
		LoadKpi("glBackground_d.kpi");
		LoadKpi("glPickup_d.kpi");
		LoadKpi("uiProgressBar_d.kpi");
		LoadKpi("uiCommon_d.kpi");
		LoadKpi("glWCS_d.kpi");
#else
		LoadKpi("glContent.kpi");
		LoadKpi("glSensor.kpi");
		LoadKpi("glMaterial.kpi");
		LoadKpi("glLight.kpi");
		LoadKpi("glBackground.kpi");
		LoadKpi("glPickup.kpi");
		LoadKpi("uiProgressBar.kpi");
		LoadKpi("uiCommon.kpi");
		LoadKpi("glWCS.kpi");
#endif

		LoadKpiFromScript("startup.xml");
	}
	~TComxApp(){/*Do nothing here*/}
protected:
	void LoadKpi(const char *fnam)
	{
		TStatus status;
		status = TFactory::Register(fnam);

		if (status == M_FAIL)
		{
			::MessageBox(NULL,fnam,"Error",MB_OK+MB_ICONSTOP);
			exit(0);
		}
	}
private:
	void LoadKpiFromScript(const char *filename){}
};

#endif