/*
 * TPTPParser: P a r s e r  H e a d e r 
 *
 * Generated from: parser.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-1998
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR18
 */

#ifndef TPTPParser_h
#define TPTPParser_h

#ifndef ANTLR_VERSION
#define ANTLR_VERSION 13318
#endif

#include "AParser.h"


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
class TPTPParser : public ANTLRParser {
public:
	static  const ANTLRChar *tokenName(int tk);
protected:
	static  ANTLRChar *_token_tbl[];
private:

public:
	static SetWordType err1[4];
	static SetWordType err2[4];
	static SetWordType err3[4];
	static SetWordType setwd1[26];
	static SetWordType err4[4];
	static SetWordType setwd2[26];
	static SetWordType err5[4];
	static SetWordType setwd3[26];
private:
	void zzdflthandlers( int _signal, int *_retsignal );

public:
	TPTPParser(ANTLRTokenBuffer *input);
	 Unit::List*   input(void);
	 Unit::List*   unit_list(void);
	 Unit*   unit(void);
	 Formula*   formula(void);
	 Clause*   clause(void);
	 Literal::List*   literals(void);
	 Literal*   literal(void);
	 InputType   type(void);
	 Formula*   xorformula(void);
	 Formula*   impformula(void);
	 Formula*   orformula(void);
	 Formula*   andformula(void);
	 Formula*   simple_formula(void);
	 Atom*   atom(void);
	 VarList*   varlist(void);
	 VarList*   vars(void);
	 Term::List*   terms(void);
	 Term*   term(void);
};

#endif /* TPTPParser_h */
