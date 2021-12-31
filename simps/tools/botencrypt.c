#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("%s string\n", argv[0]);
		return 0;
	}
	int i;
	char *str = argv[1];
	for(i = 0; (i < 100 && str[i] != '\0'); i++)
	str[i] = str[i] + 3;
	printf("encrypted: %s\n", str);
}
