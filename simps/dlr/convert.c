#include <stdio.h>
#include <stdint.h>

static void hacks(char *file)
{
	FILE *ihacku;
	char buffer[128 / 3 - 3];
	int n = 0, k = 0;
	int next = 0;
	if((ihacku = fopen(file, "r")) == NULL)
	return;
	while((n = fread(buffer, sizeof(char), 128 / 3 - 3, ihacku)) != 0)
	{
		int i =0;
		for(i = 0; i < n; i++)
		{
			k++;
			printf("\\x%02x", (uint8_t)buffer[i]);
		}
	}
	printf("\n");
	printf("%d\n", k);
	return;
}

int main(int argc, char **args)
{
	if(argc < 2)
	{
		printf("%s binary\n", args[0]);
		return 0;
	}
	hacks(args[1]); 
}
