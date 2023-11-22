#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <getopt.h>

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