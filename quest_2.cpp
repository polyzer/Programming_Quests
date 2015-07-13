//ПРОГРАММА РАБОТАЕТ, СДАТЬ НАДО!
#include <Windows.h>
#include <stdio.h>

char *append_char(char *mod_str, char elem);

char * str_mod(const char *str1, const char * inner,const char * replacer);
int getlen(const char *str);
char *remap (char *str, int len);
char *append(char *str, const char *replace);

int main (int argc, char **argv) {
	char *newstr = str_mod("23rfsdfsdf34", "23rf", "pf");
	printf("%s", newstr);
	system("pause");
	free(newstr);
	return 0;
}

char *str_mod(const char *str, const char *inner_str, const char * replace_str) {
	char *mod_str;
	int str_len = 0, // длина исходной строки 
		inner_len = 0, //длина вложенного слова
		replace_len = 0, //длина замены
		mod_len = 0, //длина модиф строки
		i = 0, //счетчик внешний
		j = 0, //счетчик внутренний
		in_count = 0, //счетчик совпадений и замен	
		ok_len = 0; // длина текущего совпадения 
	str_len = getlen(str);
	inner_len = getlen(inner_str);
	replace_len = getlen(replace_str);
	mod_str = (char *) malloc(sizeof(char));
	mod_str[0] = '\0';
	//mod_str = remap(mod_str, str_len); // выделение памяти
	for (i = 0; i < str_len; i++) { // проход по исходной строке
		for (j = 0; j < inner_len; j++) { // проход по второй строке 
			if (str[i + j] == inner_str[j]) { // если есть совпадение
				ok_len++; // счетчик совпадений увеличивается 
			} else
				break;
		}
		if (ok_len == inner_len) { // и если это совпадение == длине подстроки
			append(mod_str, replace_str);
			i += (ok_len - 1);
		} else { // иначе считываем текущий элемент в модиф строки
			append_char(mod_str, str[i]);
		}
		ok_len = 0;
	}
	return mod_str;
}


char *append(char *mod_str, const char *replace){
	int oldlen = getlen(mod_str); // узнаем старую длину
	int replen = getlen(replace);
	char *newstr = (char *) realloc(mod_str, (oldlen + replen + 1) * sizeof(char)); //выделяем память
	int i = 0; //узнаем новую длину
	int k = 0; // смещение для replace
	for (i = oldlen; i < (oldlen + replen); i++) {
		newstr[i] = replace[k];
		k++;
	}
	newstr[oldlen + replen] = '\0';
	return newstr;
}

char *append_char(char *mod_str, char elem) {
	int oldlen = getlen(mod_str); // узнаем старую длину
	char *newstr = (char *) realloc(mod_str, (oldlen + 1 + 1)); //выделяем память
	newstr[oldlen] = elem;
	newstr[oldlen + 1] = '\0';
	return newstr;
}

int getlen (const char *str) {
	int i = 0;
	for (i = 0; ; i++) {
		if ( str[i] == '\0') {
			return i;
		}
	}
	return -1;
}
