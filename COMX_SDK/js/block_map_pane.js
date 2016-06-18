function OnOkay()
{
	comx.gf.block_map_value = binding.value;
	comx.gf.block_map_value_okay = true;
    
	pane.Close();
}

function OnClose()
{
    pane.Close();
}

function OnInitializeData()
{
	binding.value = comx.gf.block_map_value;
}