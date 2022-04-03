//==================================================================-*-C++-*-=
// File:        gram.g
// Description: The parser of KTF (Knowledge Interchange Format).
//              See http://suo.ieee.org/suo-kif.html
//              The grammar is written for ANTLR parser generator.
// Part of:     
// Status:      
// Created:     Feb 13, 2002, Andrey Chaltsev, chaltsev@cs.man.ac.uk
// Doc:         http://suo.ieee.org/suo-kif.html
// Compliance:
// Reviewed:    
//              
// Revised:     
//              
// Note:        
//============================================================================


header 
{
  // head of lexer file
#include <iostream>
}

options { language="Cpp"; }

class MyLexer extends Lexer;
options
{ 
	k=3;
	charVocabulary = '\u0000'..'\u00FF'; // 8 bits character
  defaultErrorHandler=false;
}

// characters
 
protected
UPPER  : 
  'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' |
  'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'
;

protected
LOWER  :
  'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' |
  'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
;

protected
DIGIT  :
  '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
;

protected
SPECIAL  :
  '!' | '$' | '%' | '&' | '*' | '+' | '-' | '.' | '/' | '<' | '=' | '>' | '?' |
  '@' | '_' | '~' 
;

protected
WHITE  :
   ' '| '\t' | '\r' | '\n' {newline();} | '\f'
; // the same in skip-rule --- check it

protected
INITIALCHAR  :     UPPER | LOWER
;

protected
WORDCHAR  :        UPPER | LOWER | DIGIT | SPECIAL
;

protected
CHARACTER  :       UPPER | LOWER | DIGIT | SPECIAL | WHITE
;

// lexemes

WORD :            INITIALCHAR (WORDCHAR)*
;

STRING :          '"' ( (~('"' | '\\'| '\n')) 
                      | '\n' {newline();}
                      | ('\\' . ) 
                      )* 
                  '"'                             // ?????
;

VARIABLE :        '?' WORD
;

protected
FIGURE :  DIGIT (FIGURE)?
;

NUMBER :          ('-')? FIGURE ( '.' FIGURE )? (EXPONENT)?
;

protected
EXPONENT :        'e'  ('-')? FIGURE
;



// my character. 

// skip white characters.
SKIP_RULE :	  ( ' ' | '\t' | '\r' {newline();}| '\n'{newline();} | '\f' )
                          { _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
;

//commentaries are skipped as well. Commentary is a string beginning with ';'.
COMMENT_LINE:     ';'   (~('\n'))*
//                     ({LA(1)!=EOF_CHAR}? '\n' {newline();} )?
                           { _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
	;

// special character string (almost keywords)

LPAR:          '(';
RPAR:          ')';
ASSIGN:        '=';
LESS:          '<';
MORE:          '>';
LESSEQ:        "<=";
MOEWEQ:        ">=";
IMPLY:         "=>";
EQUALITY:      "<=>";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

{
  // head of parser file
  
  /// global output stream
  std::ostream* out;
}

//pointer on glabal table of names .
class MyParser extends Parser;
options 
{
  k=2;
  defaultErrorHandler=false;
}

// first rule to be invoked
start [std::ostream& s ] 
returns [bool b]
  {
    out = &s; // set output stream
  } 
 :
  ( sentence_plus )?                  { b = true; }
;

term : 
    v:VARIABLE                   { (*out) << v->getText() << ' '; }
  | w:WORD                       { (*out) << w->getText() << ' '; }
  | s:STRING                     { (*out) << s->getText() << ' '; }
  | funterm 
  | n:NUMBER                     { (*out) << n->getText() << ' '; }

;


relword : 
  w:WORD                         { (*out) << w->getText() << ' '; }
;

funword :
  w:WORD                         { (*out) << w->getText() << ' '; }
;

funterm :
  LPAR                            { (*out) << "( "; }
  funword
  term_plus
  RPAR                            { (*out) << ")"; }

;

sentence :
    w:WORD                       { (*out) << w->getText() << ' '; }
  | equation 
  | inequality 
  | relsent 
  | logsent 
  | quantsent
;


equation : 
  LPAR  
  "="                            { (*out) << "( = "; }
  term  term
  RPAR                            { (*out) << ")"; }
;

inequality : 
    LPAR  
    ">"                          { (*out) << "( > "; }
    term  term
    RPAR                          { (*out) << ")"; }

  | LPAR  
    "<"                          { (*out) << "( < "; }
    term  term
    RPAR                          { (*out) << ")"; }

  | LPAR  
    ">="                         { (*out) << "( >= "; }
    term  term
    RPAR                          { (*out) << ")"; }

  | LPAR  
    "<="                         { (*out) << "( <= "; }
    term  term
    RPAR                          { (*out) << ")"; }
;

relsent : 
  LPAR                            { (*out) << "( "; }
  relword  
  term_plus 
  RPAR                            { (*out) << ")"; }
;

logsent :
    LPAR  kw_not                  { (*out) << "( not "; }
    sentence     
    RPAR                          { (*out) << ")"; }

  | LPAR  kw_and                  { (*out) << "( and "; }
    sentence_plus  
    RPAR                          { (*out) << ")"; }

  | LPAR  kw_or                   { (*out) << "( or "; }
    sentence_plus
    RPAR                          { (*out) << ")"; }

  | LPAR  "=>"                    { (*out) << "( => "; }
    sentence  sentence  
    RPAR                          { (*out) << ")"; }

  | LPAR  "<=>"                   { (*out) << "( <=> "; }
    sentence sentence
    RPAR                          { (*out) << ")"; }
;

quantsent :
    LPAR  kw_forall  LPAR         { (*out) << "( forall ( "; }
    variable_plus
    RPAR                          { (*out) << ")"; }
    sentence  
    RPAR                          { (*out) << ")"; }

  | LPAR  kw_exists  LPAR         { (*out) << "( forall ( "; }
    variable_plus
    RPAR                          { (*out) << ")"; }
    sentence  
    RPAR                          { (*out) << ")"; }
;
//--------------  we avoid use of '+' => some additional rules

// (sentence)+
sentence_plus :
  sentence
  ( sentence_plus )?
;

// (term)+
term_plus :
  term
  ( term_plus )?
;

// (VARIBALE)+
variable_plus :
  v:VARIABLE                    { (*out) << v->getText() << ' '; } 
  ( variable_plus )?
;
//I do not know why but ANTLR does not recognise literal ("aaa") in lexer rules

//------------------   KEYWORDS  ---------------------------------------

kw_not       returns [int line]	: l: "not"      {line=l->getLine();};
kw_and       returns [int line]	: l: "and"      {line=l->getLine();};
kw_or        returns [int line]	: l: "or"       {line=l->getLine();};
kw_forall    returns [int line]	: l: "forall"   {line=l->getLine();};
kw_exists    returns [int line]	: l: "exists"   {line=l->getLine();};




