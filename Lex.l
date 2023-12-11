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

/* �հ׷���ע�� */
WhiteSpace    [ \t\n\r]*
LineComment   \/\/.*
BlockComment "/*"([^*]*|(\*+[^/]))*"*/"

/* ��ʶ�� */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*
Btype "int"

/* ���������� */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

/* ˫�ַ������ */

%%
{WhiteSpace}    { /* ����, �����κβ��� */ }
{LineComment}   { /* ����, �����κβ��� */ }
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