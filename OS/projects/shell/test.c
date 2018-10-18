#include <stdio.h>
#include <stdbool.h>
int main()
{
	int i = 0;
	while(true)
	{
		i++;
		if (i == 1000000)
			i = 0;
	}

	return 0;
}
