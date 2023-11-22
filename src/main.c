#include "typedef.h"
#include "header.h"

int main(int argc, char** argv)
{
	int opt = -1;
	int opt_idx = -1;
	opterr = 0;

	debug("Number of args: %d\n", argc);

	//peut etre pas utile
	if(argc == 1)
	{
		display_usage(argv[0]);
		return 1;
	}

	while((opt = getopt_long(argc, argv, optstr, options, &opt_idx)) != -1)
	{
		debug("Option: %c\n", opt);
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

				create_archive(optarg, argv, argc);

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
				display_usage(argv[0]);
				return 0;

			case '?':
				display_usage(argv[0]);
				return 1;

			default:
			printf("Unknown error\n");
				break;
		}
	}

	return 0;
}

void create_archive(char* archive_name, char** files, int files_end_nb)
{
	TAR* archive;

	if(tar_open(&archive, archive_name, NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU) != 0)
	{
		fprintf(stderr, "Error while opening archive %s\n", archive_name);
		return;
	}

	debug("Successfully created archive %s\n", archive_name);

	for(int i = 3; i < files_end_nb; i++)
	{
		if(tar_append_file(archive, files[i], NULL) != 0)
		{
			fprintf(stderr, "Error while appending file %s to archive %s\n", files[i], archive_name);
			return;
		}

		debug("Successfully appended file %s to archive %s\n", files[i], archive_name);
	}

	if(tar_append_eof(archive) != 0)
	{
		fprintf(stderr, "Error while appending EOF to archive %s\n", archive_name);
		return;
	}

	debug("Successfully appended EOF to archive %s\n", archive_name);

	if(tar_close(archive) != 0)
	{
		fprintf(stderr, "Error while closing archive %s\n", archive_name);
		return;
	}

	debug("Successfully closed archive %s\n", archive_name);

	debug("Success\n");
}

void display_usage(const char* bin_name)
{
	fprintf(stderr, "Usage : %s [OPTION...] [FILE]...\n\n", bin_name);
	fprintf(stderr, "OPTIONS:\n\n");
	fprintf(stderr, "\t-l,\t--list\t\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-e,\t--extract\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-c,\t--create\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-d,\t--directory\tDIRECTORY_TO_PROCESS\n");
	fprintf(stderr, "\t-z,\t--compress\n");
	fprintf(stderr, "\t-v,\t--verbose\t: enable *verbose* mode\n");
	fprintf(stderr, "\t-h,\t--help\t\t: display this help\n\n");
	fprintf(stderr, "See manual for more details.\n");
}

void debug(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	fprintf(stderr, "[DEBUG] - ");
	vfprintf(stderr, msg, args);

	va_end(args);
}