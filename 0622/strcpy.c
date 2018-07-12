#include <stdio.h>

char *mystrcpy(char *dest,const char *src)
{
  char *tmp=dest;
  while((*tmp++=*src++)!='\0');
  return dest;
}
int main(int argc,char *argv[])
{
  char arr[]="aaaaaaaaaaaaaaaaaaaaaaaa";
  char *src="Hello World";
  printf("mystrcpy: %s\n",mystrcpy(arr,src));
  return 0;
}
