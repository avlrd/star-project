#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <getopt.h>

struct posix_header {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag;
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};

/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * @see man 3 getopt_long or getopt
 * @see struct option definition
 */
static struct option options[] =
{
	{ "list",		required_argument,	0,	'l'},
	{ "extract",	required_argument,	0,	'e'},
	{ "create",		required_argument,	0,	'c'},
	{ "directory",	required_argument,	0,	'd'},
	{ "compress",	no_argument,		0,	'z'},
	{ "verbose",	no_argument,		0,	'v'},
	{ "help",		no_argument,		0,	'h'},
	{ 0, 			0,					0,	 0 }
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* optstr = "l:e:c:d:zvh";

#endif