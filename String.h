struct String
{
	String();
	String(char *str);
	String(char c);
	String(unsigned short int);
	String(const String &str);
	~String();
	String operator+(String str);
	String operator+(const char *str);
	String operator+(char c);
	void operator=(String str);
	void operator=(const char *str);
	void operator=(char c);
	void operator+=(String str);
	void operator+=(const char *str);
	void operator+=(char c);
	bool operator==(String str);
	bool operator==(const char *str);
	bool operator==(char c);
	bool operator!=(String str);
	bool operator!=(const char *str);
	bool operator!=(char c);
	bool operator<(String str);
	bool operator<(const char *str);
	bool operator<(char c);
	bool operator>(String str);
	bool operator>(const char *str);
	bool operator>(char c);
	bool operator<=(String str);
	bool operator<=(const char *str);
	bool operator<=(char c);
	bool operator>=(String str);
	bool operator>=(const char *str);
	bool operator>=(char c);
	char operator[](int index);

	String copyAndReturnStrings(const char *str);
	void addStr(const char *str);
	void setStr(const char *str);
	char *getStr() const;
	unsigned int getLength() const;
	
	friend std::ostream& operator<<(std::ostream& stream, const String &obj);
	friend std::istream& operator>>(std::istream& stream, const String &obj);


private:

	char *string;
	unsigned int length;

};
