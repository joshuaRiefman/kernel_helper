# Interfaces

Kernel Helper has two public interfaces that can be used to convert OpenCL kernels at runtime.

| Interface        | Purpose                                   |
|------------------|-------------------------------------------|
| `load_kernel`    | Return a kernel converted into a string   |
| `process_kernel` | Format a kernel to a paste-able text file |

### `load_kernel`

| Argument : Type         | Purpose                                            |
|-------------------------|----------------------------------------------------|
| `kernel`: `FILE`        | Opened and readable `FILE` that will be converted. |
| `kernel_size`: `size_t` | Size of the kernel file in bytes.                  |

`load_kernel` returns a `char*` to the generated string. It can be cast to `const`
and directly used as the source argument to `clCreateProgramWithSource()`.

### `process_kernel`
| Argument : Type      | Description                                                  |
|----------------------|--------------------------------------------------------------|
| `kernel`: `FILE`     | Opened and readable `FILE` that will be converted.           |
| `kernel_out`: `FILE` | Opened and writable `FILE` where the output will be written. |

`process_kernel` takes in an input and output file and formats the input per the specifications
required to paste the output into a C++ program such that it can paste as a field to a `const char*`
then provided to `clCreateProgramWithSource()`.
