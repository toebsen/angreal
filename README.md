
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