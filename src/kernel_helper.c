////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:        kernel_helper.c
//
// Abstract:    This is a helper program to convert a *.cl kernel file
//              into a format that is able to be pasted into a char[]
//              within a C++ program to be then built as a OpenCL kernel.
//
// Version:     <1.0>
//
// Note:        Kernel *.cl file must be in the same directory/folder as the program executable.
//              Expects a blank line at the end of file, else UNDEFINED BEHAVIOUR WILL OCCUR.
//              The above is a known issue, which will be in a future version.
//              Use -h or --help to print help message for usage instructions.
//
// Example:     __kernel void example(
//                  __global float* output_buffer)
//              {
//                  int i = get_global_id(0);
//                  output_buffer[i] = i;
//              }
//
//              The above will be reformatted to the following:
//
//              "__kernel void example( \n"
//              "   __global float* output_buffer) \n"
//              "{ \n"
//              "   int i = get_global_id(0); \n"
//              "   output_buffer[i] = i; \n"
//              "} \n"
//
//              Once again, the text is reformatted such that it can
//              be pasted into a C++ program to be built into an OpenCL kernel.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/kernel_helper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    // Input and Output Files
    FILE *kernel_in;
    FILE *kernel_out;

    // File addresses
    char *kernel_filename = NULL;
    char *output_filename = NULL;

    /* Loop through command-line arguments, matching them to options and setting the corresponding argument */
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-h") == STRINGS_ARE_EQUAL || strcmp(argv[i], "--help") == STRINGS_ARE_EQUAL) {
            print_help();
            exit(0); // No need to continue program execution when help message is printed
        }
        if (strcmp(argv[i], "-b") == STRINGS_ARE_EQUAL) {
            use_blank_lines = TRUE;
        }
        if (strcmp(argv[i], "-v") == STRINGS_ARE_EQUAL) {
            verbose = TRUE;
        }
        if (strcmp(argv[i], "-a") == STRINGS_ARE_EQUAL) {
            get_from_immediate_directory = TRUE;
        }
        if (strcmp(argv[i], "-f") == STRINGS_ARE_EQUAL) {
            kernel_filename = argv[i + 1];
            i++; //We've already processed the next argument, so increment again
        }
        if (strcmp(argv[i], "-o") == STRINGS_ARE_EQUAL) {
            output_filename = argv[i + 1];
            i++;
        }
    }

    // Open files
    char *in_address = getPath(kernel_filename != NULL ? kernel_filename : DEFAULT_KERNEL_FILE, get_from_immediate_directory);
    kernel_in = fopen(in_address, "r");
    if (kernel_in == NULL) {
        printf("Fatal Error: Kernel was not found!\n");
        return 1;
    }

    char *out_address = getPath(output_filename != NULL ? output_filename : DEFAULT_OUTPUT_FILE, get_from_immediate_directory);
    kernel_out = fopen(out_address, "w");
    if (kernel_out == NULL) {
        fclose(kernel_in); // kernel_in will be open if we get to here, so just make sure to close it
        printf("Fatal Error: Output file was not found or created!\n");
        return 1;
    }

    process_kernel(kernel_in, kernel_out);

    fclose(kernel_in);
    fclose(kernel_out);
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
        "    -a                 Search the executable's immediate directory (instead of looking for data/)\n"
    };

    printf("%s", help);
}

/*
 * Get a path to the IO directory (or not, if immediate_directory search mode is enabled)
 * Params:
 *      file_name: name of the file that the path to is being generated
 *      immediate_directory: non-zero if immediate_directory search should be enabled
 * Returns:
 *      a pointer to a file path
 */
char *getPath(char *file_name, int immediate_directory) {
    // If we don't need to get the path to the IO directory we can just return the file_name
    if (immediate_directory == TRUE) {
        return file_name;
    }

    // Create the buffer and fill it with "../data/file_name"
    char *file_address = malloc(sizeof(char) * (strlen(file_name) + strlen(IO_DIRECTORY)));

    strcpy(file_address, IO_DIRECTORY);
    strcat(file_address, file_name);

    return file_address;
}

void process_kernel(FILE *kernel_in, FILE* kernel_out) {
    // Define strings that will be used
    const char string_prefix[] = { '"' };
    const char string_suffix[] = {' ', '\\', 'n', '"', '\n', '\0' };
    const char blank_line[] = { '"', ' ', '\\', 'n', '"', '\n', '\0' };

    // Create input buffer and initialize buffer size
    char *in_string_buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    size_t buffer_size;

    /*  Process the input file, line by line, placing the line in the buffer  */
    while (fgets(in_string_buffer, MAX_BUFFER_SIZE, kernel_in)) {
        // Identify string length
        buffer_size = get_length(in_string_buffer, MAX_BUFFER_SIZE);
        // Allocate a buffer for the output string of size prefix + input + suffix characters
        // The buffer is created and destroyed every iteration of the loop instead of being created once and
        // using realloc() because using realloc() was experimentally more prone to memory issues
        char *out_string_buffer = malloc(sizeof(char) * (buffer_size + strlen(string_prefix) + strlen(string_suffix)));

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

        // We construct the string manually instead of using strcpy() and strcat() as those
        // built-in methods result in more common issues with memory and garbage values
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
