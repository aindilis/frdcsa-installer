/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-1998
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR18
 *
 *   ..\pccts\bin\antlr.exe -ga -CC -e3 parser.g
 *
 */

#define ANTLR_VERSION	13318
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "tokens.h"

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
#include "AParser.h"
#include "TPTPParser.h"
#include "DLexerBase.h"
#include "ATokPtr.h"
#ifndef PURIFY
#define PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif
// scanner definitions would go here

#include <iostream.h>
#include <stdlib.h>

#include "DLexerBase.h"
#include "DLGLexer.h"
#include "AToken.h"

  

 Unit::List*  
TPTPParser::input(void)
{
  TRACER ( "TPTPParser::input" );

	 Unit::List*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Unit::List*  	))
	 _retv  = unit_list();

	zzmatch(1);
	 consume();
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x1);
	return _retv;
}

 Unit::List*  
TPTPParser::unit_list(void)
{
	 Unit::List*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Unit::List*  	))
	
	Unit* first;
	Unit::List* rest = Unit::List::empty ();
	 first  = unit();

	{
		if ( (setwd1[LA(1)]&0x2) ) {
			 rest  = unit_list();

		}
	}
	
	_retv = new Unit::List ( first, rest );
	return _retv;
}

 Unit*  
TPTPParser::unit(void)
{
	 Unit*  	 _retv;
	zzRULE;
	ANTLRTokenPtr nm1=NULL, nm2=NULL;
	PURIFY(_retv,sizeof( Unit*  	))
	
	Formula* f;
	Clause* c;
	InputType t;
	char* nameCopy;
	char* name;
	if ( (LA(1)==T_INFORM) ) {
		zzmatch(T_INFORM);
		 consume();
		zzmatch(T_LPAR);
		 consume();
		zzmatch(T_NAME);
		
		nm1 = (ANTLRTokenPtr)LT(1);
 consume();
		zzmatch(T_COMMA);
		 consume();
		 t  = type();

		zzmatch(T_COMMA);
		 consume();
		zzmatch(T_LPAR);
		 consume();
		 f  = formula();

		zzmatch(T_RPAR);
		 consume();
		zzmatch(T_RPAR);
		 consume();
		zzmatch(T_DOT);
		
		
		name =  nm1->getText ();
		nameCopy = new char [ strlen(name) + 1 ];
		strcpy ( nameCopy, name );
		_retv = new Unit ( nameCopy, t, f );
 consume();
	}
	else {
		if ( (LA(1)==T_INCLAUSE) ) {
			zzmatch(T_INCLAUSE);
			 consume();
			zzmatch(T_LPAR);
			 consume();
			zzmatch(T_NAME);
			
			nm2 = (ANTLRTokenPtr)LT(1);
 consume();
			zzmatch(T_COMMA);
			 consume();
			 t  = type();

			zzmatch(T_COMMA);
			 consume();
			 c  = clause();

			zzmatch(T_RPAR);
			 consume();
			zzmatch(T_DOT);
			
			
			name =  nm2->getText ();
			nameCopy = new char [ strlen(name) + 1 ];
			strcpy ( nameCopy, name );
			_retv = new Unit ( nameCopy, t, c );
 consume();
		}
		else {FAIL(1,err1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x8);
	return _retv;
}

 Formula*  
TPTPParser::formula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* lft;
	Formula* rht = 0;
	 lft  = xorformula();

	{
		if ( (LA(1)==T_IFF) ) {
			zzmatch(T_IFF);
			 consume();
			 rht  = formula();

		}
	}
	
	if ( rht )
	_retv = new Formula ( Formula::IFF, lft, rht );
	else
	_retv = lft;
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x10);
	return _retv;
}

 Clause*  
TPTPParser::clause(void)
{
	 Clause*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Clause*  	))
	
	Literal::List* l;
	zzmatch(T_LBRA);
	 consume();
	 l  = literals();

	zzmatch(T_RBRA);
	
	
	_retv = new Clause ( l );
 consume();
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x20);
	return _retv;
}

 Literal::List*  
TPTPParser::literals(void)
{
	 Literal::List*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Literal::List*  	))
	
	_retv = Literal::List::empty ();
	if ( (LA(1)==T_RBRA) ) {
	}
	else {
		if ( (setwd1[LA(1)]&0x40) ) {
			
			Literal* first;
			Literal::List* rest = Literal::List::empty ();
			 first  = literal();

			{
				if ( (LA(1)==T_COMMA) ) {
					zzmatch(T_COMMA);
					 consume();
					 rest  = literals();

				}
			}
			
			_retv = new Literal::List ( first, rest );
		}
		else {FAIL(1,err2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x80);
	return _retv;
}

 Literal*  
TPTPParser::literal(void)
{
	 Literal*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Literal*  	))
	
	Atom* a;
	if ( (LA(1)==T_PP) ) {
		zzmatch(T_PP);
		 consume();
		 a  = atom();

		
		_retv = new Literal ( true, a );
	}
	else {
		if ( (LA(1)==T_MM) ) {
			zzmatch(T_MM);
			 consume();
			 a  = atom();

			
			_retv = new Literal ( false, a );
		}
		else {FAIL(1,err3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x1);
	return _retv;
}

 InputType  
TPTPParser::type(void)
{
	 InputType  	 _retv;
	zzRULE;
	ANTLRTokenPtr tp=NULL;
	PURIFY(_retv,sizeof( InputType  	))
	zzmatch(T_NAME);
	
	tp = (ANTLRTokenPtr)LT(1);

	
	if ( ! strcmp ( tp->getText(), "axiom") )
	_retv = AXIOM;
	else if ( ! strcmp ( tp->getText(), "conjecture") )
	_retv = CONJECTURE;
	else if ( ! strcmp ( tp->getText(), "hypothesis") )
	_retv = HYPOTHESIS;
 consume();
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x2);
	return _retv;
}

 Formula*  
TPTPParser::xorformula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* lft;
	Formula* rht = 0;
	 lft  = impformula();

	{
		if ( (LA(1)==T_XOR) ) {
			zzmatch(T_XOR);
			 consume();
			 rht  = xorformula();

		}
	}
	
	if ( rht )
	_retv = new Formula ( Formula::XOR, lft, rht );
	else
	_retv = lft;
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x4);
	return _retv;
}

 Formula*  
TPTPParser::impformula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* lft;
	Formula* rht = 0;
	 lft  = orformula();

	{
		if ( (LA(1)==T_IMP) ) {
			zzmatch(T_IMP);
			 consume();
			 rht  = impformula();

		}
	}
	
	if ( rht )
	_retv = new Formula ( Formula::IMP, lft, rht );
	else
	_retv = lft;
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x8);
	return _retv;
}

 Formula*  
TPTPParser::orformula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* lft;
	Formula* rht = 0;
	 lft  = andformula();

	{
		if ( (LA(1)==T_OR) ) {
			zzmatch(T_OR);
			 consume();
			 rht  = orformula();

		}
	}
	
	if ( rht )
	_retv = Formula::parseJunction ( Formula::OR, lft, rht );
	else
	_retv = lft;
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x10);
	return _retv;
}

 Formula*  
TPTPParser::andformula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* lft;
	Formula* rht = 0;
	 lft  = simple_formula();

	{
		if ( (LA(1)==T_AND) ) {
			zzmatch(T_AND);
			 consume();
			 rht  = andformula();

		}
	}
	
	if ( rht )
	_retv = Formula::parseJunction ( Formula::AND, lft, rht );
	else
	_retv = lft;
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x20);
	return _retv;
}

 Formula*  
TPTPParser::simple_formula(void)
{
	 Formula*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Formula*  	))
	
	Formula* arg;
	VarList* vs;
	Atom* a;
	if ( (LA(1)==T_NOT) ) {
		zzmatch(T_NOT);
		 consume();
		 arg  = simple_formula();

		
		_retv = new Formula ( Formula::NOT, arg );
	}
	else {
		if ( (LA(1)==T_FORALL) ) {
			zzmatch(T_FORALL);
			 consume();
			 vs  = varlist();

			zzmatch(T_COLON);
			 consume();
			 arg  = simple_formula();

			
			_retv = new Formula ( Formula::FORALL, vs, arg );
		}
		else {
			if ( (LA(1)==T_EXISTS) ) {
				zzmatch(T_EXISTS);
				 consume();
				 vs  = varlist();

				zzmatch(T_COLON);
				 consume();
				 arg  = simple_formula();

				
				_retv = new Formula ( Formula::EXISTS, vs, arg );
			}
			else {
				if ( (LA(1)==T_LPAR) ) {
					zzmatch(T_LPAR);
					 consume();
					 _retv  = formula();

					zzmatch(T_RPAR);
					 consume();
				}
				else {
					if ( (LA(1)==T_NAME) ) {
						 a  = atom();

						
						_retv = new Formula ( a );
					}
					else {FAIL(1,err4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
		}
	}
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x40);
	return _retv;
}

 Atom*  
TPTPParser::atom(void)
{
	 Atom*  	 _retv;
	zzRULE;
	ANTLRTokenPtr fun=NULL;
	PURIFY(_retv,sizeof( Atom*  	))
	
	Term::List* ts = 0;
	zzmatch(T_NAME);
	
	fun = (ANTLRTokenPtr)LT(1);
 consume();
	{
		if ( (LA(1)==T_LPAR) ) {
			zzmatch(T_LPAR);
			 consume();
			 ts  = terms();

			zzmatch(T_RPAR);
			 consume();
		}
	}
	
	_retv = new Atom ( sig->createPred ( fun->getText (),ts->length()),
	ts );
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x80);
	return _retv;
}

 VarList*  
TPTPParser::varlist(void)
{
	 VarList*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( VarList*  	))
	zzmatch(T_LBRA);
	 consume();
	 _retv  = vars();

	zzmatch(T_RBRA);
	 consume();
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x1);
	return _retv;
}

 VarList*  
TPTPParser::vars(void)
{
	 VarList*  	 _retv;
	zzRULE;
	ANTLRTokenPtr v=NULL;
	PURIFY(_retv,sizeof( VarList*  	))
	
	VarList* rest = 0;
	Var var;
	zzmatch(T_VAR);
	
	v = (ANTLRTokenPtr)LT(1);
 consume();
	{
		if ( (LA(1)==T_COMMA) ) {
			zzmatch(T_COMMA);
			 consume();
			 rest  = vars();

		}
	}
	
	var = IntNameTable::vars->insert ( v->getText ());
	_retv = new VarList ( var, rest );
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x2);
	return _retv;
}

 Term::List*  
TPTPParser::terms(void)
{
	 Term::List*  	 _retv;
	zzRULE;
	PURIFY(_retv,sizeof( Term::List*  	))
	
	Term::List* rest = 0;
	Term* t;
	 t  = term();

	{
		if ( (LA(1)==T_COMMA) ) {
			zzmatch(T_COMMA);
			 consume();
			 rest  = terms();

		}
	}
	
	_retv = new Term::List ( t, rest );
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x4);
	return _retv;
}

 Term*  
TPTPParser::term(void)
{
	 Term*  	 _retv;
	zzRULE;
	ANTLRTokenPtr fun=NULL, var=NULL;
	PURIFY(_retv,sizeof( Term*  	))
	
	Term::List* ts = 0;
	Var v;
	if ( (LA(1)==T_NAME) ) {
		zzmatch(T_NAME);
		
		fun = (ANTLRTokenPtr)LT(1);
 consume();
		{
			if ( (LA(1)==T_LPAR) ) {
				zzmatch(T_LPAR);
				 consume();
				 ts  = terms();

				zzmatch(T_RPAR);
				 consume();
			}
		}
		
		_retv = new Term ( sig->createFun ( fun->getText (),ts->length()),
		ts );
	}
	else {
		if ( (LA(1)==T_VAR) ) {
			zzmatch(T_VAR);
			
			var = (ANTLRTokenPtr)LT(1);

			
			v = IntNameTable::vars->insert ( var->getText ());
			_retv = new Term ( v );
 consume();
		}
		else {FAIL(1,err5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	return _retv;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x8);
	return _retv;
}
