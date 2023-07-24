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

#include "../include/kernel_helper.h"

////////////////////////////////////////////  PUBLIC INTERFACES  //////////////////////////////////////////////

/*
 * Load an OpenCL kernel from a *.cl file into a string that can then be built into an OpenCL
 * kernel inside a C++ program.
 * Params:
 *      kernel_in: pointer to opened, readable file
 *      kernel_size: size of the kernel file in bytes
 * Returns:
 *      pointer to a string that contains the entire kernel
 */
char* load_kernel(FILE *kernel, size_t kernel_size) {
    char* kernelSource = malloc(kernel_size);
    int character;
    size_t i = 0;
    while ((character = fgetc(kernel)) != EOF) {
        kernelSource[i] = (char)character;
        i++;
    }

    kernelSource[i] = '\0';
    return kernelSource;
}

void process_kernel(FILE *kernel, FILE* kernel_out) {
    // Create input buffer
    char *in_string_buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    size_t buffer_size;

    /*  Process the input file, line by line, placing the line in the buffer  */
    while (fgets(in_string_buffer, MAX_BUFFER_SIZE, kernel)) {
        // Identify string length
        buffer_size = get_length(in_string_buffer, MAX_BUFFER_SIZE);

        // If the line is blank, then output the blank_line string if that mode is enabled, and continue
        if (is_blank_line(in_string_buffer, buffer_size) == TRUE) {
            if (use_blank_lines) {
                if (verbose) {
                    printf("%s", blank_line);
                }
                fputs(blank_line, kernel_out);
            }
            continue;
        }

        char* out_string_buffer = process_line(in_string_buffer, buffer_size);

        // Verbose mode will print the resultant text to the screen
        if (verbose) {
            printf("%s", out_string_buffer);
        }

        // Write the output buffer into the output file, and free the output buffer.
        fputs(out_string_buffer, kernel_out);
        free(out_string_buffer);
    }

    // Clean up
    free(in_string_buffer);
}

//////////////////////////////////////////  PRIVATE FUNCTIONS  ////////////////////////////////////////////////


char* process_line(const char* in_string_buffer, size_t buffer_size) {
    // Allocate a buffer for the output string of size prefix + input + suffix characters
    // The buffer is created and destroyed every iteration of the loop instead of being created once and
    // using realloc() because using realloc() was experimentally more prone to memory issues
    char *out_string_buffer = malloc(sizeof(char) * (buffer_size + strlen(string_prefix) + strlen(string_suffix)));

    // Output string is first constructed by adding the initial `"` character.
    out_string_buffer[0] = string_prefix[0];
    // Next, append the string from the input buffer into the output buffer
    for (int i = 0; i < buffer_size; i++) {
        out_string_buffer[i + 1] = in_string_buffer[i];
    }
    // Finally, append the suffix characters to the output buffer
    for (int i = 1; i <= strlen(string_suffix); i++) {
        out_string_buffer[buffer_size + i] = string_suffix[i - 1];
    }

    return out_string_buffer;
}

/*
 * Return the length of a string buffer by sequentially checking if a character is the newline character which
 * indicates the end of the line, and returning the index of the newline character.
 * Known Issue: Undefined behaviour when newline character is absent.
 * Params:
 *      string: string in which the length will be evaluated.
 *      max_length: maximum buffer size
 * Returns:
 *      a size_t that indicates the length of the string
 * Errors:
 *      exits if the indicated length is greater than max_length
 */
size_t get_length(const char *string, size_t max_length) {
    for (int i = 0; i < max_length; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            return i;
        }
    }

    printf("Fatal Error: Maximum string length was exceeded!");
    exit(1);
}

/*
 * Check for blank lines by returning FALSE when a non-whitespace character is encountered, and TRUE
 * if no non-whitespace character is encountered.
 * Params:
 *      string: string that will be evaluated to be blank or not.
 *      buffer_size: size of `string`
 * Returns:
 *      TRUE if line is blank (all whitespace), and FALSE otherwise
 */
int is_blank_line(const char *string, size_t buffer_size) {
    for (int i = 0; i < buffer_size; i++) {
        if (!isspace(string[i])) {
            return FALSE;
        }
    }

    return TRUE;
}