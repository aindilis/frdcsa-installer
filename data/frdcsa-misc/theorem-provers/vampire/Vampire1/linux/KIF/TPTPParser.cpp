/*
 * TPTPParser: P a r s e r  S u p p o r t
 *
 * Generated from: parser.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-1998
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR18
 */

#define ANTLR_VERSION	13318
#include "pcctscfg.h"
#include "pccts_stdio.h"
#define ANTLR_SUPPORT_CODE
#include "tokens.h"
#include "TPTPParser.h"

const ANTLRChar *TPTPParser::tokenName(int tok)   { return _token_tbl[tok]; }

ANTLRChar *TPTPParser::_token_tbl[]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"[\\ \\t\\r]",
	/* 03 */	"% ~[\\n]* [\\n]",
	/* 04 */	"[\\n]",
	/* 05 */	"T_LPAR",
	/* 06 */	"T_RPAR",
	/* 07 */	"T_LBRA",
	/* 08 */	"T_RBRA",
	/* 09 */	"T_COMMA",
	/* 10 */	"T_COLON",
	/* 11 */	"T_DOT",
	/* 12 */	"T_INFORM",
	/* 13 */	"T_INCLAUSE",
	/* 14 */	"T_AND",
	/* 15 */	"T_NOT",
	/* 16 */	"T_OR",
	/* 17 */	"T_IFF",
	/* 18 */	"T_IMP",
	/* 19 */	"T_XOR",
	/* 20 */	"T_FORALL",
	/* 21 */	"T_EXISTS",
	/* 22 */	"T_PP",
	/* 23 */	"T_MM",
	/* 24 */	"T_NAME",
	/* 25 */	"T_VAR"
};

TPTPParser::TPTPParser(ANTLRTokenBuffer *input) : ANTLRParser(input,1,0,0,4)
{
	token_tbl = _token_tbl;
	traceOptionValueDefault=0;		// MR10 turn trace OFF
}

SetWordType TPTPParser::err1[4] = {0x0,0x30,0x0,0x0};
SetWordType TPTPParser::err2[4] = {0x0,0x1,0xc0,0x0};
SetWordType TPTPParser::err3[4] = {0x0,0x0,0xc0,0x0};
SetWordType TPTPParser::setwd1[26] = {0x0,0xd,0x0,0x0,0x0,0x0,0x30,
	0x0,0x80,0x0,0x0,0x0,0xa,0xa,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,
	0x40,0x0,0x0};
SetWordType TPTPParser::err4[4] = {0x20,0x80,0x30,0x1};
SetWordType TPTPParser::setwd2[26] = {0x0,0x0,0x0,0x0,0x0,0x0,0xfc,
	0x0,0x81,0x83,0x0,0x0,0x0,0x0,0xc0,
	0x0,0xe0,0xfc,0xf0,0xf8,0x0,0x0,0x0,
	0x0,0x0,0x0};
SetWordType TPTPParser::err5[4] = {0x0,0x0,0x0,0x3};
SetWordType TPTPParser::setwd3[26] = {0x0,0x0,0x0,0x0,0x0,0x0,0xc,
	0x0,0x2,0x8,0x1,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0};
