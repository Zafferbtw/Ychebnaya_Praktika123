# technical design

## goals
- dense matrices up to 100x100
- operations: add, sub, scalar mul, matmul, transpose, trace, determinant (small sizes), lu solver for ax=b
- console app reads matrices from a text file and writes results to console + file

## data layout
- row-major contiguous buffer: `data[r*cols + c]`
- element type: `float`

## algorithms
- matmul:
  - naive `i-k-j` loop order for cache friendliness
  - avx2 path uses `_mm256_fmadd_ps` and processes 8 floats per step (when `n % 8 == 0`)
- determinant:
  - gaussian elimination with partial pivoting
  - restriction: `n <= 10`
- lu solver:
  - lu decomposition with partial pivoting (in-place lu + pivot vector)
  - forward/back substitution

## limitations
- simd path expects square matrices, sizes divisible by 8 for full avx throughput (fallback exists)
- determinant is intended for small sizes only
