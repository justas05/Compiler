#include <string.h>

char *chararray0(char *, char, char, char);

int main()
{
    char array[3];
    char *test_array = "abc";
    return !( chararray0(array, 'a', 'b', 'c')[0] == test_array[0] &&
	     chararray0(array, 'a', 'b', 'c')[1] == test_array[1] &&
	     chararray0(array, 'a', 'b', 'c')[2] == test_array[2] );
}
