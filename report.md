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
