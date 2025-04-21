
# Lexical Analyzer in C

**Lexical analyzer simplified** written in C that processes an input file and identifies various types of tokens: keywords, identifiers, numeric constants, delimiters, operators, and string literals. It also handles basic errors related to invalid or incomplete tokens.

## Features
This file contains the implementation of the lexical analyzer. The program uses specific functions to recognize and process different token types in an input file. Key features include:

- **Input File Reading:** The `readFile` function reads the content of a file and stores it in memory for analysis.
- **Token Detection:** The `analyzer` function processes the content and categorizes tokens into the following types:
  - **Keywords:** `if`, `else`, `while`, `return`, etc.
  - **Identifiers:** Valid variables following specific rules.
  - **Numeric Constants:** Includes integers, floats, and scientific notation.
  - **Delimiters:** `,`, `;`, `{`, `}`, `(`, `)`, etc.
  - **Operators:** `+`, `-`, `*`, `/`, `&&`, `||`, etc.
  - **String Literals:** Strings enclosed in double quotes (`"example"`).
- **Validation Using Deterministic Finite Automata (DFA):**
  - DFA for numbers (`statenumb`).
  - DFA for identifiers and variables (`stateVar`).
- **Error Detection:** The program identifies and displays error messages for invalid or improperly formatted tokens.

## Getting Started

Follow these simple steps to install, compile, and run the program.

### 1. Prerequisites

- A Linux-based operating system (tested on Linux Mint).
- GCC (GNU Compiler Collection) installed on your system.


### 2. Compiling the Code

Use the following command to compile the code:

```bash
gcc analyzer.c  -o  analyzer
```

### 3. Running the Program

Execute the program with an input file:

```bash
./analyzer prueba.c
```

## Examples

Here are some examples of input and the corresponding output from the lexical analyzer:

### Example 1: Simple Input
#### Input File (`example1.c`):
```c
int main() {
    int a = 5;
    float b = 10.25;
    if (a < b) {
        printf("a is less than b\n");
    }
    return 0;
}
```
#### Output is:

```bash
The file is: 

int main() {
    int a = 5;
    float b = 10.25;
    if (a < b) {
        printf("a is less than b\n");
    }
    return 0;
}

The tokens in the file: 

TOKEN (INT): int
TOKEN (MAIN): main
TOKEN (LPAR): (
TOKEN (RPAR): )
TOKEN (LBRACE): {
TOKEN (INT): int
TOKEN (ID): a
TOKEN (ASSIGN): =
TOKEN (NUMBER): 5
TOKEN (SEMI): ;
TOKEN (FLOAT): float
TOKEN (ID): b
TOKEN (ASSIGN): =
TOKEN (NUMBER): 10.25
TOKEN (SEMI): ;
TOKEN (IF): if
TOKEN (LPAR): (
TOKEN (ID): a
TOKEN (LT): <
TOKEN (ID): b
TOKEN (RPAR): )
TOKEN (LBRACE): {
TOKEN (PRINTF): printf
TOKEN (LPAR): (
TOKEN (STRING): "a is less than b\n" 
TOKEN (RPAR): )
TOKEN (SEMI): ;
TOKEN (RBRACE): }
TOKEN (RETURN): return
TOKEN (NUMBER): 0
TOKEN (SEMI): ;
TOKEN (RBRACE): }
```
##How It Works  

The lexical analyzer operates in the following steps:  

1. **File Reading**  
   The program reads the input file line by line using the `readFile` function, storing the content in memory. This enables sequential token processing.  

2. **Tokenization**  
   The `analyzer` function scans the file content character by character to identify token boundaries. It uses specific rules to classify tokens, such as checking for keywords, operators, delimiters, and numeric constants.  

3. **Token Validation**  
   Each identified token is validated using helper functions:
   - **Keywords:** Compared against a predefined list (`if`, `else`, etc.).  
   - **Identifiers:** Verified to start with a letter or underscore, followed by letters, digits, or underscores.
   - **Operators:** Classified based on common C operators (`+`, `-`, `*`, `=`, etc.).
   - **Delimiters:** Identified based on common syntax delimiters (`;`, `,`, `(`, `)`, etc.).
   - **Constants:** Identified based on numeric formats for integers or floats.

4. **Output**  
   Once tokens are recognized, they are output with their corresponding types and values. Invalid tokens are flagged with error messages.
