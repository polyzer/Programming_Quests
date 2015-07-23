#include <iostream>
#include "String.h"

int main(int argc, char **argv) 
{
	String mystr, str;
	//mystr = "hello";
	str = "world1111111";
	mystr = "this is string";
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