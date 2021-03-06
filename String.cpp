#include <iostream>
#include "String.h"
#include <cstring>

String::String()
{
	this->length = 0;
	this->string = new char('\0');
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
	if (this->string != NULL){
		strcpy(temp, this->string);
		return temp;
	}
	else{
		delete [] temp;
		return NULL;
	}
}

void String::addStr(const char *str)
{
	if (this->string == NULL){
		this->setStr(str);
		return;
	}
	char *temp = this->string;
	unsigned int len = strlen(str);
	this->string = NULL;
	this->string = new char[this->length + len + 1];
	for(unsigned int i = 0; i < this->length; i++)
	{
		this->string[i] = temp[i];
	}
	delete [] temp;
	temp = NULL;
	for(unsigned int i = 0; i < len; i++) 
	{
		this->string[this->length + i] = str[i];
	}
	this->length += len;
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
	this->string = new char[this->length + 1];
	for(unsigned int i = 0; i < this->length; i++) 
	{
		this->string[i] = str[i];
	}
	this->string[this->length] = '\0';
}
String::~String()
{
	delete [] this->string;
}

String String::operator+(String strObj)
{
	char *tempchr = strObj.getStr();//�������� ������ �� ����������� �������
	String retStrObj = this->copyAndReturnStrings(tempchr); // ���������� �� � ������� �����������
	delete [] tempchr; // ������� ��������� ������
	return retStrObj; // ���������� ����� ������
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
	if (!strcmp(this->string, tempchr))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator==(const char *str)
{
	bool status = false;
	if(!strcmp(this->string, str))
		status = true;
	return status;
}

bool String::operator==(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(!strcmp(this->string, tempchr))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator!=(String strObj)
{
	bool status = true;
	char *tempchr = strObj.getStr();
	if (!strcmp(this->string, tempchr))
		status = false;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator!=(const char *str)
{
	bool status = true;
	if(!strcmp(this->string, str))
		status = false;
	return status;
}

bool String::operator!=(char c)
{
	bool status = true;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(!strcmp(this->string, tempchr))
		status = false;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if (strcmp(this->string, tempchr) < 0)
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<(const char *str)
{
	bool status = false;
	if(strcmp(this->string, str) < 0)
		status = true;
	return status;
}

bool String::operator<(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(strcmp(this->string, tempchr) < 0)
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if (strcmp(this->string, tempchr) > 0)
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

bool String::operator>(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if(strcmp(this->string, tempchr) > 0)
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<=(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if ((strcmp(this->string, tempchr) < 0) || (!strcmp(this->string, tempchr)))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator<=(const char *str)
{
	bool status = false;
	if((strcmp(this->string, str) < 0) || (!strcmp(this->string, str)))
		status = true;
	return status;
}

bool String::operator<=(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if((strcmp(this->string, tempchr) < 0) || (!strcmp(this->string, tempchr)))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>=(String strObj)
{
	bool status = false;
	char *tempchr = strObj.getStr();
	if ((strcmp(this->string, tempchr) > 0) || (!strcmp(this->string, tempchr)))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

bool String::operator>=(const char *str)
{
	bool status = false;
	if((strcmp(this->string, str) > 0) || (!strcmp(this->string, str)))
		status = true;
	return status;
}

bool String::operator>=(char c)
{
	bool status = false;
	char *tempchr = new char[2];
	tempchr[0] = c;
	tempchr[1] = '\0';
	if((strcmp(this->string, tempchr) > 0) || (!strcmp(this->string, tempchr)))
		status = true;
	delete [] tempchr;
	tempchr = NULL;
	return status;
}

std::ostream& operator<<(std::ostream &stream, const String &obj)
{
	char *tempstr = obj.getStr();
	stream << tempstr;
	delete [] tempstr;
	return stream;
}

std::istream& operator>>(std::istream &stream, String &obj)
{
	int count = 1, len1, len2, buflen;
	char buf[20], *tempstr1 = new char('\0'), *tempstr2 = new char('\0');
	while(stream.getline(buf, 20))
	{
		len2 = strlen(tempstr1);
		tempstr2 = tempstr1;
		buflen = strlen(buf);
		len1 = buflen + len2;
		tempstr1 = new char[len1 + 1];
		for(int i = 0; i < len2; i++) 
		{
			tempstr1[i] = tempstr2[i];
		}
		for(int i = 0; i < buflen; i++)
		{
			tempstr1[len2 + i] = buf[i];
		}
		tempstr1[len1] = '\0';
	}
	obj.setStr(tempstr1);
	free(tempstr2);
	return stream;
}

