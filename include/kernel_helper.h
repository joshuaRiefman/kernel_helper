///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                           //
// File:        kernel_helper.c                                                                              //
//                                                                                                           //
// Abstract:    This is a library with two public interfaces to convert OpenCL kernel                        //
//              files (*.cl) to either a paste-able format into the source code of                           //
//              a C++ program, or loaded directly into a C++ program at runtime.                             //
//                                                                                                           //
// Version:     <1.0>                                                                                        //
//                                                                                                           //
// Usage:       Use the PUBLIC INTERFACES to either format a kernel in advance, or at runtime.               //
//                                                                                                           //
// Interfaces:  load_kernel(FILE *kernel, size_t kernel_size) - load kernel as string at runtime             //
//              process_kernel(FILE *kernel, FILE* kernel_out) - format kernel to paste-able format          //
//                                                                                                           //
// Note:        Expects a blank line at the end of file, else UNDEFINED BEHAVIOUR WILL OCCUR.                //
//              The above is a known issue, which should be fixed in a future version.                       //
//                                                                                                           //
// Example:     __kernel void example(                                                                       //
//                  __global float* output_buffer)                                                           //
//              {                                                                                            //
//                  int i = get_global_id(0);                                                                //
//                  output_buffer[i] = i;                                                                    //
//              }                                                                                            //
//                                                                                                           //
//              The above will be reformatted to the following:                                              //
//                                                                                                           //
//              "__kernel void example( \n"                                                                  //
//              "   __global float* output_buffer) \n"                                                       //
//              "{ \n"                                                                                       //
//              "   int i = get_global_id(0); \n"                                                            //
//              "   output_buffer[i] = i; \n"                                                                //
//              "} \n"                                                                                       //
//                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
#define IO_DIRECTORY "data/"
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

// Functions
size_t get_length(const char *string, size_t max_length);
int is_blank_line(const char *string, size_t buffer_size);
char* process_line(const char* in_string_buffer, size_t in_string_size);

// Interfaces
void process_kernel(FILE *kernel, FILE* kernel_out);
char* load_kernel(FILE *kernel, size_t kernel_size);

#endif //KERNEL_HELPER_KERNEL_HELPER_H
