#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t  pid;

  pid = fork();
  if (pid == 0) { 
    printf("I am when fork returns 0");
    execlp("/bin/ls", "ls", NULL);
  }
  else if (pid < 0) { 
    printf("I am when fork returns less than 0");
  }
  else {
    wait (NULL);
    printf ("the last else");
  }
}
