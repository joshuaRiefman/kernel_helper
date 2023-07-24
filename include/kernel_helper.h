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
#define DEFAULT_OUTPUT_FILE "example.txt"
#define IO_DIRECTORY "../data/"
#define MAX_BUFFER_SIZE 128

// Default parameter values
#define VERBOSE_DEFAULT FALSE
#define USE_BLANK_LINES_DEFAULT FALSE
#define IMMEDIATE_DIRECTORY_DEFAULT FALSE

// Set global option defaults
static int verbose = VERBOSE_DEFAULT;
static int use_blank_lines = USE_BLANK_LINES_DEFAULT;
static int use_immediate_directory = IMMEDIATE_DIRECTORY_DEFAULT;

// Define global strings that will be used
static const char string_prefix[] = { '"' };
static const char string_suffix[] = {' ', '\\', 'n', '"', '\n', '\0' };
static const char blank_line[] = { '"', ' ', '\\', 'n', '"', '\n', '\0' };

// Declarations for private functions
static size_t get_length(const char *string, size_t max_length);
static int is_blank_line(const char *string, size_t buffer_size);
static void print_help();
static char *get_file_path(char *file_name, int immediate_directory);
static char* process_line(const char* in_string_buffer, size_t buffer_size);

// Declarations for public interfaces
void process_kernel(FILE *kernel, FILE* kernel_out);
char* load_kernel(FILE *kernel, size_t kernel_size);

#endif //KERNEL_HELPER_KERNEL_HELPER_H
