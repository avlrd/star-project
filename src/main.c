#include "typedef.h"
#include "header.h"

int main(int argc, char** argv)
{
	int opt = -1;
	int opt_idx = -1;
	opterr = 0;

	if(argc < 2)
	{
		display_usage(argv[0]);
		return 1;
	}

	while((opt = getopt_long(argc, argv, optstr, options, &opt_idx)) != -1)
	{
		switch(opt)
		{
			case 'l':
				printf("list\n");
				break;

			case 'e':
				printf("extract\n");
				break;

			case 'c':
				printf("create\n");
				break;

			case 'd':
				printf("directory\n");
				break;

			case 'z':
				printf("compress\n");
				break;

			case 'v':
				printf("verbose\n");
				break;

			case 'h':
				printf("help\n");
				break;

			case '?':
				display_usage(argv[0]);
				break;

			default:
			printf("Unknown error\n");
				break;
		}
	}

	return 0;
}

void display_usage(char* bin_name)
{
	printf("Test : %s\n", bin_name);
}