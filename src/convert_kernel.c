///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                           //
// File:        convert_kernel.c                                                                             //
//                                                                                                           //
// Abstract:    This is a helper program to convert a *.cl kernel file                                       //
//              into a format that is able to be pasted into a char[]                                        //
//              within a C++ program to be then built as a OpenCL kernel.                                    //
//                                                                                                           //
// Version:     <1.0>                                                                                        //
//                                                                                                           //
// Usage:       Build the kernel_helper EXECUTABLE to preprocess OpenCL kernels.                             //
//                                                                                                           //
// Note:        Kernel *.cl file must be in the same directory/folder as the program executable.             //
//              Expects a blank line at the end of file, else UNDEFINED BEHAVIOUR WILL OCCUR.                //
//              The above is a known issue, which should be fixed in a future version.                       //
//              Use -h or --help to print help message for usage instructions.                               //
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
#include "../include/convert_kernel.h"

////////////////////////////////////////////////  MAIN  ///////////////////////////////////////////////////////

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
            use_immediate_directory = TRUE;
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
    char *in_address = get_file_path(kernel_filename != NULL ? kernel_filename : DEFAULT_KERNEL_FILE,
                                     use_immediate_directory);
    kernel_in = fopen(in_address, "r");
    if (kernel_in == NULL) {
        printf("Fatal Error: Kernel was not found!\n");
        return 1;
    }

    char *out_address = get_file_path(output_filename != NULL ? output_filename : DEFAULT_OUTPUT_FILE,
                                      use_immediate_directory);
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
 * Print a help message detailing usage, options, and flags.
 */
static void print_help() {
    char help[] = {
            "usage: kernel_helper [options]\n"
            "  example: kernel_helper -f kernel.cl -o out.txt -v\n"
            "  options:\n"
            "    -h, --help         Display help\n"
            "    -v                 Activate verbose mode\n"
            "    -b                 Enable output of blank lines\n"
            "    -f [kernel_file]   Indicate kernel file for the program to process\n"
            "    -o [output_file]   Indicate an output file for the program\n"
            "    -a                 Search executable's immediate directory (instead of looking for data/)\n"
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
static char *get_file_path(char *file_name, int immediate_directory) {
    // If we don't need to get the path to the IO directory we can just return the file_name
    if (immediate_directory) {
        return file_name;
    }

    // Create the buffer and fill it with "../data/file_name"
    char *file_address = malloc(sizeof(char) * (strlen(file_name) + strlen(IO_DIRECTORY)));

    strcpy(file_address, IO_DIRECTORY);
    strcat(file_address, file_name);

    return file_address;
}
