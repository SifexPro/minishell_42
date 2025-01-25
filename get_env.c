#include "stdio.h"

int main(int argc, char **argv, char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		printf("%s", envp[i]);
		i++;
	}
	return (0);
}