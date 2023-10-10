#include <stdio.h>
#include <stdlib.h>

# define SIZE 999999

int main(int argc, char **argv, char **env)
{
	char 	*content;
	int		index;

	index = 0;
	content = (char *)malloc(SIZE);
	if (!content)
		return (1);
	for (int i = 0; i < SIZE; ++i)
		content[i] = '\0';
	for (int i = 0; env[i]; ++i)
	{
		for (int j = 0; env[i][j]; ++j)
			content[index++] = env[i][j];
		content[index++] = '\n';
	}
	printf("%s\r\n", "HTTP/1.1 200 OK");
	printf("%s\r\n", "Content-Type: text/plain");
	printf("%s%i\r\n\r\n", "Content-Length: ", index);
	printf("%s\n", content);
	free(content);
	return (0);
}