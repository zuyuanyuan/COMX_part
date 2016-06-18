#ifndef _KMAS_COMX_DATA_TRANSFORM_H_20061107
#define _KMAS_COMX_DATA_TRANSFORM_H_20061107

#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

class DataTransform
{
public:
	void int_to_cstr(const int &int_input, char *cstr_output)
	{
		stringstream iostr;
		iostr << int_input;
		iostr >> cstr_output;
	}
	void float_to_cstr(const float &float_input, char *cstr_output)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << float_input;
		iostr >> cstr_output;
	}
	void double_to_cstr(const double &double_input, char *cstr_output)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << double_input;
		iostr >> cstr_output;
	}
	void cstr_to_int(const char *cstr_input, int &int_ouput)
	{
		stringstream iostr;
		iostr << cstr_input;
		iostr >> int_ouput;
	}
	void cstr_to_float(const char *cstr_input, float &float_ouput)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << cstr_input;
		iostr >> float_ouput;
	}
	void cstr_to_double(const char *cstr_double, double &double_ouput)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << cstr_double;
		iostr >> double_ouput;
	}
	void int_to_str(const int &int_input, string &str_output)
	{
		stringstream iostr;
		iostr << int_input;
		iostr >> str_output;
	}
	void float_to_str(const float &float_input, string &str_output)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << float_input;
		iostr >> str_output;
	}
	void double_to_str(const double &double_input, string &str_output)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << double_input;
		iostr >> str_output;
	}
	void str_to_int(const string &str_input, int &int_ouput)
	{
		stringstream iostr;
		iostr << str_input;
		iostr >> int_ouput;
	}
	void str_to_float(const string &str_input, float &float_ouput)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << str_input;
		iostr >> float_ouput;
	}
	void cstr_to_double(const string &str_double, double &double_ouput)
	{
		stringstream iostr;
		iostr << setprecision(10) << fixed << str_double;
		iostr >> double_ouput;
	}
};


#endif