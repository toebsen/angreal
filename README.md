
# Readme

## Setup
```console
$ pacman -Syuu
$ pacman -S mingw-w64-x86_64-toolchain 
$ pacman -Sy mingw-w64-i686-boost mingw-w64-x86_64-boost
```

## TBLang
Small interpreted pet language named *TBLang*!

Inspired by https://drmenguin.com/minilang-interpreter/

Regular BNF syntax of *TBLang*:
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

variable_decl ::= 'var' identifier ':' type '=' expression
assignment ::= 'set' identifier '=' expression

print_statement ::= 'print' '(' expression ')'
if_statement ::= 'if' '('expression')'block['else' 'if' expression' block]['else'block]
while_statement ::= 'while' '('expression')'block
return_statement ::= 'return expression';'

function_decl ::= 'def' identifier '('[formal_params]')' : type block
formal_params ::=  formal_param (',' formal_param)*
formal_param ::= type ':' identifier 

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


## Current state:

| Token  | Lexer  | Parser  | Environment  | -  |
|---|---|---|---|---|
|program | :heavy_check_mark: | :heavy_check_mark: |  |
|block | :heavy_check_mark: | :heavy_check_mark: |  |
|statement | :heavy_check_mark: | :heavy_check_mark: |  |                                  
|variable_decl | :heavy_check_mark: | :heavy_check_mark: |  |
|assignment |:heavy_check_mark: |:heavy_check_mark:  |  |
|print_statement |:heavy_check_mark: | :x: |  |
|if_statement |:heavy_check_mark: |:x:  |  |
|while_statement | :heavy_check_mark:| :x: |  |
|return_statement | :heavy_check_mark:| :x: |  |
|function_decl | :heavy_check_mark:| :heavy_check_mark: |  |
|formal_params |:heavy_check_mark: | :heavy_check_mark: |  |
|formal_param |:heavy_check_mark: | :heavy_check_mark: |  | 
|function_call | :heavy_check_mark:|:heavy_check_mark: |  |
|actual_params |:heavy_check_mark: | :heavy_check_mark: |  |
|expression | :heavy_check_mark:| :heavy_check_mark: |  |
|simple_expression | :heavy_check_mark:| :heavy_check_mark: |  |
|term | :heavy_check_mark:| :heavy_check_mark: |  |
|factor | :heavy_check_mark:|:heavy_check_mark:  |  |
|sub_expression |:heavy_check_mark: |:heavy_check_mark: |  | 
|unary-op | :heavy_check_mark:| :heavy_check_mark: |  | 
|comp-op |:heavy_check_mark: |  :heavy_check_mark:|  |
|additive_op | :heavy_check_mark:| :heavy_check_mark: |  | 
|multiplicative_op | :heavy_check_mark:|:heavy_check_mark: |  |
|type | :heavy_check_mark:|  :heavy_check_mark:|  |
|identifier |:heavy_check_mark: |:heavy_check_mark:  |  |
|literal | :heavy_check_mark:| :heavy_check_mark: |  | 
|float_literal |:heavy_check_mark: |  :heavy_check_mark:|  |
|fraction_literal |:heavy_check_mark: | :heavy_check_mark: |  |
|int_literal |:heavy_check_mark: |:heavy_check_mark:  |  |
|bool_literal |:heavy_check_mark: | :heavy_check_mark: |  |
|string_literal |:heavy_check_mark: | :heavy_check_mark: |  |
|printable |:heavy_check_mark: |:heavy_check_mark:  |  |
|letter |:heavy_check_mark: |:heavy_check_mark:  |  |
|digit |:heavy_check_mark: | :heavy_check_mark: |  |
