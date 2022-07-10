#include"Some string functions.h"

void reverse(std::string & s)
{
	int i, j;
	char c;
	int len = s.length();
	for (i = 0, j = len - 1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
