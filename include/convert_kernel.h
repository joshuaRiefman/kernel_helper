///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                           //
// File:        convert_kernel.h                                                                             //
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

#ifndef KERNEL_HELPER_CONVERT_KERNEL_H
#define KERNEL_HELPER_CONVERT_KERNEL_H

static void print_help();
static char *get_file_path(char *file_name, int immediate_directory);

#endif //KERNEL_HELPER_CONVERT_KERNEL_H
