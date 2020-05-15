
![test-clang-format](https://github.com/toebsen/angreal/workflows/test-clang-format/badge.svg?branch=master)

# Readme

## Angreal

Small interpreted *python*-like toy language named **Angreal**.

```python
# this is a comment
def Hello(name) {
    # this is a function
    return "Hello " + name
}

print(Hello("World"));
```

> Created for learning purposes only.

## Name

The name is taken from the high fantasy series *Wheel Of Time* novels by Robert Jordan.

> Angreal (ahn-gree-AHL) are objects which allow those who can channel to draw more of the One Power 
> than they normally could without harming themselves in the process.
>
> [Wheel Of Time wiki](https://wot.fandom.com/wiki/Angreal)


### Current Features:

- dynamic typing 
- automatic memory management using reference counting
- functions are first class objects 

### Planned Features:

- object orientation
- move from **tree walk** to a **VM** interpreter

### Inspired by:

- the awesome [craftinginterpreters](https://www.craftinginterpreters.com/contents.html) :heart:
- and the great report of [minilang-interpreter](https://drmenguin.com/minilang-interpreter/)

## Setup

Currently developed on windows with msys2 using:
- cmake
- ninja
- clang-tidy
- boost
- [magic_enum](https://github.com/Neargye/magic_enum) 

### MSys2 on Windows
```bash
$ pacman -Syuu
$ pacman -Sy mingw-w64-x86_64-toolchain 
$ pacman -Sy mingw-w64-i686-boost mingw-w64-x86_64-boost
```

For building with Ninja:

```
$ pacman -Sy mingw-w64-x86_64-ninja
```

### Ubuntu 18.04

#### Install Cmake (>= 3.11) for older distributions 
Check cmake version:

``$ cmake --version``

Remove old version (<3.11) of cmake

``$ sudo apt purge --auto-remove cmake``

Obtain a copy of the signing key

``$ wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -``

Add the repository to your sources list
a. For Ubuntu Bionic Beaver (18.04)

``$ sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'``

b. For Ubuntu Xenial Xerus (16.04)

``$ sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ xenial main'``

Update and install
```
$ sudo apt update
$ sudo apt install cmake
```

#### Build via g++
Install gcc-9

```bash
$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
$ sudo apt update
$ sudo apt install gcc-9 g++-9 libboost-dev
```
 
To make gcc-9 the default for the future:

```
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
```

#### Build via clang

Install clang-10

```bash
$ wget https://apt.llvm.org/llvm.sh
$ chmod +x llvm.sh
$ sudo ./llvm.sh 10
```

To build:

```bash
$ mkdir build && cd build
$ CXX=clang++-10 CC=clang-10 cmake ..
$ make -j 2
```
## Project Layout

```
+---.cmake 
+---src 
|   +---common
|   +---interpreter
|   |   +---analysis
|   |   |   \---semantic
|   |   \---environment
|   +---lexer
|   \---parser
+---tests
|   +---acceptance
|   +---interpreter
|   +---lexer
|   \---parser
\---third_party
    \---magic_enum
```

## Syntax

Regular BNF syntax of *Angreal*:
- [syntax](http://matt.might.net/articles/grammars-bnf-ebnf/)

```
program ::= {statement}

block ::= '{'{statement}'}'

statement ::= variable_decl';' |                 
                assignement';' | 
                print_statement';' | 
                if_statement | 
                while_statement | 
                return_statement ';' | 
                function_decl | 
                block 

variable_decl ::= 'var' identifier '=' expression
assignment ::= 'set' identifier '=' expression

print_statement ::= 'print' '(' expression ')'
if_statement ::= 'if' '('expression')'block['else'block]
while_statement ::= 'while' '('expression')'block
return_statement ::= 'return expression';'

function_decl ::= 'def' identifier '('[formal_params]')' : type block
formal_params ::=  identifier (',' identifier)*


function_call ::= identifier'(' actual_params ')'
actual_params ::= expression (',' expression)*

expression ::= simple_expression (comp-op simple_expression)*
simple_expression ::= term (additive_op term)*
term ::= factor (multiplicative_op factor)*

factor ::= literal | identifier | function_call | sub_expression | unary-op
 
sub_expression ::= '(' expression ')' 

unary-op ::= ('+'|'-'|'!') expression+ 
comp-op ::= ''| ''| '='| '='| '=='| '!='

additive_op ::= '+' | '-' | 'and' 
multiplicative_op ::= '*' | '/' | 'or'

type ::= 'int' | 'bool' | 'float' | 'string'
identifier ::= '_'?|letter*|digit*}

literal ::= bool_literal | int_literal | string_literal | float_literal 
float_literal ::= [int_literal]fraction_literal
fraction_literal ::= '.'digit*
int_literal ::= digit+
bool_literal ::= 'true' | 'false'
string_literal ::= '"'printable+'"'

printable ::= digit* | letter+
letter ::= [a-zA-Z]
digit ::= [0-9]

```
