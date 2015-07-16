
#include <Windows.h>
#include <stdio.h>
//Программу можно сдать.
//45 "-", 48-57 "0...9"
int myatoi(const char *str);
char *get_hex(int num);
char *get_dec(int num);

int main(int argc, char **argv) {
	char str[] = "127684";
	char *num = get_hex(0x100);	
	printf("%s\n", num);
	system("pause");
	free(num);
	return 0;
}

int myatoi(const char *str) 
{
	int len, answer = 0, left = 0, i = 0, m = 0;
;
	if (str[0] == 45)
		len = 1;
	else
		len = 0;
	for (; ; len++) 
	{
		if (str[len] == '\0') {
			break;
		}	
		for (m = 48; m <= 57; m++) {
			if (str[len] == m) {
				break;
			}
			if ((m == 57 && str[len] != m) || (len != 0 && str[len] == 45)) 
				return -1;
			
		}
	}
	if (len > 10) {
		return -2;
	}
	for (i = (len - 1); i >= 0; i--) {
		int j;
		int number = 0;
		
		for (j = 48; j <= 57; j++) {
			if ((int) str[i] == j) {
				if (left == 0)
					answer += number;
				else{
				int mul = 1, k;
				for (k = 1; k <= left; k++)
					mul *= 10;
				answer += number * mul;
				}
				break;
			} 
			++number;
		}
		++left;
		if (i == 0 && str[i] == 45 && len != 1) {
			return -answer;
		}
	}

	return answer;
}
char *get_hex(unsigned int num) 
{
	int i = 0, j = 0, comp_num = 9, div = 16;
	char *answer;
	for (i = 1;;i++) 
	{
		if(num <= comp_num)
			break;
		else
			comp_num = comp_num * 10 + 9;
	}
	answer = (char*) malloc((i + 2 + 1) * sizeof(char));
	answer[0] = '0';
	answer[1] = 'x';
	for(j = (i + 2 - 1); j >=2 ;j--) 
	{
		switch(num % div){
			case 0:
				answer[j] = '0';
			break;
			case 1:
				answer[j] = '1';
			break;
			case 2:
				answer[j] = '2';
			break;
			case 3:
				answer[j] = '3';
			break;
			case 4:
				answer[j] = '4';
			break;
			case 5:
				answer[j] = '5';
			break;
			case 6:
				answer[j] = '6';
			break;
			case 7:
				answer[j] = '7';
			break;
			case 8:
				answer[j] = '8';
			break;
			case 9:
				answer[j] = '9';
			break;
			case 10:
				answer[j] = 'A';
			break;
			case 11:
				answer[j] = 'B';
			break;
			case 12:
				answer[j] = 'C';
			break;
			case 13:
				answer[j] = 'D';
			break;
			case 14:
				answer[j] = 'E';
			break;
			case 15:
				answer[j] = 'F';
			break;

		}
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

		switch(num % 10){
			case 0:
				answer[j] = '0';
			break;
			case 1:
				answer[j] = '1';
			break;
			case 2:
				answer[j] = '2';
			break;
			case 3:
				answer[j] = '3';
			break;
			case 4:
				answer[j] = '4';
			break;
			case 5:
				answer[j] = '5';
			break;
			case 6:
				answer[j] = '6';
			break;
			case 7:
				answer[j] = '7';
			break;
			case 8:
				answer[j] = '8';
			break;
			case 9:
				answer[j] = '9';
			break;
		}
		num = num / 10;
	}
	answer[i] = '\0';
	return answer;	
}