#include <Windows.h>
#include <stdio.h>
//Программу можно сдать.
int myatoi(const char *str);
char *get_hex(int num);
char *get_dec(int num);

int main(int argc, char **argv) {
//	char str[] = "127684";
	char *num = get_dec(256);	
	printf("%s\n", num);
	system("pause");
	free(num);
	return 0;
}

int myatoi(const char *str) 
{
	int len, answer = 0, left = 0, i = 0, mul = 1;
	if (str[0] == 45)
		len = 1;
	else
		len = 0;
	for (; ; len++) 
	{
		if (str[len] == '\0') {
			break;
		}	
		if(str[i] - '0' > 9)
			return -2;
	}
	if (len > 10) {
		return -2;
	}
	for (i = (len - 1); i >= 0; i--) 
	{
		answer += (str[i] - '0') * mul;
		mul = mul * 10;
	}
	return answer;
}
char *get_hex(unsigned int num) 
{
	int i = 0, j = 0, comp_num = 15, div = 16;
	char *answer;
	for (i = 1;;i++) 
	{
		if(num <= comp_num)
			break;
		else
			comp_num = comp_num * div + 15;
	}
	answer = (char*) malloc((i + 2 + 1) * sizeof(char));
	answer[0] = '0';
	answer[1] = 'x';
	for(j = (i + 2 - 1); j >=2 ;j--) 
	{
		if (num % div > 9)
			answer[j] = '0' + 8 + num % div;
		else 
			answer[j] = '0' + num % div;
		num = num / div;
	}
	answer[i + 2] = '\0';
	return answer;	
}
char *get_dec(unsigned int num) 
{
	int i = 0, j = 0, comp_num = 9, div = 10;
	char *answer;
	for (i = 1;;i++) 
	{
		if(num <= comp_num)
			break;
		else
			comp_num = comp_num * 10 + 9;
	}
	answer = (char*) malloc((i + 1) * sizeof(char));
	for(j = (i - 1); j >=0 ;j--) 
	{
		answer[j] = '0' + num % div;
		num = num / div;
	}
	answer[i] = '\0';
	return answer;	
}