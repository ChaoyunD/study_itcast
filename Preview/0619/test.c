#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char buff[256];
static char *string;
int main()
{
	printf("Please input your string:");
	string=buff;
	gets(string);
	printf("\nYour string is:%s\n",string);
}
