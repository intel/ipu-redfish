# Coding style for C/C++

## Introduction

### Motivation

The purpose of this document is to specify the coding standards used within
Intel RackScale project for C/C++ development. It covers the following issues:
* source code formatting
* commenting and documenting source code
* best known methods (BKMs) for source code development

### Rationale

C and C++ are free-form languages which allow for a wide variety of programming
styles and conventions to be used. Combining it with the fact that throughout
the project lifetime the code is developed by tens of programmers leads to
a possible threat of the code being difficult to develop, read and maintain.
A lot of issues regarding coding style such as location of brackets or naming
conventions can spark a fierce debate among developers. The purpose of this
document is not to argue which method is better - its sole purpose is to
establish a consistent coding standard, as it is this consistency which leads
to greater productivity.

Some may find the rules overly restrictive at times - be assured that they are
there for a reason and common good. Each rule presented will be accompanied with
a rationale standing behind it. When in doubt, please refer that
a self-consistent set of rules will let you think less about coding style and
more on problem solving.

### Goals

The goal of this document is to increase quality of the final product by
improving the following key areas:
* complexity - designing highly complex systems results in poor code
  readability, understanding and maintainability
* maintainability - the design should be self explanatory to reduce time, effort
  and cost of unavoidable future maintaince of the code
* loose coupling - decreasing the number of software interdependencies results
  in simpler and easier to understand code which can be reused more easily in the
  future
* reusability - designing abstract modules enables them to be used later without
  much modifications which saves time and costs
* extensibility
* portability
* performance
* standarization

### Document structure

Each section regards a different topic; each topic is divided into rules. Each
rule have the following structure:

  \<rule number\>. \<brief description\> (\<descriptor\>)

  \<detailed description\>

  Rationale: \<rationale\>

  Correct use: \<examplary code\>

## Naming conventions

### General rules

1. All names should be unambiguous (mandatory)
2. Names should not contain abbreviations other than standard industry concepts
   (mandatory)
3. Names should be kept short
4. Names must not begin with and underscore ('`_`')
5. Names must not use double underscores ('`__`')
6. Avoid name overloading

### Files

1. Header file extension for C language is `.h`
2. Header file extension for C++ language is `.hpp`
3. Source file extension for C language is `.c`
4. Source file extension for C++ language is `.cpp`
5. Configuration header files should have additional `.in` extension
6. Header name shall match the name of the source file containing its implementation
7. File names should not contain reference to the top-level product being implemented
8. Use only lower-case characters for file names
9. Separate words with single underscore character ('`_`')
10. Files must be named after classes they contain

### Variables

1. Non-const Variable name which are not constants must consist only of lower-case characters, numbers or underscores
2. Const variable name must consist only of upper-case characters, numbers or underscores
3. Separate words with underscore ('`_`')
4. Variable name must pertain to the data is stores
5. Global variables must begin with a prefix `g_`
6. Global pointer variables must begin with a prefix `gp_`

### Functions

1. Function name must consist only of lower-case characters, numbers or underscores
2. Separate words with underscore ('`_`')

### Classes

1. Class name must consists only of upper or lower-case characters or numbers
2. Class names must be written in CamelCase
3. Every word  must start with a capital letter
4. If an abbreviation appears in the class name, only first letter is capitalized
5. Use `Interface` suffix to indicate abstract classes

#### Class members

1. Class member name must begin with `m_` prefix
2. Class member pointer names should begin with `mp_` prefix

#### Class methods

1. Class method name must consist only of lower-case characters, numbers or underscores
2. Separate words with underscore ('`_`')
3. Use standard prefixes (`get_`, `set_`, `add_`, `remove_`) for accessors and mutators

### Structs

All rules regarding class naming applies also to struct

### Enums

1. Enum name must consists only of upper or lower-case characters or numbers
2. Enum name must be written in CamelCase
3. Enum literals should consist only of upper-case characters
4. Words in enum literals must be separated with an underscore ('`_`')

### Preprocessor macros

1. Macro name must consist only of upper-case characters
2. Words in macro name must be separated with an underscore ('`_`')

### Namespaces

1. Namespace name must consist only of lower-case characters and numbers

### Templates

1. Function templates must be named according to rules applying to function naming
2. Class templates must be named according to rules applying to class naming

## Code documentation

1. Comments should contain meaningful information
2. Do not state the obvious
3. Doxygen comments must be used
4. Use 'at' character ('`@`') for special Doxygen commands
5. One-line Doxygen comments must have the form `/*! <comment> */`
6. Doxygen block comments must adhere to the standard
7. Always use Doxygen comments to document files
8. Always use Doxygen comments to document classes
9. Always use Doxygen comments to document class methods
10. Always use Doxygen comments to document functions
11. Always use Doxygen comments to document templates
12. Always use Doxygen comments to document enums
13. Comments shall match the indentation level of the code it documents
14. Always use `@param[in]` and `@param[out]` to document function parameters
15. Never use ASCII art comments
16. Use `@TODO` syntax to indicate tasks that need to be performed

## Formatting rules

### General rules

1. Use spaces for indentation
2. Indentation level must be equal to 4 spaces
3. No trailing spaces are allowed
4. Blank lines shall be used to separate logically connected sections of code
5. Blank lines must not contain white characters
6. A single line of code shall not exceed 128 characters
7. Expressions extending more than one line shall be aligned to the next indentation level with respect to the indentation level of the beginning of the expression
8. Excluding loops, each line of code must contain at most one statement.
9. Each comma ('`,`') must be proceeded with one space

### Types

1. Type modifiers must be placed to the left of the variable type
2. While declaring a reference type, ampersand sign ('`&`') must be placed next to the reference type and be separated from the reference name with a space.
3. While declaring a pointer type, star sign ('`*`') must be placed next to the pointer type and be separated from the pointer name with a space.

### Functions

1. Do not separate function name with parentheses with a space
2. Do not use void-casting of unused parameters where possible
3. In function definitions the opening curly bracket should be placed in the same line as the closing bracket of the parameter list.
4. Indentation level of the curly bracket closing function body should match that of function return type.
5. Prefer single-task functions
6. Function body should not exceed one page of code
7. Do not use `inline` functions

### Classes

1. Always use the following order of class access modifiers: public, protected, private
2. Never mix class access modifiers
3. Class members should be accessed only via accessor functions
4. Indentation level of access modifiers should match that of the `class` keyword
5. Indentation level of class methods and members should be one level higher than that of the `class` keyword
6. Use spaces on both sides of the the colon sign ('`:`') while declaring class inheritance
7. Always indicate inheritance type by using appropriate keyword
8. Place method definition inside class definition only for one line long methods

### Function and class templates

1. `template` keyword must be placed above the template name
2. Template parameters brackets ('`<...>`') must not be separated from the `template` keyword
3. All rules applying to functions or classes apply for templates
4. While defining class templates, put template method definition outside the class definition

### `if` statement

1. The `if` keyword must be separated from the condition parentheses with one space
2. Conditional code must always be placed in curly brackets with the only exception being else-if statements
3. The opening curly bracket must be placed in the same line as the end of condition
4. Opening curly bracket must be separated from the condition bracket with one space
5. The indentation level of the closing bracket must match that of the statement it belongs to.

### `switch` statement

1. The `switch` keyword must be separated from the condition parentheses with one space
2. Default case must always be present
3. Each fall-through case must be commented
4. Indentation level of each case code must be one level higher than that of the `switch` keyword
5. Always add `default` clause with assertion

### Loop statements

1. The condition parenthesis must be separated from the loop keyword with one space
2. Curly brackets must always be used to enclose loop body
3. The loop body opening curly bracket must be placed in the same line as the end of condition
4. The opening curly bracket must be separated from the condition with one space
5. Closing curly bracket indentation level must match that of the loop keyword

### Operators

1. A unary operator should not be separated from the operand
2. Binary operators must be separated from the operands with single spaces
3. Ternary operator must separated from the operands with single spaces

### Boolean expressions

1. Do not put spaces between the conditional statement and the enclosing brackets
2. Use brackets to improve readability of the statement
3. Use newlines to separate subexpressions connected with OR or AND operators
4. Never compare expression with true/false value

### Namespaces

1. Do not use indentation inside namespaces
2. Never use `using namespace` in header files
3. Prefer using `using namespace` in CPP files
4. Do not import whole namespaces if not necessary, use concrete classes instead
5. Use anonymous namespaces to hide implementation detais (local functions, classes, constants)

## Code complexity management

1. Use layering
2. Do not write the same code twice
3. Avoid nested loops if possible
4. Avoid `goto` statements whenever possible

## Exception handling

1. Use exceptions to indicate error conditions
2. If using C functions inside C++ code, convert error return codes to exceptions
3. Never throw exceptions from object destructors
4. Make sure each exception is caught at some point
5. Catch exceptions by const reference
6. Never throw object created with `new` operator

## File conventions

### General rules

1. Follow Doxygen C++ style of comments
2. Always place appropriate license section at the top of the file
3. The order of including headers must be the following: local headers, third party headers, standard headers

### Header files conventions

1. All header files must include a header guard
2. Header guard must be separated with blank lines from the license/comment section and the rest of the header file
3. Use `@# pragma once` as header guards
4. Include other headers only after the include guard
5. Move as much `@# include` macros to the source file as possible

### Source files conventions

1. Each class implementation should be placed in a separate file
2. Internal small classes might be kept together with implementation

## Miscellaneous

1. Prefer using stream instead of `stdio` functions
2. Never use magic numbers and hardcoded literals
3. Use `constexpr` statements instead of preprocessor macros where possible
4. If using preprocessor macro cannot be avoided, place the macro definition at the top of the file
5. Do not use comments to remove dead code, if necessary use `@#ifdef` clause
6. Do not overuse `auto` keyword
7. Avoid unnecessary use of lambda functions
8. Use C++11 alias definition with `using` keyword
9. Never use `using` alias definition outside the class
10. Put general class alias definition in front of the class
11. Keep private alias definition close to the private members or methods.
