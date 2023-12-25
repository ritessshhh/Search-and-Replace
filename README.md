# Text Manipulation Tool

## Overview

This project, is a comprehensive text manipulation tool. It implements functionality to search and replace text within files, supporting command-line argument parsing, file I/O operations, and optional wildcard search capabilities.

## Features

- **Command-Line Argument Parsing**: Process arguments including search text, replacement text, optional wildcard flag, and line ranges.
- **Text File Processing**: Open, read, and write text files to implement the search and replace functionality.
- **Search and Replace**: Search for specific text in the input file and replace it with provided text in the output file.
- **Wildcard Search**: Implement an optional wildcard search feature to extend the functionality of text search.

## Usage

The tool accepts various command-line arguments to specify the input and output files, text to search, replacement text, and additional options. Here's the general syntax:


### Options

- `-s`: Specify the text to search for in the input file. This option is required.
- `-r`: Specify the text that will replace the search text in the output file. This option is required.
- `-w`: Enable wildcard searching. This option is optional.
- `-l`: Specify the start and end lines to process in the input file. Lines outside this range are copied unmodified. This option is optional.

### Example Usage

1. **Basic Search and Replace**
```bash
./text_tool -s hello -r world input.txt output.txt
```
This replaces all occurrences of "hello" with "world" in `input.txt` and writes the result to `output.txt`.

2. **Wildcard Search**
```bash
./text_tool -s he* -r wo -w input.txt output.txt
```
Using the wildcard option, this replaces words starting with "he" with "wo".

3. **Line Range Specification**
```bash
./text_tool -s hello -r world -l 5 10 input.txt output.txt
```
Performs the replacement only on lines 5 to 10 of the input file.

## Implementation Details

- **Language**: The tool is implemented in C.
- **Libraries**: Utilizes standard C libraries for string processing and file I/O operations.
- **Error Handling**: Includes comprehensive error handling to manage different edge cases and input errors.

## Conclusion

This Text Manipulation Tool is a practical application of systems programming concepts, demonstrating file manipulation, command-line processing, and string operations in C.
