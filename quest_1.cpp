
#include <Windows.h>
#include <stdio.h>
//Программа проверяет корректность ввода + может отрицательные числа.
//45 "-", 48-57 "0...9"
int myatoi(const char *str);

int main(int argc, char **argv) {
	char str[] = "127684";
	//char hex_str[] = "0x1923";
	printf("%d", myatoi(str));
	system("pause");
	return 0;
}

int myatoi(const char *str) 
{
	int len, answer = 0, left = 0;
	if (str[0] == 45)
		len = 1;
	else
		len = 0;
	for (; ; len++) 
	{
		if (str[len] == '\0') {
			break;
		}	
		int m;
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
	int i;
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
void print_hex(unsigned int dec_num) 
{
	
}