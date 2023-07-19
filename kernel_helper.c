//
// Created by Joshua Riefman on 2023-07-18.
//
// File:        kernel_helper.c
//
// Abstract:    This is a helper program to convert a *.cl kernel file
//              into a format that is able to be pasted into a char[]
//              within a C++ program to be then built as a OpenCL kernel.
//
// Version:     <1.0>
//
// Note:        Expects a blank line at the end of file, else undefined behaviour can occur.
//              The above is a known issue, which will be in a future version.
//              Use -h or --help to print help message for usage instructions.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define EQUAL 0

#define DEFAULT_KERNEL_FILE "test_kernel.cl"
#define DEFAULT_OUTPUT_FILE "out.txt"
#define MAX_BUFFER_SIZE 128
#define VERBOSE_DEFAULT 0
#define USE_BLANK_LINES_DEFAULT 0


size_t get_length(const char *string, size_t max_length);
int is_blank_line(const char *string, size_t buffer_size);
void print_help();

int main(int argc, char *argv[]) {
    FILE *raw_kernel_in;
    FILE *parsed_kernel_out;

    int got_inFile = 0;
    int got_outFile = 0;

    int verbose = VERBOSE_DEFAULT;
    int use_blank_lines = USE_BLANK_LINES_DEFAULT;
    if (argc != 0) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "-h") == EQUAL || strcmp(argv[i], "--help") == EQUAL) {
                print_help();
                exit(0);
            }
            if (strcmp(argv[i], "-b") == EQUAL) {
                use_blank_lines = 1;
            }
            if (strcmp(argv[i], "-v") == EQUAL) {
                verbose = 1;
            }
            if (strcmp(argv[i], "-f") == EQUAL) {
                raw_kernel_in = fopen(argv[i + 1], "r");
                got_inFile = 1;
                i++;
            }
            if (strcmp(argv[i], "-o") == EQUAL) {
                parsed_kernel_out = fopen(argv[i + 1], "w");
                got_outFile = 1;
                i++;
            }
        }
    }
    if (!got_inFile) {
        raw_kernel_in = fopen(DEFAULT_KERNEL_FILE, "r");
    }
    if (!got_outFile) {
        parsed_kernel_out = fopen(DEFAULT_OUTPUT_FILE, "w");
    }

    if (raw_kernel_in == NULL) {
        printf("Fatal Error: Kernel was not found!\n");
        return 1;
    }
    if (parsed_kernel_out == NULL) {
        printf("Fatal Error: Output file was not found or created!\n");
        return 1;
    }

    char *in_string_buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    char string_prefix[] = { '"' };
    char string_suffix[] = {' ', '\\', 'n', '"', '\n', '\0' };
    char blank_line[] = { '"', ' ', '\\', 'n', '"', '\n', '\0' };
    size_t buffer_size;

    while (fgets(in_string_buffer, MAX_BUFFER_SIZE, raw_kernel_in)) {
        buffer_size = get_length(in_string_buffer, MAX_BUFFER_SIZE);
        char *out_string_buffer = malloc(sizeof(char) * buffer_size + strlen(string_prefix) + strlen(string_suffix));

        if (is_blank_line(in_string_buffer, buffer_size) == TRUE) {
            if (use_blank_lines) {
                if (verbose) {
                    printf("%s", blank_line);
                }
                fputs(blank_line, parsed_kernel_out);
            }
            continue;
        }

        out_string_buffer[0] = string_prefix[0];
        for (int i = 0; i < buffer_size; i++) {
            out_string_buffer[i + 1] = in_string_buffer[i];
        }
        for (int i = 1; i <= strlen(string_suffix); i++) {
            out_string_buffer[buffer_size + i] = string_suffix[i - 1];
        }

        if (verbose) {
            printf("%s", out_string_buffer);
        }
        fputs(out_string_buffer, parsed_kernel_out);
        free(out_string_buffer);

    }

    fclose(raw_kernel_in);
    fclose(parsed_kernel_out);
    free(in_string_buffer);
}

/*
 * Return the length of a string buffer by sequentially checking if a character is the newline character which
 * indicates the end of the line, and returning the index of the newline character.
 * Known Issue: Undefined behaviour when newline character is absent.
 * Params:
 *      string: string in which the length will be evaluated.
 *      max_length: maximum buffer size
 * Returns: a size_t that indicates the length of the string
 * Errors: exits if the indicated length is greater than max_length
 */
size_t get_length(const char *string, size_t max_length) {
    for (int i = 0; i < max_length; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            return i;
        }
    }

    printf("Maximum string length was exceeded!");
    exit(1);
}

/*
 * Check for blank lines by returning FALSE when a non-whitespace character is encountered, and TRUE
 * if no non-whitespace character is encountered.
 * Params:
 *      string: string that will be evaluated to be blank or not.
 *      buffer_size: size of `string`
 */
int is_blank_line(const char *string, size_t buffer_size) {
    for (int i = 0; i < buffer_size; i++) {
        if (!isspace(string[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

/*
 * Print a help message detailing usage, options, and flags.
 */
void print_help() {
    char help[] = {
        "usage: kernel_helper [options]\n"
        "  example: kernel_helper -f kernel.cl -o out.txt -v\n"
        "  options:\n"
        "    -h, --help         Display help\n"
        "    -v                 Activate verbose mode\n"
        "    -b                 Enable output of blank lines\n"
        "    -f [kernel_file]   Indicate kernel file for the program to process\n"
        "    -o [output_file]   Indicate an output file for the program\n"
    };

    printf("%s", help);
}
