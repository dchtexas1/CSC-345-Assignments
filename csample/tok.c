#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char str[100];
enum {NOT_FOUND=0,FOUND};
static char *ptr;

char *mystrtok(char* string)
{
  int j,flag=NOT_FOUND;
  char *p; // temp pointer to store the current substring

  if(string != NULL)   // not end of input
    {
      ptr=string;
      p=string;
    }
  else   // no more input 
    {
      if(*ptr == '\0')
	return NULL;
 
      p=ptr;
    }
 
  while(*ptr != '\0')  // while not last character or end of input
    {
      if(*ptr == ' ')   // skip a space  
            {
	      if(ptr == p)  // if this is the beginning of the scanning process
                {           // then rolling the current substring p and ptr to next
		  p++;
		  ptr++;
                }
	      else          // if p != p, that means we got a token
                {
		  *ptr='\0';
		  ptr++;
 
		  return p;
                }
            }
      ptr++;   // in cases, no space, we got a char which is part of the substring, then move ptr to the next char.
    }
  return p;
}
 
int main()
{
  int i;
  char *p_str,*token;
 
  printf("\n Enter a string to tokenize: ");
  scanf("%[^\n]",str);
     
  /* simple tokenizing */  
  for (i = 1, p_str = str; ; i++, p_str = NULL)
    {
      token = mystrtok(p_str);
      if (token == NULL)
	break;
      printf("\n%d: %s",i,token);
    }
  printf("\n");
}
