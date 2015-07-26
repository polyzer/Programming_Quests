#include <iostream>
#include "String.h"

int main(int argc, char **argv) 
{
	String mystr("sadpofnwernj"), str, str1("sdfsdfsdfarv");
	//mystr = "hello";
	str = "world1111111";
//	mystr = "this is string";
//	str1 = str + mystr;
	std::cout << str1 + str1 + str1 + str1+ str1 << std::endl;
	if(mystr > str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;
	if(mystr < str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;
	if(mystr != str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;
	if(mystr == str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;
	if(mystr <= str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;
	if(mystr >= str)
		std::cout << "true" << std::endl;
	else 
		std::cout << "false" << std::endl;

	system("pause");

	return 0;
}