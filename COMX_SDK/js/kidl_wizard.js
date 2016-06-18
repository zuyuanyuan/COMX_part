function OnInitializeData()
{
    //binding.multi_text = "hello,world!\r\nmy name is xiangkui.\r\n";
    binding.sci_content = "int main(int argc, char** argv)\n{\n    return 0;\n}\n";
}

apply_flag = "false";

function OnApply()
{
    apply_flag = "true";
}

function OnCalculate()
{
    if(apply_flag == "false")
    {
        comx.ui.InformationBox("Please Execute Apply first");
    }
    comx.ui.InformationBox(binding.sci_content);
}