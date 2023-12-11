%option noyywrap
%option nounput
%option noinput

%{
#include <cstdlib>
#include <string>
#include <iostream>
#include "BaseAST.hpp"
#include "sysy.tab.hpp"
using namespace std;
%}

/* 空白符和注释 */
WhiteSpace    [ \t\n\r]*
LineComment   \/\/.*
BlockComment "/*"([^*]*|(\*+[^/]))*"*/"

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*
Btype "int"

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

/* 双字符运算符 */

%%
{WhiteSpace}    { /* 忽略, 不做任何操作 */ }
{LineComment}   { /* 忽略, 不做任何操作 */ }
{BlockComment}   { }

"return"        { return RETURN; }
"const"         { return CONST; }
{Btype}         { yylval.str_val = new string(yytext); return BTYPE; }


{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }


{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

.               { return yytext[0]; }

%%
