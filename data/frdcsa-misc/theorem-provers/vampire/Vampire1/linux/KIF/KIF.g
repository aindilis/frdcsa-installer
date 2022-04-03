//==================================================================-*-C++-*-=
// File:        KIF.g
// Description: The parser of KTF (Knowledge Interchange Format).
//              See http://suo.ieee.org/suo-kif.html
//              The grammar is written for ANTLR parser generator.
// Part of:     
// Status:      
// Created:     Jul 09, 2002, Andrey Chaltsev, chaltsev@cs.man.ac.uk
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
#include "typedefs.h"
#include "term.h"
#include "formula.h"


class Atom;
class Formula;
}


options { language="Cpp"; }


class KIFLexer extends Lexer;
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
   ' '| '\t' | '\r' | ('\n' {newline();}) | '\f'
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
                      | ( '\n' {newline();} )
                      | ('\\' . ) 
                      )* 
                  '"'                             // ?????
;

VARIABLE : '?' WORD
;

protected
FIGURE :  DIGIT (FIGURE)?
;

NUMBER :          ('-')? FIGURE ( '.' FIGURE )? (EXPONENT)?
;

protected
EXPONENT :        'e'  ('-')? FIGURE
;



// skip white characters.
SKIP_RULE :	  ( ' ' | '\t' | '\r' | ( '\n'{newline();}) | '\f' )
                          { _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
; // I am not sure about newline and '\r'

//commentaries are skipped as well. Commentary is a string beginning with ';'.
COMMENT_LINE:     ';'   (~('\n'))*
                           { _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
	;

// special character string (almost keywords)

LPAR:          '(';
RPAR:          ')';
ASSIGN:        '=';
LESS:          '<';
MORE:          '>';
LESSEQ:        "<=";
GEQ:           ">=";
IMPLY:         "=>";
EQUALITY:      "<=>";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

{
  // head of parser file
#include "literal.h"
#include "term.h"
#include "clause.h"
#include "formula.h"
#include "int_name_table.h"
#include "signature.h"
#include "problem.h"
#include "atom.h"

}


class KIFParser extends Parser;
options 
{
  k=2;
  defaultErrorHandler=false;
}

// it is added into body of class KIFParser
{
}


// first rule to be invoked
start  
returns [Formula::List* formulas]
        {
	  Formula* f;
	  Formula::List* fs = Formula::List::empty ();
	}
  :
  f = sentence
  (
    fs = start
    |
  )
        {
	  formulas = new Formula::List (f, fs);
	}
;

term 
  returns [Term* t]
        {
	  Var v;
	}
  :
    v = variable // variable
        {
	  t = new Term (v);
	}
  | 
    c : WORD  // constant
        {
	  t = new Term ( sig->createFun (c->getText().c_str(), 0), 
                         Term::List::empty () );
	}
  | 
    s:STRING // string constant
        { 
 	  t = new Term ( sig->createFun (s->getText().c_str(), 0), 
                         Term::List::empty () );
        }
  | 
    t = funterm
  | 
    n:NUMBER                 
        {
	  char* endptr = 0;
	  const char* string = n->getText().c_str();
	  strtod (string, &endptr);

	  if (*endptr) { // error returned by strtol
	    throw MyException ("Incorrect floating point number");
	  }
	  t = new Term (sig->createFun (string, 0), 
                         Term::List::empty () );
	}
;


funterm 
  returns [Term* t]
        {
	  Term::List* ts;
	}
  :
  LPAR
  w : WORD
  ts = term_plus
        {
	  t = new Term ( sig->createFun (w->getText().c_str(), ts->length()),
                         ts );
	}
  RPAR     
;


sentence
  returns [Formula* f]
        {
	  Atom* a;
	}
  :
    c : WORD
        {
          f = new Formula (new Atom ( sig->createPred (c->getText().c_str(), 0), 
                                      Term::List::empty () ));
	}
  | 
    a = equation
        {
          f = new Formula (a);
	}
  | 
    a = inequality
        {
          f = new Formula (a);
	}
  | 
    a = relsent
        {
          f = new Formula (a);
	}
  | 
    f = logsent 
  | 
    f = quantsent
;


equation 
  returns [Atom* a]
        {
	  Term* t1;
	  Term* t2;
	}
  :
  LPAR  
  ASSIGN
  t1 = term  
  t2 = term
        {
	  a = new Atom (t1, t2);
	}
  RPAR
;


binary_comparison
  returns [Signature::Pred* p]
:
  MORE
        {
	  p = sig->createPred (">", 2);
	}
  |
  LESS
        {
	  p = sig->createPred ("<", 2);
	}
  |
  GEQ
        {
	  p = sig->createPred (">=", 2);
	}
  |
  LESSEQ
        {
	  p = sig->createPred ("<=", 2);
	}
;


inequality
  returns [Atom* a]
        {
	  Signature::Pred* p;
	  Term* t1;
	  Term* t2;
	}
  :
    LPAR
    p = binary_comparison
    t1 = term  
    t2 = term
    RPAR
        {
	  a = new Atom (p, new Term::List (t1, 
			   new Term::List (t2)));
	}
;


relsent
  returns [Atom* a]
        {
	  Term::List* ts;
	}
  :
  LPAR
  w : WORD
  ts = term_plus
        {
	  a = new Atom ( sig->createPred (w->getText().c_str(), ts->length()),
                         ts );
	}
  RPAR     
;


logsent
  returns [Formula* f]
        {
	  Formula* f1;
	  Formula* f2;
	  Formula::List* fs;
	}
  :
    LPAR  
    kw_not
    f1 = sentence     
    RPAR
        {
	  f = new Formula (Formula::NOT, f1);
	}
  | 
    LPAR  
    kw_and
    fs = sentence_plus  
    RPAR
        {
	  f = new Formula (Formula::AND, fs);
	}
  | 
    LPAR  
    kw_or
    fs = sentence_plus
    RPAR
        {
	  f = new Formula (Formula::OR, fs);
	}
  | LPAR  
    IMPLY
    f1 = sentence
    f2 = sentence  
    RPAR
        {
	  f = new Formula (Formula::IMP, f1, f2);
	}
  | LPAR  
    EQUALITY
    f1 = sentence
    f2 = sentence
    RPAR
        {
	  f = new Formula (Formula::IFF, f1, f2);
	}
;

quantsent 
  returns [Formula* f]
        {
	  Formula* arg;
	  VarList* vs;
	}
  :
    LPAR  kw_forall  LPAR
    vs = variable_plus
    RPAR
    arg = sentence  
    RPAR
        {
	  f = new Formula (Formula::FORALL, vs, arg);
	}
  | 
    LPAR  kw_exists  LPAR
    vs = variable_plus
    RPAR
    arg = sentence  
    RPAR
        {
	  f = new Formula (Formula::EXISTS, vs, arg);
	}
;
//--------------  we avoid use of '+' => some additional rules

// (sentence)+
sentence_plus
  returns [Formula::List* fs]
        {
          Formula::List* rest = Formula::List::empty ();
          Formula* f;
        }
  :
  f = sentence
  ( 
    rest = sentence_plus 
    |
  )
        {
	  fs = new Formula::List (f, rest);
	}
;

// (term)+
term_plus
  returns [Term::List* ts]
        {
          Term::List* rest = Term::List::empty ();
          Term* t;
        }
  :
  t = term
  ( 
    rest = term_plus 
    |
  )
        {
	  ts = new Term::List (t, rest);
	}
;

// (VARIABLE)+
variable_plus  
  returns [VarList* ws]
        {
          VarList* vs = VarList::empty ();
          Var vv;
        }
  :
  vv = variable
  ( 
    vs = variable_plus
    | 
  )
        {
	  ws = new VarList (vv, vs);
	}
;

variable 
  returns [Var v]
  :
  w : VARIABLE
        { 
          v = IntNameTable::vars->insert(w->getText().c_str());
        } 
;


// I do not know why but ANTLR does not recognise literal (such as "not")
// in lexer rules

//------------------   KEYWORDS  ---------------------------------------

// kw_not       returns [int line]	: l: "not"      {line=l->getLine();};
// kw_and       returns [int line]	: l: "and"      {line=l->getLine();};
// kw_or        returns [int line]	: l: "or"       {line=l->getLine();};
// kw_forall    returns [int line]	: l: "forall"   {line=l->getLine();};
// kw_exists    returns [int line]	: l: "exists"   {line=l->getLine();};



kw_not       : "not";
kw_and       : "and";
kw_or        : "or";
kw_forall    : "forall";
kw_exists    : "exists";

