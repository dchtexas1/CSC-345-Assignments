#include <stdio.h>

//function declaration
int mult ( int x, int y );

int main()
{
  int x;
  int y;
  printf( "Please input two numbers to be multiplied: " );
  scanf( "%d", &x );
  scanf( "%d", &y );
  printf( "The product of your two numbers is %d\n", mult( x, y ) );
  return 0;
}

//define the function body
//return value: int
//utility: return the multiplication of two integer values
//parameters: take two int parameters
int mult (int x, int y)
{
  return x * y;
}

