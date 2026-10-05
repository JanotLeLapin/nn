# Report

Currently (commith hash `1163d8933c3c4d37d944fc96413c7d9589e16956`, 2026-10-05) the
inference engine works, but it's somewhat buggy and unsafe. Because of the
recent massive architectural change (every block now points to an offset in a
big float buffer) that was definitely NOT thought through, there's now a wide
margin for improvement, performance-wise. I'll document updates alongside
benchmarks in this report. For now I'll just keep track of some stuff that I'm
considering to improve the efficiency of the inference loop:

1. Kernels take `block_t` *pointers*, marked with `restrict`. But the
`block_t` type is just 16-bytes, so it might be more efficient to just copy
instead.
2. The `GEMM` instruction is naive (striding), I can improve cache locality
here.
3. Still regarding `GEMM`, I could try to use OpenMP and make the kernel
trivially parallel, but given the relatively small size of the neural network
it might take more time to synchronize threads than to sequentially run the
kernel. Same idea applies to rewriting `GEMM` in OpenCL.
4. I'd also like to experiment with the `-ffast-math` compiler flag.

That's about all I can think of for now

## Baseline

- compiler: `gcc`, version: `15.3.0`
- flags: `-fuse-ld=mold -g -Wall -O3`
- CPU: `AMD Ryzen 7 5800X 8-Core Processor`
- OS: `NixOS 26.11`, Linux: `7.2.6`

I've recorded the performance of the inference loop for the following network:

```c
nn_layer_input (&net, 784);
nn_layer_dense (&net, 784, 128, 0, 0, ACT_TYPE_RELU); // 100,352 muls
nn_layer_dense (&net, 128, 64, 0, 0, ACT_TYPE_RELU); // 8.192 muls
nn_layer_dense (&net, 64, 10, 0, 0, ACT_TYPE_SOFTMAX); // 640 muls
// TOTAL: 109,184 muls
```

It consistently took between `0.125` and `0.185` milliseconds, that's roughly
`727,893,333` floating-point ops per second (~**728 MFLOPS**/s)
