#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <libtar.h>
#include <stdbool.h>
#include <stdarg.h>
#include <fcntl.h>

/**
 * Displays the usage of the binary command on the terminal.
 * 
 * @param bin_name name of the binary
*/
void display_usage(const char* bin_name);

/**
 * Prints a debug formatted message, used only for debugging.
 * Works the same as printf.
 *
 * @param msg Debug message
 */
void debug(const char* msg, ...);

/**
 * Creates an archive with the given name and files.
 * 
 * @param archive_name name of the archive
 * @param files files to add to the archive
 * @param nb_files number of files to add to the archive
*/
void create_archive(char* archive_name, char** files, int nb_files);

#endif