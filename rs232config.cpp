#include "rs232.hpp"
#include <stdlib.h>
#include <string>

RS232config::RS232config(const std::string & configfile)
{
    std::ifstream f(configfile.c_str());
    char cstring[256];
    char * pch;

    std::cout << "opening RS232 config file: " << configfile << std::endl;
    if( f.is_open() != true) {
	std::cerr << "couldn't open RS232 config file" << std::endl;
	abort();
    }

    while (!f.eof())
    {
        f.getline(cstring, sizeof(cstring));
	pch=strchr(cstring,'=');
	if (pch)
	{
	    *pch = '\0';

	    if(!strcmp(cstring,"eos")){
		int n;
		//char tmp[16];
		//strncpy(tmp,pch+1,strlen(pch+1));
		//n = sprintf(pch+1, tmp);
		n = sprintf(pch+1, "\r\n");
		if(n > 0)
		    *(pch+1+n)='\0';
	    }

	    s_params.insert(std::pair<std::string,std::string>(cstring, pch+1));	    
	}
    }
    f.close();

    //std::map<std::string, std::string>::const_iterator p = s_params.begin();
    //for (; p != s_params.end(); ++p)
    //std::cout << " result: "<< p->first << " = " << p->second << std::endl;

    std::cout << "\tDevice name: " << get_string_param("devname")  << std::endl;
    std::cout << "\tEOS: " << get_string_param("eos")  << std::endl;
    std::cout << "\tProtocol: " << get_string_param("protocol")  << std::endl;
    std::cout << "\tBaudrate: " << get_int_param("baudrate")  << std::endl;
    std::cout << "\tTimeout: " << get_int_param("timeout")  << std::endl;
    std::cout << "\tQuit: " << get_int_param("quit") << std::endl;

    f.close();
}

double RS232config::get_double_param(const std::string& s) const throw (E_missing_parameter)
{
    double d;
    if (!get_param(s,d)) 
	throw (E_missing_parameter(s.c_str(),name()));
    return d;
}

int RS232config::get_int_param(const std::string& s) const throw (E_missing_parameter)
{
    int d;
    if (!get_param(s,d)) 
	throw (E_missing_parameter(s.c_str(),name()));
    return d;
}

std::string RS232config::get_string_param(const std::string& s) const throw (E_missing_parameter)
{
    std::string d;
    if (!get_param(s,d)) 
	throw (E_missing_parameter(s.c_str(),name()));
    return d;
}


bool RS232config ::get_param(const std::string& s, double& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
	return false;
    v = 3.1415;// p->second;
    return true;
}

bool RS232config::get_param(const std::string& s, int& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
	return false;
    v = atoi((p->second).c_str());
    return true;
}

bool RS232config::get_param(const std::string& s, std::string& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
	return false;
    v = p->second;
    return true;
}