# TerasortingShared

This project implements both in-memory and external sorting techniques to handle datasets of varying sizes efficiently.

## Sorting Techniques

- **In-Memory Sorting**: Utilizes algorithms like Quick Sort and Merge Sort for datasets that fit entirely in RAM, offering fast sorting due to direct memory access.
- **External Sorting**: Used for large datasets that exceed RAM capacity, involving disk I/O operations. It sorts data in chunks and merges them, using algorithms like External Merge Sort.

## How to Run

1. Ensure all necessary files are in the directory.
2. Use the `make` command to compile the C code.
3. Execute the sorting with: `./mysort 64Gb.txt`
   - Automatically handles log generation and cleanup.
   - Files <8GB use in-memory sorting; larger files use external sorting.

## System Requirements

- **OS**: Ubuntu 22.04.3 LTS (Jammy)
- **Minimum Specs**:
  - CPU: Intel Core i5-10500H @ 2.50GHz
  - Architecture: x86_64
  - RAM: Sufficient to handle in-memory sorting for smaller datasets

## Notes

- Log files are generated post-execution.
- `Valsort` and `gensort` tools are included in the directory.
- Use specified commands in the report to generate `Linsort` logs.

For more details, visit the [GitHub repository](https://github.com/rakeshreddy06/terasortingShared).
