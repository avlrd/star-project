#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <tar.h>
#include <sys/sysmacros.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <zlib.h>
#include <assert.h>

/**
 * Displays the usage of the binary command on the terminal.
 * 
 * @param bin_name name of the binary
*/
void display_usage(const char* bin_name);

/**
 * Creates an archive from a directory.
 * 
 * @param archive
 * @param dir
*/
void create_archive(char* archive, char* dir);

/**
 * Lists archive files.
 * 
 * @param archive
*/
void list_archive(char *archive);

/**
 * Extracts archive files.
 * 
 * @param archive
*/
void extract_archive(char *archive);

/**
 * Compresses archive files.
 * 
 * @param archive
*/
int compress_archive(char *archive);

void debug(const char* msg, ...);

#endif