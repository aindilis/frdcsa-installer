#header
<<
   // any definitions that one needs in the
   //  generated files

#include "term.h"
#include "literal.h"
#include "clause.h"
#include "formula.h"
#include "int_name_table.h"
#include "unit.h"
#include "signature.h"
#include "problem.h"
>>


<< // scanner definitions would go here

#include <iostream.h>
#include <stdlib.h>

#include "DLexerBase.h"
#include "DLGLexer.h"
#include "AToken.h"

>>

// scanner rules

#lexclass START // Not really necessary, but
                // good commentary nonetheless

// White Space
#token "[\ \t\r]"
    <<
      skip ();
    >>

// Comments
#token "% ~[\n]* [\n]"
    <<
      skip ();
      newline ();
    >>

// End of line
#token "[\n]"
    <<
      skip();
      newline ();
    >>

// Punctuation
#token T_LPAR        "\("
#token T_RPAR        "\)"
#token T_LBRA         "\["
#token T_RBRA         "\]"
#token T_COMMA       "\,"
#token T_COLON       "\:"
#token T_DOT         "\."

// keywords
#token T_INFORM        "input_formula"
#token T_INCLAUSE      "input_clause"

// logical connectives
#token T_AND          "\&"
#token T_NOT          "\~"
#token T_OR           "\|"
#token T_IFF          "\<\=\>"
#token T_IMP          "\=\>"
#token T_XOR          "\<\~\>"
#token T_FORALL       "\!"
#token T_EXISTS       "\?"
#token T_PP           "\+\+"
#token T_MM           "\-\-"

// idenitifiers
#token T_NAME      "[a-z][A-Za-z_0-9]* | [0-9]+"
#token T_VAR      "[A-Z_][A-Za-z_0-9]*"

class TPTPParser
{
      <<
          public:
      >>

input > [Unit::List* fs]
  :

  unit_list > [$fs]
  "@" // end of file

  ;

unit_list > [Unit::List* p]
  :
    <<
      Unit* first;
      Unit::List* rest = Unit::List::empty ();
    >>

  unit > [first]
  {
  unit_list > [rest]
  }

    <<
      $p = new Unit::List ( first, rest );
    >>
  ;

unit > [Unit* u]
  :
    <<
      Formula* f;
      Clause* c;
      InputType t;
      char* nameCopy;
      char* name;
    >>

  T_INFORM      // input_formula
  T_LPAR        //              (
  nm1 : T_NAME   //              name
  T_COMMA       //                   ,
  type > [t]    //                    axiom
  T_COMMA       //                         ,
  T_LPAR        //                          (
  formula > [f] //                           a <=> b
  T_RPAR        //                                  )
  T_RPAR        //                                   )
  T_DOT         //                                    .

    <<
      name = $nm1->getText ();
      nameCopy = new char [ strlen(name) + 1 ];
      strcpy ( nameCopy, name );
      $u = new Unit ( nameCopy, t, f );
    >>

  |

  T_INCLAUSE    // input_clause
  T_LPAR        //              (
  nm2 : T_NAME   //              name
  T_COMMA       //                   ,
  type > [t]    //                    axiom
  T_COMMA       //                         ,
  clause > [c] //                           [++ a, --b]
  T_RPAR        //                                     )
  T_DOT         //                                      .

    <<
      name = $nm2->getText ();
      nameCopy = new char [ strlen(name) + 1 ];
      strcpy ( nameCopy, name );
      $u = new Unit ( nameCopy, t, c );
    >>
  ;

formula > [Formula* f]
  :
    <<
      Formula* lft;
      Formula* rht = 0;
    >>

  xorformula > [lft]
  {
  T_IFF
  formula > [rht]
  }

    <<
      if ( rht )
        $f = new Formula ( Formula::IFF, lft, rht );
      else
        $f = lft;
    >>
  ;

clause > [Clause* c]
  :
    <<
      Literal::List* l;
    >>

  T_LBRA
  literals > [l]
  T_RBRA

    <<
      $c = new Clause ( l );
    >>
  ;

literals > [Literal::List* ls]
  :
    <<
      $ls = Literal::List::empty ();
    >>

  |

    <<
      Literal* first;
      Literal::List* rest = Literal::List::empty ();
    >>

  literal > [first]
  {
  T_COMMA
  literals > [rest]
  }

    <<
      $ls = new Literal::List ( first, rest );
    >>
  ;

literal > [Literal* l]
  :
    <<
      Atom* a;
    >>

  T_PP
  atom > [a]

    <<
      $l = new Literal ( true, a );
    >>
  |

  T_MM
  atom > [a]

    <<
      $l = new Literal ( false, a );
    >>
  ;

type > [InputType b]
  :

  tp : T_NAME   //              name

    <<
      if ( ! strcmp ($tp->getText(), "axiom") )
        $b = AXIOM;
      else if ( ! strcmp ($tp->getText(), "conjecture") )
        $b = CONJECTURE;
      else if ( ! strcmp ($tp->getText(), "hypothesis") )
        $b = HYPOTHESIS;
    >>
  ;

xorformula > [Formula* f]
  :
    <<
      Formula* lft;
      Formula* rht = 0;
    >>

  impformula > [lft]
  {
  T_XOR
  xorformula > [rht]
  }

    <<
      if ( rht )
        $f = new Formula ( Formula::XOR, lft, rht );
      else
        $f = lft;
    >>
  ;

impformula > [Formula* f]
  :
    <<
      Formula* lft;
      Formula* rht = 0;
    >>

  orformula > [lft]
  {
  T_IMP
  impformula > [rht]
  }

    <<
      if ( rht )
        $f = new Formula ( Formula::IMP, lft, rht );
      else
        $f = lft;
    >>
  ;

orformula > [Formula* f]
  :
    <<
      Formula* lft;
      Formula* rht = 0;
    >>

  andformula > [lft]
  {
  T_OR
  orformula > [rht]
  }

    <<
      if ( rht )
        $f = Formula::parseJunction ( Formula::OR, lft, rht );
      else
        $f = lft;
    >>
  ;

andformula > [Formula* f]
  :
    <<
      Formula* lft;
      Formula* rht = 0;
    >>

  simple_formula > [lft]
  {
  T_AND
  andformula > [rht]
  }

    <<
      if ( rht )
        $f = Formula::parseJunction ( Formula::AND, lft, rht );
      else
        $f = lft;
    >>
  ;

simple_formula  > [Formula* f]
  :

    <<
      Formula* arg;
      VarList* vs;
      Atom* a;
    >>

  T_NOT
  simple_formula > [arg]

    <<
      $f = new Formula ( Formula::NOT, arg );
    >>
  |

  T_FORALL
  varlist > [vs]
  T_COLON
  simple_formula > [arg]

    <<
      $f = new Formula ( Formula::FORALL, vs, arg );
    >>
  |

  T_EXISTS
  varlist > [vs]
  T_COLON
  simple_formula > [arg]

    <<
      $f = new Formula ( Formula::EXISTS, vs, arg );
    >>
  |

  T_LPAR
  formula > [$f]
  T_RPAR

  |

  atom > [a]

    <<
      $f = new Formula ( a );
    >>
  ;

atom > [Atom* f]
  :

    <<
      Term::List* ts = 0;
    >>

  fun : T_NAME
  {
  T_LPAR
  terms > [ts]
  T_RPAR
  }

    <<
      $f = new Atom ( sig->createPred ($fun->getText (),ts->length()),
                      ts );
    >>
  ;

varlist > [VarList* vs]
  :

  T_LBRA
  vars > [$vs]
  T_RBRA

  ;

vars > [VarList* vs]
  :

    <<
      VarList* rest = 0;
      Var var;
    >>

  v : T_VAR
  {
  T_COMMA
  vars > [rest]
  }

    <<
      var = IntNameTable::vars->insert ($v->getText ());
      $vs = new VarList ( var, rest );
    >>
  ;

terms > [Term::List* ts]
  :

    <<
      Term::List* rest = 0;
      Term* t;
    >>

  term > [t]
  {
  T_COMMA
  terms > [rest]
  }

    <<
      $ts = new Term::List ( t, rest );
    >>
  ;

term > [Term* t]
  :

    <<
      Term::List* ts = 0;
      Var v;
    >>

  fun : T_NAME
  {
  T_LPAR
  terms > [ts]
  T_RPAR
  }

    <<
      $t = new Term ( sig->createFun ($fun->getText (),ts->length()),
                      ts );
    >>
  |

  var : T_VAR

    <<
      v = IntNameTable::vars->insert ($var->getText ());
      $t = new Term ( v );
    >>
  ;
