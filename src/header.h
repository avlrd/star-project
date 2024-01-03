/**
 * @file header.h
 * 
 * @brief This file contains the function prototypes for the functions defined in header.c.
*/

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
 * @brief Displays the usage information for the program.
 *
 * @param bin_name The name of the binary file.
 */
void display_usage(const char* bin_name);

/**
 * @brief Creates an archive from a directory.
 * 
 * @param archive
 * @param dir
*/
void create_archive(char* archive, char* dir);

/**
 * @brief Lists archive files.
 * 
 * @param archive
*/
void list_archive(char *archive);

/**
 * @brief Extracts the contents of an archive.
 *
 * This function takes a pointer to a null-terminated string representing the path to the archive file.
 * It extracts the contents of the archive and saves them to the appropriate location.
 *
 * @param archive A pointer to a null-terminated string representing the path to the archive file.
 */
void extract_archive(char *archive);

/**
 * @brief Compresses archive files.
 * 
 * @param archive
*/
int compress_archive(char *archive);

/**
 * @brief Decompresses archive files.
 * 
 * @param archive
*/
int decompress_archive(char *archive);

/**
 * @brief Prints a debug message.
 *
 * This function is used to print debug messages to the console.
 * It takes a format string and optional arguments, similar to printf.
 *
 * @param msg The format string for the debug message.
 * @param ... Optional arguments to be formatted and printed.
 */
void debug(const char* msg, ...);

#endif