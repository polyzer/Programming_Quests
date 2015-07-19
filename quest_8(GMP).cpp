#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *a = "aaaa";
char *b = "bbbb";

char *geta() {
     return a;
}

void getb(char **p) {
    *p = b;
}

void setFree(char **p)
{
	free(*p);
	*p = NULL;
}

int main(int argc, char **argv) {
	char *p;

	p = geta();
	printf("%s\n", p);

	getb(&p);
	printf("%s\n", p);
	p = (char *) malloc((strlen("hello") + 1) * sizeof(char));
	strcmp(p, "hello");
	setFree(&p);
	if (p == NULL)
	{
		printf("It's FREE");
	}
	system("pause");
	return 0;
}
