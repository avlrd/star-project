#include "typedef.h"
#include "header.h"

int main(int argc, char** argv)
{
	int opt = -1;
	int opt_idx = -1;
	opterr = 0;

	debug("Number of args: %d\n", argc);

	if(argc == 1)
	{
		display_usage(argv[0]);
		return 1;
	}

	char* archive_name = NULL;
	char* source_dir = NULL;
	int LF = 0, EF = 0, CF = 0, ZF = 0, UF = 0, VF = 0;

	// Parsing des options
	while((opt = getopt_long(argc, argv, optstr, options, &opt_idx)) != -1)
	{
		debug("Option: %c\n", opt);
		switch(opt)
		{
			case 'l':
				LF = 1;
				archive_name = optarg;
				break;

			case 'e':
				EF = 1;
				archive_name = optarg;
				break;

			case 'c':
				CF = 1;
				archive_name = optarg;
				break;

			case 'd':
				source_dir = optarg;
				break;

			case 'z':
				ZF = 1;
				archive_name = optarg;
				break;

			case 'u':
				UF = 1;
				archive_name = optarg;
				break;

			case 'v':
				VF = 1;
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

	// Verbose pas encore fait
	if(VF)
	{
		printf("verbose\n");
	}

	// Processus en fonction des options
	if(LF)
	{
		printf("list\n");
		if(archive_name == NULL)
		{
			fprintf(stderr, "No archive specified\n");
			return 1;
		}
		list_archive(archive_name);
	}
	else if(EF)
	{
		printf("extract\n");
		if(archive_name == NULL)
		{
			fprintf(stderr, "No archive specified\n");
			return 1;
		}
		extract_archive(archive_name);
	}
	else if(CF)
	{
		printf("create\n");
		if(source_dir == NULL)
		{
			fprintf(stderr, "No source directory specified\n");
			return 1;
		}
		create_archive(archive_name, source_dir);
	}
	else if(ZF)
	{
		printf("compress\n");
		if(archive_name == NULL)
		{
			fprintf(stderr, "No archive specified\n");
			return 1;
		}
		compress_archive(archive_name);
	}
	else if(UF)
	{
		printf("decompress\n");
		if(archive_name == NULL)
		{
			fprintf(stderr, "No archive specified\n");
			return 1;
		}
		decompress_archive(archive_name);
	}
	else
	{
		display_usage(argv[0]);
		return 1;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/

int octal_to_int(const char *octal) {
	return strtol(octal, NULL, 8);
}

void extract_archive(char *archive) {
	int fd = open(archive, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	struct posix_header header;
	// On boucle sur les donnees à lire
	while (read(fd, &header, sizeof(header)) > 0) {
		if (header.name[0] != '\0') {
			int size = octal_to_int(header.size);

			// Vérifie si le chemin correspond à un répertoire
			int is_directory = (header.typeflag == '5' || header.name[strlen(header.name) - 1] == '/');

			if (is_directory) {
				// Crée le répertoire avec les permissions spécifiées dans l'en-tête
				mkdir(header.name, octal_to_int(header.mode));
			} else {
				// Crée le fichier avec les permissions spécifiées dans l'en-tête
				int file_fd = creat(header.name, octal_to_int(header.mode));
				if (file_fd == -1) {
					perror("creat");
					close(fd);
					exit(1);
				}

				// Copie les données du fichier depuis l'archive vers le fichier créé
				char buffer[4096];
				int remaining_data = size;
				while (remaining_data > 0) {
					int read_size = (remaining_data > (int) sizeof(buffer)) ? (int) sizeof(buffer) : remaining_data;
					if (read(fd, buffer, read_size) != read_size) {
						perror("read");
						close(fd);
						close(file_fd);
						exit(1);
					}
					if (write(file_fd, buffer, read_size) != read_size) {
						perror("write");
						close(fd);
						close(file_fd);
						exit(1);
					}
					remaining_data -= read_size;
				}

				// Passe au prochain en-tête dans l'archive sans ajuster la position
			}

			// Passe au prochain en-tête dans l'archive
			lseek(fd, (size + 511) / 512 * 512 - size, SEEK_CUR);
		}
	}

	close(fd);
}

/*---------------------------------------------------------------------------*/

void list_archive(char *archive) {
    int fd = open(archive, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct posix_header header;

    // Fonction pour vérifier la fin de l'archive
    int is_end_of_archive = 0;

    while (!is_end_of_archive) {
        // Lire l'en-tête
        if (read(fd, &header, sizeof(header)) <= 0) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Vérifier si c'est la fin de l'archive
        for (size_t i = 0; i < sizeof(header); i++) {
            if (((char*)&header)[i] != '\0') {
                is_end_of_archive = 0;  // Il y a encore des données non nulles
                break;
            }
            is_end_of_archive = 1;  // Tous les octets sont nuls, c'est la fin de l'archive
        }

        if (!is_end_of_archive) {
            printf("%s\n", header.name);

            // Calculer la taille du fichier en utilisant strtol
            int filesize = strtol(header.size, NULL, 8);

            // Gestion des tailles non multiples de 512
            int padding = (filesize % 512 != 0) ? 512 - (filesize % 512) : 0;
            lseek(fd, filesize + padding, SEEK_CUR);
        }
    }

    close(fd);
}

/*---------------------------------------------------------------------------*/

char get_file_type(struct stat st) {
	if (S_ISREG(st.st_mode)) {
		return '0'; // Fichier ordinaire
	} else if (S_ISDIR(st.st_mode)) {
		return '5'; // Répertoire
	} else if (S_ISLNK(st.st_mode)) {
		return '2'; // Lien symbolique
	} else {
		return '\0'; // Type non pris en charge
	}
}

unsigned int calculate_checksum(struct posix_header*header) {
	unsigned int sum = 0;
	char *p = (char *)header;

	for (size_t i = 0; i < sizeof(struct posix_header); ++i) {
		sum += p[i];
	}

	// Remplir l'espace checksum avec des espaces
	for (size_t i = 0; i < sizeof(header->checksum); ++i) {
		sum += ' ';
	}

	return sum;
}

void copy_file_contents(int source_fd, int dest_fd, size_t size) {
	char buffer[512];
	ssize_t bytesRead;

	while ((bytesRead = read(source_fd, buffer, sizeof(buffer))) > 0) {
		if (write(dest_fd, buffer, bytesRead) != bytesRead) {
			perror("write");
			exit(1);
		}
	}

	if (bytesRead == -1) {
		perror("read");
		exit(1);
	}

	// Remplir le reste du bloc avec des zéros si nécessaire
	while (size % 512 != 0) {
		char zero = 0;
		if (write(dest_fd, &zero, 1) != 1) {
			perror("write");
			exit(1);
		}
		size++;
	}
}

void add_elem_to_tar(char *archive, char *name)
{
	struct stat st;
	if (lstat(name, &st) == -1) {
		perror("lstat");
		exit(1);
	}

	// Remplissage de l'en-tête
	struct posix_header header;
	memset(&header, 0, sizeof(header));
	snprintf(header.name, sizeof(header.name), "%s", name);
	snprintf(header.mode, sizeof(header.mode), "%07o", st.st_mode & 0777);
	snprintf(header.uid, sizeof(header.uid), "%07o", st.st_uid);
	snprintf(header.gid, sizeof(header.gid), "%07o", st.st_gid);
	snprintf(header.size, sizeof(header.size), "%011lo", (unsigned long)st.st_size);
	snprintf(header.mtime, sizeof(header.mtime), "%011lo", (unsigned long)st.st_mtime);
	header.typeflag = get_file_type(st);
	snprintf(header.magic, sizeof(header.magic), "ustar");
	snprintf(header.version, sizeof(header.version), "0");

	// Calculer checksum
	unsigned int checksum = calculate_checksum(&header);
	snprintf(header.checksum, sizeof(header.checksum), "%07o", checksum);
	int fd_archive = open(archive, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 0777);
	if (fd_archive == -1) {
		perror("open archive");
		exit(1);
	}
	write(fd_archive, &header, sizeof(header));

	// Copie du contenu du fichier
	int file_fd = open(name, O_RDONLY);
	if(header.typeflag == '0')
		copy_file_contents(file_fd, fd_archive, st.st_size);
	close(file_fd);

}

void archive_build(char* archive, char* source)
{
	add_elem_to_tar(archive, source);

	DIR* dir = opendir(source);
	if(!dir)
	{
		perror("opendir");
		exit(1);
	}

	struct dirent* entry;

	while((entry = readdir(dir)) != NULL)
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			char file_path[PATH_MAX];
			snprintf(file_path, sizeof(file_path), "%s/%s", source, entry->d_name);

			if(entry->d_type == DT_DIR)
			{
				closedir(dir); // on ferme pour éviter le "Too many open files"
				// on pourrait également créer plusieurs processus pour gérer les différents fichiers
				archive_build(archive, file_path);
				dir = opendir(source); // et on réouvre
				if(!dir)
				{
					perror("opendir");
					exit(1);
				}
			}
			else
			{
				add_elem_to_tar(archive, file_path);
			}
		}
	}
	closedir(dir);
}

void create_archive(char* archive, char* source)
{
	// Récursion sur les sous-répertoires
	archive_build(archive, source);

	int archive_fd = open(archive, O_WRONLY | O_APPEND);
	if(archive_fd == -1)
	{
		perror("open");
		exit(1);
	}

	// Ajout de deux blocs de 512 octets remplis de zéros à la fin de l'archive
	char zero_block[512] = {0};
	for(int i = 0; i < 2; i++)
	{
		if(write(archive_fd, zero_block, sizeof(zero_block)) != sizeof(zero_block))
		{
			perror("write zero block");
			close(archive_fd);
			exit(1);
		}
	}

	close(archive_fd);
}

/*---------------------------------------------------------------------------*/

int compress_archive(char* archive)
{
	int ret = Z_OK;
	z_stream stream = {0};
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];
	FILE *source_file = NULL, *dest_file = NULL;
	char dest[256];

	// Nom du fichier dest = nom du fichier source + .gz
	snprintf(dest, sizeof(dest), "%s.gz", archive);

	source_file = fopen(archive, "rb");
	if (source_file == NULL)
	{
		perror("Unable to open source file");
		return Z_ERRNO;
	}

	dest_file = fopen(dest, "wb");
	if (dest_file == NULL)
	{
		perror("Unable to open destination file");
		fclose(source_file); // Clean up source file
		return Z_ERRNO;
	}

	// Stream init
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
	if (ret != Z_OK)
	{
		fclose(source_file);
		fclose(dest_file);
		return ret;
	}

	// Lecture et compression
	while (1)
	{
		stream.avail_in = fread(in, 1, CHUNK, source_file);
		if (ferror(source_file))
		{
			deflateEnd(&stream);
			fclose(source_file);
			fclose(dest_file);
			perror("Error reading source file");
			return Z_ERRNO;
		}

		int flush = feof(source_file) ? Z_FINISH : Z_NO_FLUSH;
		stream.next_in = in;

		do
		{
			stream.avail_out = CHUNK;
			stream.next_out = out;
			ret = deflate(&stream, flush);
			assert(ret != Z_STREAM_ERROR);
			size_t have = CHUNK - stream.avail_out;
			if (fwrite(out, 1, have, dest_file) != have || ferror(dest_file))
			{
				deflateEnd(&stream);
				fclose(source_file);
				fclose(dest_file);
				perror("Error writing to destination file");
				return Z_ERRNO;
			}
		} while (stream.avail_out == 0);

		if (flush == Z_FINISH)
		{
			break;
		}
	}

	deflateEnd(&stream);
	fclose(source_file);
	fclose(dest_file);

	// Supprimer le fichier tar
	if (remove(archive) != 0)
	{
		perror("Error removing original file");
		return Z_ERRNO;
	}

	return Z_OK;
}

/*---------------------------------------------------------------------------*/

int decompress_archive(char* archive)
{
	char dest[256];
	strncpy(dest, archive, sizeof(dest) - 1);
	dest[sizeof(dest) - 1] = '\0';

	// Enlever .gz de la fin du nom du fichier
	size_t len = strlen(dest);
	if (len > 3 && strcmp(dest + len - 3, ".gz") == 0)
	{
		dest[len - 3] = '\0';
	}
	else
	{
		fprintf(stderr, "Error: Input file does not have a .gz extension.\n");
		return Z_DATA_ERROR;
	}

	FILE *source_file = fopen(archive, "rb");
	if (!source_file)
	{
		perror("Unable to open source file");
		return Z_ERRNO;
	}

	FILE *dest_file = fopen(dest, "wb");
	if (!dest_file)
	{
		perror("Unable to open destination file");
		fclose(source_file);
		return Z_ERRNO;
	}

	z_stream stream = {0};
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	
	if (inflateInit2(&stream, 15 + 32) != Z_OK)
	{
		fclose(source_file);
		fclose(dest_file);
		return Z_MEM_ERROR;
	}

	int ret;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	while (1)
	{
		stream.avail_in = fread(in, 1, CHUNK, source_file);
		if (ferror(source_file))
		{
			inflateEnd(&stream);
			fclose(source_file);
			fclose(dest_file);
			return Z_ERRNO;
		}
		if (stream.avail_in == 0)
			break;
		stream.next_in = in;

		while (stream.avail_in != 0)
		{
			stream.avail_out = CHUNK;
			stream.next_out = out;
			ret = inflate(&stream, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);
			switch (ret)
			{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					fclose(source_file);
					fclose(dest_file);
					return ret;
			}
			size_t have = CHUNK - stream.avail_out;
			if (fwrite(out, 1, have, dest_file) != have || ferror(dest_file))
			{
				inflateEnd(&stream);
				fclose(source_file);
				fclose(dest_file);
				return Z_ERRNO;
			}
		}
	}

	// Supprimer le fichier gz
	if (remove(archive) != 0)
	{
		perror("Error removing original file");
		return Z_ERRNO;
	}

	inflateEnd(&stream);
	fclose(source_file);
	fclose(dest_file);

	if (ret != Z_STREAM_END)
	{
		return Z_DATA_ERROR;
	}

	return Z_OK;
}

/*---------------------------------------------------------------------------*/

void display_usage(const char* bin_name)
{
	fprintf(stderr, "Usage : %s [OPTION...] [FILE]...\n\n", bin_name);
	fprintf(stderr, "OPTIONS:\n\n");
	fprintf(stderr, "\t-l,\t--list\t\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-e,\t--extract\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-c,\t--create\tARCHIVE_FILE\n");
	fprintf(stderr, "\t-d,\t--directory\tDIRECTORY_TO_PROCESS\n");
	fprintf(stderr, "\t-z,\t--compress\tARCHIVE_FILE\n");
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