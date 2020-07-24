# regex

Regular expression engine implementation in C++ that supports
concatenation, union, and closure operations. Constructs NFA from a
regular expression.

Example usage:
```cpp
Please enter regular expression
aapl(e|s)*
Please enter the full path of the input file: /Users/andrewchang/Desktop/cs_projects/regex/regex/input_file.txt
Found match for: aapl(e|s)*
```

Files Included:

1. main.cpp: driver program for accepting regular
expression from user and path to input file.

2. nfa.cpp: class for construction NFA from regular expression

3. formatter.cpp: utility functions for converting regular expressions
into postfix notation for NFA construction
