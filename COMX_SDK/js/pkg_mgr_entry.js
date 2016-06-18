function OnCreate()
{
	comx.ui.PopupSystemModalessForm(unit.FORMID_IPkgGeneratePane);
	pane.Close();
}

function OnInstall()
{
	comx.ui.PopupSystemModalessForm(unit.FORMID_IPkgInstallToolkit);
	pane.Close();
}