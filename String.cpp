#include "String.h"
#include <cstring>

String::String()
{
	this->length = 0;

}

String::String(char *str)
{
	length = strlen(str);
	this->string = new char[this->length+1];
	strcpy(this->string, str);
}

String::String(char c)
{
	length = 0;
	length++;
	this->string = new char[length+1];
	this->string[length - 1] = c;
	this->string[length] = '\0';
}

String::String(unsigned short int c)
{
	length = 0;
	length++;
	this->string = new char[length+1];
	this->string[length - 1] = (char) c;
	this->string[length] = '\0';	
}

String::String(const String &str)
{
	this->length = str.getLength();
	this->string = new char[length+1];
	char *tempStr = str.getStr();
	strcpy(this->string, tempStr);
	delete []tempStr;
	tempStr = NULL;
}

unsigned int String::getLength() const
{
	return this->length;
}

char *String::getStr() const
{
	char *temp = new char[this->length + 1];
	strcpy(temp, this->string);
	return temp;
}

void String::addStr(const char *str)
{
	char *temp = this->string;
	this->string = NULL;
	this->string = new char[this->length + strlen(str)];
	for(int i = 0; i < this->length; i++)
	{
		this->string[i] = temp[i];
	}
	delete [] temp;
	temp = NULL;
	for(int i = 0; i < strlen(str); i++) 
	{
		this->string[this->length + i] = str[i];
	}
	this->length += strlen(str);
	this->string[this->length] = '\0';
}

String String::copyAndReturnStrings(const char *str)
{
	String temp(this->string);
	temp.addStr(str);
	return temp;
}

void String::setStr(const char *str)
{
	delete [] this->string;
	this->length = strlen(str);
	this->string = new char[this->length];
	for(int i = 0; i < this->length; i++) 
	{
		this->string[i] = str[i];
	}
}
String::~String()
{
	delete [] this->string;
}

String String::operator+(String strObj)
{
	char *tempchr = strObj.getStr();//получаем строку из переданного объекта
	String retStrObj = this->copyAndReturnStrings(tempchr); // отправляем ее в функцию копирования
	delete [] tempchr; // удаляем временную строку
	return retStrObj; // возвращаем новый объект
}

String String::operator+(const char *str)
{
	String retStrObj = this->copyAndReturnStrings(str);
	return retStrObj;
}

String String::operator+(char c)
{
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	String retStrObj = this->copyAndReturnStrings(tempchr);
	delete [] tempchr;
	return retStrObj;
}

void String::operator=(String strObj)
{
	char *tempchr = strObj.getStr();
	this->setStr(tempchr);
	delete [] tempchr;
	tempchr = NULL;
}

void String::operator=(const char *str)
{
	this->setStr(str);
}

void String::operator=(char c)
{
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	this->setStr(tempchr);
	delete [] tempchr;
	tempchr = NULL;
}

void String::operator+=(String strObj)
{
	char *tempchr = strObj.getStr();
	this->addStr(tempchr);
	delete [] tempchr;
	tempchr = NULL;
}

void String::operator+=(const char *str)
{
	this->addStr(str);
}

void String::operator+=(char c)
{
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	this->addStr(tempchr);
	delete [] tempchr;
	tempchr = NULL;
}

bool String::operator==(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if (!strcmp(this->string, tempchr));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator==(const char *str)
{
	bool status = false;
	if(!strcmp(this->string, str));
		status = true;
	return status;
}

bool String::operator==(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(!strcmp(this->string, tempchr));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator!=(String strObj)
{
	bool status = true;
	char *tempchr = strObj.getStr();
	if (!strcmp(this->string, tempchr));
		status = false;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator!=(const char *str)
{
	bool status = true;
	if(!strcmp(this->string, str));
		status = false;
	return status;
}

bool String::operator!=(char c)
{
	bool status = true;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(!strcmp(this->string, tempchr));
		status = false;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if (strcmp(this->string, tempchr) < 0);
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<(const char *str)
{
	bool status = false;
	if(strcmp(this->string, str) < 0);
		status = true;
	return status;
}

bool String::operator<(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(strcmp(this->string, tempchr) < 0);
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if (strcmp(this->string, tempchr) > 0);
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>(const char *str)
{
	bool status = false;
	if(strcmp(this->string, str) > 0);
		status = true;
	return status;
}

bool String::operator<(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(strcmp(this->string, tempchr) > 0);
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<=(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if ((strcmp(this->string, tempchr) < 0) || (!strcmp(this->string, tempchr)));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<=(const char *str)
{
	bool status = false;
	if((strcmp(this->string, str) < 0) || (!strcmp(this->string, str)));
		status = true;
	return status;
}

bool String::operator<=(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if((strcmp(this->string, tempchr) < 0) || (!strcmp(this->string, tempchr)));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>=(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if ((strcmp(this->string, tempchr) > 0) || (!strcmp(this->string, tempchr)));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>=(const char *str)
{
	bool status = false;
	if((strcmp(this->string, str) > 0) || (!strcmp(this->string, str)));
		status = true;
	return status;
}

bool String::operator>=(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if((strcmp(this->string, tempchr) > 0) || (!strcmp(this->string, tempchr)));
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}