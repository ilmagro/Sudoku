# Summary
The goal of this repository is to implement a Sudoku solver in C.
The strategy used is [**recursion** with **backtracking** and **cutoff**](sudoku.c#L185-190)
if the user does not want to explore further solutions.

The program reads from an input file structured like [`sudoku.txt`](sudoku.txt).
Extra spaces and extra tabs are properly handled.
Malformed input (e.g. less/extra columns, less/extra rows, numbers not in the [1-9] range)
is properly handled. User can insert [.0Oo] for unknowns.

## Usage
Usage: <program\_name> <input_file>

## Requirements
- Make
- a C compiler (like GCC)

Note: a POSIX-compatible system is not required.

## Further developments
A system formed by a segmentation layer plus an OCR will be used to create a sudoku template
from an image or a camera device.

## Acknowledgements
The algorithm takes inspiration from the famous video
(Python Sudoku Solver - Computerphile)[https://www.youtube.com/watch?v=G_UYXzGuqvM]
by (Dr. Thorsten Altenkirch)[https://en.wikipedia.org/wiki/Thorsten_Altenkirch].
