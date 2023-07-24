__kernel void example(
   __global float* output_buffer)
{
   int i = get_global_id(0);
   output_buffer[i] = i;
}
