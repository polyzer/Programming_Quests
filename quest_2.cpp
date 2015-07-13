//��������� ��������, ����� ����!
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
	int str_len = 0, // ����� �������� ������ 
		inner_len = 0, //����� ���������� �����
		replace_len = 0, //����� ������
		mod_len = 0, //����� ����� ������
		i = 0, //������� �������
		j = 0, //������� ����������
		in_count = 0, //������� ���������� � �����	
		ok_len = 0; // ����� �������� ���������� 
	str_len = getlen(str);
	inner_len = getlen(inner_str);
	replace_len = getlen(replace_str);
	mod_str = (char *) malloc(sizeof(char));
	mod_str[0] = '\0';
	//mod_str = remap(mod_str, str_len); // ��������� ������
	for (i = 0; i < str_len; i++) { // ������ �� �������� ������
		for (j = 0; j < inner_len; j++) { // ������ �� ������ ������ 
			if (str[i + j] == inner_str[j]) { // ���� ���� ����������
				ok_len++; // ������� ���������� ������������� 
			} else
				break;
		}
		if (ok_len == inner_len) { // � ���� ��� ���������� == ����� ���������
			append(mod_str, replace_str);
			i += (ok_len - 1);
		} else { // ����� ��������� ������� ������� � ����� ������
			append_char(mod_str, str[i]);
		}
		ok_len = 0;
	}
	return mod_str;
}


char *append(char *mod_str, const char *replace){
	int oldlen = getlen(mod_str); // ������ ������ �����
	int replen = getlen(replace);
	char *newstr = (char *) realloc(mod_str, (oldlen + replen + 1) * sizeof(char)); //�������� ������
	int i = 0; //������ ����� �����
	int k = 0; // �������� ��� replace
	for (i = oldlen; i < (oldlen + replen); i++) {
		newstr[i] = replace[k];
		k++;
	}
	newstr[oldlen + replen] = '\0';
	return newstr;
}

char *append_char(char *mod_str, char elem) {
	int oldlen = getlen(mod_str); // ������ ������ �����
	char *newstr = (char *) realloc(mod_str, (oldlen + 1 + 1)); //�������� ������
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
