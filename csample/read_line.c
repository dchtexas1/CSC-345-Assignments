#include<stdio.h>
#include<stdlib.h>

char str[100];

int main()
{
  int i;
  char *p_str,*token;
 
  printf("\n Enter a string to tokenize: ");
  scanf("%[^\n]",str);
  printf("\nyour input is \"%s\"\n",str);
}
