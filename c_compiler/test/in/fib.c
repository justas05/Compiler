int main()
{
   unsigned long int n, first = 0, second = 1, next, c;

   n = 10;
 
   for ( c = 0 ; c < n ; c++ )
   {
      if ( c <= 1 )
         next = c;
      else
      {
         next = first + second;
         first = second;
         second = next;
      }
   }
 
   return next;
}
