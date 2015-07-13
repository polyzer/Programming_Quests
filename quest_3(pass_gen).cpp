#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

FILE *output_f_p;

void pass_gen(FILE *output_f_p, int pass_len, const char *str);
int str_length(const char *str);

int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	if ((output_f_p = fopen("OUTPUT.TXT", "w")) != NULL) 
	{	
		int k = 0;
		int number = 1, alphabet = 2;
		if  (argc < 3){
			printf("Задайте дли ну числа -n number и алфавит -a alphabet!");
			exit(-1);
		}
		for (k = 0; k < argc; k++) {
			if (k % 2 != 0) {
				if (argv[k][0] == '-' && argv[k][1] == 'n'){
					if (!atoi(argv[k+1])) {
						printf("Задайте длину пароля как число!");
					} else
						number = (k + 1);
					continue;
				}
				if (argv[k][0] == '-' && argv[k][1] == 'a') {
					alphabet = (k + 1);
					continue;
				}
			}
		}
		pass_gen(output_f_p, atoi(argv[number]), argv[alphabet]);
		printf("Пароль сгенерирован");
		fclose(output_f_p);
	} else
		printf("Невозможно открыть файл!");
	return 0;
}

void pass_gen (FILE *output_f_p, int pass_len, const char *str) {
	int i, non_null_chars;
	non_null_chars = strlen(str);
	for(i = 0; i < pass_len; i++) {
		fprintf(output_f_p, "%c", str[(int) (rand() % non_null_chars + 0)]);
	}
}