#include "toml.h"

int main(int argc, char *argv[])
{
	char buffer[2];	
	t_reader reader = create_reader(0, buffer, sizeof(buffer));

	read_toml(&reader);
	printf("%c", reader_peek(&reader));
}