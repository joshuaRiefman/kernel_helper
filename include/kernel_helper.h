#ifndef KERNEL_HELPER_KERNEL_HELPER_H
#define KERNEL_HELPER_KERNEL_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Defines for readability
#define TRUE 1
#define FALSE 0
#define STRINGS_ARE_EQUAL 0

// IO
#define DEFAULT_KERNEL_FILE "kernel.cl"
#define DEFAULT_OUTPUT_FILE "out.txt"
#define IO_DIRECTORY "../data/"
#define MAX_BUFFER_SIZE 128

// Default parameter values
#define VERBOSE_DEFAULT FALSE
#define USE_BLANK_LINES_DEFAULT FALSE
#define IMMEDIATE_DIRECTORY_DEFAULT FALSE

// Set global option defaults
int verbose = VERBOSE_DEFAULT;
int use_blank_lines = USE_BLANK_LINES_DEFAULT;
int get_from_immediate_directory = IMMEDIATE_DIRECTORY_DEFAULT;

size_t get_length(const char *string, size_t max_length);
int is_blank_line(const char *string, size_t buffer_size);
void print_help();
char *getPath(char *file_name, int immediate_directory);
void process_kernel(FILE *kernel_in, FILE* kernel_out);

#endif //KERNEL_HELPER_KERNEL_HELPER_H
