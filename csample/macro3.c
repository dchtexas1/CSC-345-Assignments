#include <stdio.h>
 
int  main()
{
    #ifdef DEBUG    
  printf("Debug run\n");
    #else
  printf("Release run\n");
    #endif
}
