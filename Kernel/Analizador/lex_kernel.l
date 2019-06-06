%option noyywrap
%{
   #include "yacc_kernel.tab.h"
   #include <stdlib.h>
   #include <stdio.h>
%}

DIGITO [0-9]
DIGITO_POSITIVO [1-9]
LETRA_MAYUSCULA [A-Z]
LETRA_MINUSCULA [a-z]
LETRA_T [a-zA-Z]
LETRA_NUM [a-zA-Z0-9]
ESPACIO [ \t]+
GUION_BAJO [_]

%%


{ESPACIO} {}
"(" {return PARENTESIS_IZQ;}
")" {return PARENTESIS_DER;}
"," {return COMA;}
";" {return PUNTO_Y_COMA;}
{GUION_BAJO} {return GUION_BAJO;}
"CREATE" {return CREATE;}
"DESCRIBE" {return DESCRIBE;}
"SELECT" {return SELECT;}
"DROP" {return DROP;}
"JOURNAL" {return JOURNAL;}
"INSERT" {return INSERT;}
"METRICS" {return METRICS;}
"SC" {return CONSISTENCIA_SC;}
"SHC" {return CONSISTENCIA_SHC;}
"EC" {return CONSISTENCIA_EC;}
"Tabla" {return NOMBRE_TABLA}


{DIGITO}+ {return KEY;}
{DIGITO_POSITIVO} {return NUMERO_POSITIVO;}
{DIGITO_POSITIVO}{DIGITO}* {return TIMESTAMP;}
{DIGITO_POSITIVO}{DIGITO}* {return COMPACTACION;}
{LETRA_T}+ {return VALUE;}
{NOMBRE_TABLA}{GUION_BAJO}{LETRA_MAYUSCULA}+ {return NOMBRE_TABLA;}

%%




