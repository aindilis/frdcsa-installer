/** This software is an adaptation of the theorem prover Vampire for
 * working with large knowledge bases in the KIF format, see 
 * http://www.prover.info for publications on Vampire.
 *
 * Copyright (C) Andrei Voronkov and Alexandre Riazanov
 *
 * @author Alexandre Riazanov <riazanov@cs.man.ac.uk>
 * @author Andrei Voronkov <voronkov@cs.man.ac.uk>, <andrei@voronkov.com>
 *
 * @date 06/06/2003
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Also add information on how to contact you by electronic and paper mail.
 */

/* $ANTLR 2.7.1: "KIF.g" -> "KIFLexer.cpp"$ */
#include "KIFLexer.hpp"
#include "antlr/CharBuffer.hpp"
#include "antlr/TokenStreamException.hpp"
#include "antlr/TokenStreamIOException.hpp"
#include "antlr/TokenStreamRecognitionException.hpp"
#include "antlr/CharStreamException.hpp"
#include "antlr/CharStreamIOException.hpp"
#include "antlr/NoViableAltForCharException.hpp"

#line 1 "KIF.g"

#line 14 "KIFLexer.cpp"
KIFLexer::KIFLexer(ANTLR_USE_NAMESPACE(std)istream& in)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in))
{
	setCaseSensitive(true);
	initLiterals();
}

KIFLexer::KIFLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(ib)
{
	setCaseSensitive(true);
	initLiterals();
}

KIFLexer::KIFLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(state)
{
	setCaseSensitive(true);
	initLiterals();
}

void KIFLexer::initLiterals()
{
	literals["exists"] = 35;
	literals["not"] = 31;
	literals["or"] = 33;
	literals["forall"] = 34;
	literals["and"] = 32;
	literals["equal"] = 30;
}
bool KIFLexer::getCaseSensitiveLiterals() const
{
	return true;
}

ANTLR_USE_NAMESPACE(antlr)RefToken KIFLexer::nextToken()
{
	ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
	for (;;) {
		ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
		int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
		resetText();
		try {   // for char stream error handling
			try {   // for lexical error handling
				switch ( LA(1)) {
				case static_cast<unsigned char>('A'):
				case static_cast<unsigned char>('B'):
				case static_cast<unsigned char>('C'):
				case static_cast<unsigned char>('D'):
				case static_cast<unsigned char>('E'):
				case static_cast<unsigned char>('F'):
				case static_cast<unsigned char>('G'):
				case static_cast<unsigned char>('H'):
				case static_cast<unsigned char>('I'):
				case static_cast<unsigned char>('J'):
				case static_cast<unsigned char>('K'):
				case static_cast<unsigned char>('L'):
				case static_cast<unsigned char>('M'):
				case static_cast<unsigned char>('N'):
				case static_cast<unsigned char>('O'):
				case static_cast<unsigned char>('P'):
				case static_cast<unsigned char>('Q'):
				case static_cast<unsigned char>('R'):
				case static_cast<unsigned char>('S'):
				case static_cast<unsigned char>('T'):
				case static_cast<unsigned char>('U'):
				case static_cast<unsigned char>('V'):
				case static_cast<unsigned char>('W'):
				case static_cast<unsigned char>('X'):
				case static_cast<unsigned char>('Y'):
				case static_cast<unsigned char>('Z'):
				case static_cast<unsigned char>('a'):
				case static_cast<unsigned char>('b'):
				case static_cast<unsigned char>('c'):
				case static_cast<unsigned char>('d'):
				case static_cast<unsigned char>('e'):
				case static_cast<unsigned char>('f'):
				case static_cast<unsigned char>('g'):
				case static_cast<unsigned char>('h'):
				case static_cast<unsigned char>('i'):
				case static_cast<unsigned char>('j'):
				case static_cast<unsigned char>('k'):
				case static_cast<unsigned char>('l'):
				case static_cast<unsigned char>('m'):
				case static_cast<unsigned char>('n'):
				case static_cast<unsigned char>('o'):
				case static_cast<unsigned char>('p'):
				case static_cast<unsigned char>('q'):
				case static_cast<unsigned char>('r'):
				case static_cast<unsigned char>('s'):
				case static_cast<unsigned char>('t'):
				case static_cast<unsigned char>('u'):
				case static_cast<unsigned char>('v'):
				case static_cast<unsigned char>('w'):
				case static_cast<unsigned char>('x'):
				case static_cast<unsigned char>('y'):
				case static_cast<unsigned char>('z'):
				{
					mWORD(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('"'):
				{
					mSTRING(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('?'):
				case static_cast<unsigned char>('@'):
				{
					mVARIABLE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('-'):
				case static_cast<unsigned char>('0'):
				case static_cast<unsigned char>('1'):
				case static_cast<unsigned char>('2'):
				case static_cast<unsigned char>('3'):
				case static_cast<unsigned char>('4'):
				case static_cast<unsigned char>('5'):
				case static_cast<unsigned char>('6'):
				case static_cast<unsigned char>('7'):
				case static_cast<unsigned char>('8'):
				case static_cast<unsigned char>('9'):
				{
					mNUMBER(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('`'):
				{
					mQUOTE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('\t'):
				case static_cast<unsigned char>('\n'):
				case static_cast<unsigned char>('\14'):
				case static_cast<unsigned char>('\r'):
				case static_cast<unsigned char>(' '):
				{
					mSKIP_RULE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>(';'):
				{
					mCOMMENT_LINE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('('):
				{
					mLPAR(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>(')'):
				{
					mRPAR(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('='):
				{
					mIMPLY(true);
					theRetToken=_returnToken;
					break;
				}
				default:
					if ((LA(1)==static_cast<unsigned char>('<')) && (LA(2)==static_cast<unsigned char>('=')) && (LA(3)==static_cast<unsigned char>('>'))) {
						mEQUIVALENCE(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==static_cast<unsigned char>('<')) && (LA(2)==static_cast<unsigned char>('=')) && (true)) {
						mLESSEQ(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==static_cast<unsigned char>('>')) && (LA(2)==static_cast<unsigned char>('='))) {
						mGEQ(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==static_cast<unsigned char>('<')) && (true)) {
						mLESS(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==static_cast<unsigned char>('>')) && (true)) {
						mMORE(true);
						theRetToken=_returnToken;
					}
				else {
					if (LA(1)==EOF_CHAR) {uponEOF(); _returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);}
				else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				}
				if ( !_returnToken ) goto tryAgain; // found SKIP token
				_ttype = _returnToken->getType();
				_ttype = testLiteralsTable(_ttype);
				_returnToken->setType(_ttype);
				return _returnToken;
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
				throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamIOException& csie) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamIOException(csie.io);
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamException& cse) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamException(cse.getMessage());
		}
tryAgain:;
	}
}

void KIFLexer::mUPPER(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = UPPER;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('A'):
	{
		match(static_cast<unsigned char>('A'));
		break;
	}
	case static_cast<unsigned char>('B'):
	{
		match(static_cast<unsigned char>('B'));
		break;
	}
	case static_cast<unsigned char>('C'):
	{
		match(static_cast<unsigned char>('C'));
		break;
	}
	case static_cast<unsigned char>('D'):
	{
		match(static_cast<unsigned char>('D'));
		break;
	}
	case static_cast<unsigned char>('E'):
	{
		match(static_cast<unsigned char>('E'));
		break;
	}
	case static_cast<unsigned char>('F'):
	{
		match(static_cast<unsigned char>('F'));
		break;
	}
	case static_cast<unsigned char>('G'):
	{
		match(static_cast<unsigned char>('G'));
		break;
	}
	case static_cast<unsigned char>('H'):
	{
		match(static_cast<unsigned char>('H'));
		break;
	}
	case static_cast<unsigned char>('I'):
	{
		match(static_cast<unsigned char>('I'));
		break;
	}
	case static_cast<unsigned char>('J'):
	{
		match(static_cast<unsigned char>('J'));
		break;
	}
	case static_cast<unsigned char>('K'):
	{
		match(static_cast<unsigned char>('K'));
		break;
	}
	case static_cast<unsigned char>('L'):
	{
		match(static_cast<unsigned char>('L'));
		break;
	}
	case static_cast<unsigned char>('M'):
	{
		match(static_cast<unsigned char>('M'));
		break;
	}
	case static_cast<unsigned char>('N'):
	{
		match(static_cast<unsigned char>('N'));
		break;
	}
	case static_cast<unsigned char>('O'):
	{
		match(static_cast<unsigned char>('O'));
		break;
	}
	case static_cast<unsigned char>('P'):
	{
		match(static_cast<unsigned char>('P'));
		break;
	}
	case static_cast<unsigned char>('Q'):
	{
		match(static_cast<unsigned char>('Q'));
		break;
	}
	case static_cast<unsigned char>('R'):
	{
		match(static_cast<unsigned char>('R'));
		break;
	}
	case static_cast<unsigned char>('S'):
	{
		match(static_cast<unsigned char>('S'));
		break;
	}
	case static_cast<unsigned char>('T'):
	{
		match(static_cast<unsigned char>('T'));
		break;
	}
	case static_cast<unsigned char>('U'):
	{
		match(static_cast<unsigned char>('U'));
		break;
	}
	case static_cast<unsigned char>('V'):
	{
		match(static_cast<unsigned char>('V'));
		break;
	}
	case static_cast<unsigned char>('W'):
	{
		match(static_cast<unsigned char>('W'));
		break;
	}
	case static_cast<unsigned char>('X'):
	{
		match(static_cast<unsigned char>('X'));
		break;
	}
	case static_cast<unsigned char>('Y'):
	{
		match(static_cast<unsigned char>('Y'));
		break;
	}
	case static_cast<unsigned char>('Z'):
	{
		match(static_cast<unsigned char>('Z'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mLOWER(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = LOWER;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('a'):
	{
		match(static_cast<unsigned char>('a'));
		break;
	}
	case static_cast<unsigned char>('b'):
	{
		match(static_cast<unsigned char>('b'));
		break;
	}
	case static_cast<unsigned char>('c'):
	{
		match(static_cast<unsigned char>('c'));
		break;
	}
	case static_cast<unsigned char>('d'):
	{
		match(static_cast<unsigned char>('d'));
		break;
	}
	case static_cast<unsigned char>('e'):
	{
		match(static_cast<unsigned char>('e'));
		break;
	}
	case static_cast<unsigned char>('f'):
	{
		match(static_cast<unsigned char>('f'));
		break;
	}
	case static_cast<unsigned char>('g'):
	{
		match(static_cast<unsigned char>('g'));
		break;
	}
	case static_cast<unsigned char>('h'):
	{
		match(static_cast<unsigned char>('h'));
		break;
	}
	case static_cast<unsigned char>('i'):
	{
		match(static_cast<unsigned char>('i'));
		break;
	}
	case static_cast<unsigned char>('j'):
	{
		match(static_cast<unsigned char>('j'));
		break;
	}
	case static_cast<unsigned char>('k'):
	{
		match(static_cast<unsigned char>('k'));
		break;
	}
	case static_cast<unsigned char>('l'):
	{
		match(static_cast<unsigned char>('l'));
		break;
	}
	case static_cast<unsigned char>('m'):
	{
		match(static_cast<unsigned char>('m'));
		break;
	}
	case static_cast<unsigned char>('n'):
	{
		match(static_cast<unsigned char>('n'));
		break;
	}
	case static_cast<unsigned char>('o'):
	{
		match(static_cast<unsigned char>('o'));
		break;
	}
	case static_cast<unsigned char>('p'):
	{
		match(static_cast<unsigned char>('p'));
		break;
	}
	case static_cast<unsigned char>('q'):
	{
		match(static_cast<unsigned char>('q'));
		break;
	}
	case static_cast<unsigned char>('r'):
	{
		match(static_cast<unsigned char>('r'));
		break;
	}
	case static_cast<unsigned char>('s'):
	{
		match(static_cast<unsigned char>('s'));
		break;
	}
	case static_cast<unsigned char>('t'):
	{
		match(static_cast<unsigned char>('t'));
		break;
	}
	case static_cast<unsigned char>('u'):
	{
		match(static_cast<unsigned char>('u'));
		break;
	}
	case static_cast<unsigned char>('v'):
	{
		match(static_cast<unsigned char>('v'));
		break;
	}
	case static_cast<unsigned char>('w'):
	{
		match(static_cast<unsigned char>('w'));
		break;
	}
	case static_cast<unsigned char>('x'):
	{
		match(static_cast<unsigned char>('x'));
		break;
	}
	case static_cast<unsigned char>('y'):
	{
		match(static_cast<unsigned char>('y'));
		break;
	}
	case static_cast<unsigned char>('z'):
	{
		match(static_cast<unsigned char>('z'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mDIGIT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = DIGIT;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('0'):
	{
		match(static_cast<unsigned char>('0'));
		break;
	}
	case static_cast<unsigned char>('1'):
	{
		match(static_cast<unsigned char>('1'));
		break;
	}
	case static_cast<unsigned char>('2'):
	{
		match(static_cast<unsigned char>('2'));
		break;
	}
	case static_cast<unsigned char>('3'):
	{
		match(static_cast<unsigned char>('3'));
		break;
	}
	case static_cast<unsigned char>('4'):
	{
		match(static_cast<unsigned char>('4'));
		break;
	}
	case static_cast<unsigned char>('5'):
	{
		match(static_cast<unsigned char>('5'));
		break;
	}
	case static_cast<unsigned char>('6'):
	{
		match(static_cast<unsigned char>('6'));
		break;
	}
	case static_cast<unsigned char>('7'):
	{
		match(static_cast<unsigned char>('7'));
		break;
	}
	case static_cast<unsigned char>('8'):
	{
		match(static_cast<unsigned char>('8'));
		break;
	}
	case static_cast<unsigned char>('9'):
	{
		match(static_cast<unsigned char>('9'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mSPECIAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = SPECIAL;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('!'):
	{
		match(static_cast<unsigned char>('!'));
		break;
	}
	case static_cast<unsigned char>('$'):
	{
		match(static_cast<unsigned char>('$'));
		break;
	}
	case static_cast<unsigned char>('%'):
	{
		match(static_cast<unsigned char>('%'));
		break;
	}
	case static_cast<unsigned char>('&'):
	{
		match(static_cast<unsigned char>('&'));
		break;
	}
	case static_cast<unsigned char>('*'):
	{
		match(static_cast<unsigned char>('*'));
		break;
	}
	case static_cast<unsigned char>('+'):
	{
		match(static_cast<unsigned char>('+'));
		break;
	}
	case static_cast<unsigned char>('-'):
	{
		match(static_cast<unsigned char>('-'));
		break;
	}
	case static_cast<unsigned char>('.'):
	{
		match(static_cast<unsigned char>('.'));
		break;
	}
	case static_cast<unsigned char>('/'):
	{
		match(static_cast<unsigned char>('/'));
		break;
	}
	case static_cast<unsigned char>('<'):
	{
		match(static_cast<unsigned char>('<'));
		break;
	}
	case static_cast<unsigned char>('='):
	{
		match(static_cast<unsigned char>('='));
		break;
	}
	case static_cast<unsigned char>('>'):
	{
		match(static_cast<unsigned char>('>'));
		break;
	}
	case static_cast<unsigned char>('?'):
	{
		match(static_cast<unsigned char>('?'));
		break;
	}
	case static_cast<unsigned char>('@'):
	{
		match(static_cast<unsigned char>('@'));
		break;
	}
	case static_cast<unsigned char>('_'):
	{
		match(static_cast<unsigned char>('_'));
		break;
	}
	case static_cast<unsigned char>('~'):
	{
		match(static_cast<unsigned char>('~'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mWHITE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WHITE;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>(' '):
	{
		match(static_cast<unsigned char>(' '));
		break;
	}
	case static_cast<unsigned char>('\t'):
	{
		match(static_cast<unsigned char>('\t'));
		break;
	}
	case static_cast<unsigned char>('\r'):
	{
		match(static_cast<unsigned char>('\r'));
		break;
	}
	case static_cast<unsigned char>('\n'):
	{
		{
		match(static_cast<unsigned char>('\n'));
#line 66 "KIF.g"
		newline();
#line 725 "KIFLexer.cpp"
		}
		break;
	}
	case static_cast<unsigned char>('\14'):
	{
		match(static_cast<unsigned char>('\14'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mINITIALCHAR(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = INITIALCHAR;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('A'):
	case static_cast<unsigned char>('B'):
	case static_cast<unsigned char>('C'):
	case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):
	case static_cast<unsigned char>('F'):
	case static_cast<unsigned char>('G'):
	case static_cast<unsigned char>('H'):
	case static_cast<unsigned char>('I'):
	case static_cast<unsigned char>('J'):
	case static_cast<unsigned char>('K'):
	case static_cast<unsigned char>('L'):
	case static_cast<unsigned char>('M'):
	case static_cast<unsigned char>('N'):
	case static_cast<unsigned char>('O'):
	case static_cast<unsigned char>('P'):
	case static_cast<unsigned char>('Q'):
	case static_cast<unsigned char>('R'):
	case static_cast<unsigned char>('S'):
	case static_cast<unsigned char>('T'):
	case static_cast<unsigned char>('U'):
	case static_cast<unsigned char>('V'):
	case static_cast<unsigned char>('W'):
	case static_cast<unsigned char>('X'):
	case static_cast<unsigned char>('Y'):
	case static_cast<unsigned char>('Z'):
	{
		mUPPER(false);
		break;
	}
	case static_cast<unsigned char>('a'):
	case static_cast<unsigned char>('b'):
	case static_cast<unsigned char>('c'):
	case static_cast<unsigned char>('d'):
	case static_cast<unsigned char>('e'):
	case static_cast<unsigned char>('f'):
	case static_cast<unsigned char>('g'):
	case static_cast<unsigned char>('h'):
	case static_cast<unsigned char>('i'):
	case static_cast<unsigned char>('j'):
	case static_cast<unsigned char>('k'):
	case static_cast<unsigned char>('l'):
	case static_cast<unsigned char>('m'):
	case static_cast<unsigned char>('n'):
	case static_cast<unsigned char>('o'):
	case static_cast<unsigned char>('p'):
	case static_cast<unsigned char>('q'):
	case static_cast<unsigned char>('r'):
	case static_cast<unsigned char>('s'):
	case static_cast<unsigned char>('t'):
	case static_cast<unsigned char>('u'):
	case static_cast<unsigned char>('v'):
	case static_cast<unsigned char>('w'):
	case static_cast<unsigned char>('x'):
	case static_cast<unsigned char>('y'):
	case static_cast<unsigned char>('z'):
	{
		mLOWER(false);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mWORDCHAR(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WORDCHAR;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('A'):
	case static_cast<unsigned char>('B'):
	case static_cast<unsigned char>('C'):
	case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):
	case static_cast<unsigned char>('F'):
	case static_cast<unsigned char>('G'):
	case static_cast<unsigned char>('H'):
	case static_cast<unsigned char>('I'):
	case static_cast<unsigned char>('J'):
	case static_cast<unsigned char>('K'):
	case static_cast<unsigned char>('L'):
	case static_cast<unsigned char>('M'):
	case static_cast<unsigned char>('N'):
	case static_cast<unsigned char>('O'):
	case static_cast<unsigned char>('P'):
	case static_cast<unsigned char>('Q'):
	case static_cast<unsigned char>('R'):
	case static_cast<unsigned char>('S'):
	case static_cast<unsigned char>('T'):
	case static_cast<unsigned char>('U'):
	case static_cast<unsigned char>('V'):
	case static_cast<unsigned char>('W'):
	case static_cast<unsigned char>('X'):
	case static_cast<unsigned char>('Y'):
	case static_cast<unsigned char>('Z'):
	{
		mUPPER(false);
		break;
	}
	case static_cast<unsigned char>('a'):
	case static_cast<unsigned char>('b'):
	case static_cast<unsigned char>('c'):
	case static_cast<unsigned char>('d'):
	case static_cast<unsigned char>('e'):
	case static_cast<unsigned char>('f'):
	case static_cast<unsigned char>('g'):
	case static_cast<unsigned char>('h'):
	case static_cast<unsigned char>('i'):
	case static_cast<unsigned char>('j'):
	case static_cast<unsigned char>('k'):
	case static_cast<unsigned char>('l'):
	case static_cast<unsigned char>('m'):
	case static_cast<unsigned char>('n'):
	case static_cast<unsigned char>('o'):
	case static_cast<unsigned char>('p'):
	case static_cast<unsigned char>('q'):
	case static_cast<unsigned char>('r'):
	case static_cast<unsigned char>('s'):
	case static_cast<unsigned char>('t'):
	case static_cast<unsigned char>('u'):
	case static_cast<unsigned char>('v'):
	case static_cast<unsigned char>('w'):
	case static_cast<unsigned char>('x'):
	case static_cast<unsigned char>('y'):
	case static_cast<unsigned char>('z'):
	{
		mLOWER(false);
		break;
	}
	case static_cast<unsigned char>('0'):
	case static_cast<unsigned char>('1'):
	case static_cast<unsigned char>('2'):
	case static_cast<unsigned char>('3'):
	case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):
	case static_cast<unsigned char>('6'):
	case static_cast<unsigned char>('7'):
	case static_cast<unsigned char>('8'):
	case static_cast<unsigned char>('9'):
	{
		mDIGIT(false);
		break;
	}
	case static_cast<unsigned char>('!'):
	case static_cast<unsigned char>('$'):
	case static_cast<unsigned char>('%'):
	case static_cast<unsigned char>('&'):
	case static_cast<unsigned char>('*'):
	case static_cast<unsigned char>('+'):
	case static_cast<unsigned char>('-'):
	case static_cast<unsigned char>('.'):
	case static_cast<unsigned char>('/'):
	case static_cast<unsigned char>('<'):
	case static_cast<unsigned char>('='):
	case static_cast<unsigned char>('>'):
	case static_cast<unsigned char>('?'):
	case static_cast<unsigned char>('@'):
	case static_cast<unsigned char>('_'):
	case static_cast<unsigned char>('~'):
	{
		mSPECIAL(false);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mCHARACTER(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = CHARACTER;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('A'):
	case static_cast<unsigned char>('B'):
	case static_cast<unsigned char>('C'):
	case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):
	case static_cast<unsigned char>('F'):
	case static_cast<unsigned char>('G'):
	case static_cast<unsigned char>('H'):
	case static_cast<unsigned char>('I'):
	case static_cast<unsigned char>('J'):
	case static_cast<unsigned char>('K'):
	case static_cast<unsigned char>('L'):
	case static_cast<unsigned char>('M'):
	case static_cast<unsigned char>('N'):
	case static_cast<unsigned char>('O'):
	case static_cast<unsigned char>('P'):
	case static_cast<unsigned char>('Q'):
	case static_cast<unsigned char>('R'):
	case static_cast<unsigned char>('S'):
	case static_cast<unsigned char>('T'):
	case static_cast<unsigned char>('U'):
	case static_cast<unsigned char>('V'):
	case static_cast<unsigned char>('W'):
	case static_cast<unsigned char>('X'):
	case static_cast<unsigned char>('Y'):
	case static_cast<unsigned char>('Z'):
	{
		mUPPER(false);
		break;
	}
	case static_cast<unsigned char>('a'):
	case static_cast<unsigned char>('b'):
	case static_cast<unsigned char>('c'):
	case static_cast<unsigned char>('d'):
	case static_cast<unsigned char>('e'):
	case static_cast<unsigned char>('f'):
	case static_cast<unsigned char>('g'):
	case static_cast<unsigned char>('h'):
	case static_cast<unsigned char>('i'):
	case static_cast<unsigned char>('j'):
	case static_cast<unsigned char>('k'):
	case static_cast<unsigned char>('l'):
	case static_cast<unsigned char>('m'):
	case static_cast<unsigned char>('n'):
	case static_cast<unsigned char>('o'):
	case static_cast<unsigned char>('p'):
	case static_cast<unsigned char>('q'):
	case static_cast<unsigned char>('r'):
	case static_cast<unsigned char>('s'):
	case static_cast<unsigned char>('t'):
	case static_cast<unsigned char>('u'):
	case static_cast<unsigned char>('v'):
	case static_cast<unsigned char>('w'):
	case static_cast<unsigned char>('x'):
	case static_cast<unsigned char>('y'):
	case static_cast<unsigned char>('z'):
	{
		mLOWER(false);
		break;
	}
	case static_cast<unsigned char>('0'):
	case static_cast<unsigned char>('1'):
	case static_cast<unsigned char>('2'):
	case static_cast<unsigned char>('3'):
	case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):
	case static_cast<unsigned char>('6'):
	case static_cast<unsigned char>('7'):
	case static_cast<unsigned char>('8'):
	case static_cast<unsigned char>('9'):
	{
		mDIGIT(false);
		break;
	}
	case static_cast<unsigned char>('!'):
	case static_cast<unsigned char>('$'):
	case static_cast<unsigned char>('%'):
	case static_cast<unsigned char>('&'):
	case static_cast<unsigned char>('*'):
	case static_cast<unsigned char>('+'):
	case static_cast<unsigned char>('-'):
	case static_cast<unsigned char>('.'):
	case static_cast<unsigned char>('/'):
	case static_cast<unsigned char>('<'):
	case static_cast<unsigned char>('='):
	case static_cast<unsigned char>('>'):
	case static_cast<unsigned char>('?'):
	case static_cast<unsigned char>('@'):
	case static_cast<unsigned char>('_'):
	case static_cast<unsigned char>('~'):
	{
		mSPECIAL(false);
		break;
	}
	case static_cast<unsigned char>('\t'):
	case static_cast<unsigned char>('\n'):
	case static_cast<unsigned char>('\14'):
	case static_cast<unsigned char>('\r'):
	case static_cast<unsigned char>(' '):
	{
		mWHITE(false);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mTICK(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = TICK;
	int _saveIndex;
	
	match(static_cast<unsigned char>('`'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mWORD(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WORD;
	int _saveIndex;
	
	mINITIALCHAR(false);
	{
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			mWORDCHAR(false);
		}
		else {
			goto _loop13;
		}
		
	}
	_loop13:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mSTRING(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = STRING;
	int _saveIndex;
	
	match(static_cast<unsigned char>('"'));
	{
	for (;;) {
		switch ( LA(1)) {
		case static_cast<unsigned char>('\n'):
		{
			{
			match(static_cast<unsigned char>('\n'));
#line 93 "KIF.g"
			newline();
#line 1116 "KIFLexer.cpp"
			}
			break;
		}
		case static_cast<unsigned char>('\\'):
		{
			{
			match(static_cast<unsigned char>('\\'));
			matchNot(EOF/*_CHAR*/);
			}
			break;
		}
		default:
			if ((_tokenSet_1.member(LA(1)))) {
				{
				{
				match(_tokenSet_1);
				}
				}
			}
		else {
			goto _loop20;
		}
		}
	}
	_loop20:;
	}
	match(static_cast<unsigned char>('"'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mVARIABLE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = VARIABLE;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('?'):
	{
		match(static_cast<unsigned char>('?'));
		mWORD(false);
		break;
	}
	case static_cast<unsigned char>('@'):
	{
		match(static_cast<unsigned char>('@'));
		mWORD(false);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mFIGURE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = FIGURE;
	int _saveIndex;
	
	mDIGIT(false);
	{
	if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
		mFIGURE(false);
	}
	else {
	}
	
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mNUMBER(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = NUMBER;
	int _saveIndex;
	
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('-'):
	{
		match(static_cast<unsigned char>('-'));
		break;
	}
	case static_cast<unsigned char>('0'):
	case static_cast<unsigned char>('1'):
	case static_cast<unsigned char>('2'):
	case static_cast<unsigned char>('3'):
	case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):
	case static_cast<unsigned char>('6'):
	case static_cast<unsigned char>('7'):
	case static_cast<unsigned char>('8'):
	case static_cast<unsigned char>('9'):
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	mFIGURE(false);
	{
	if ((LA(1)==static_cast<unsigned char>('.'))) {
		match(static_cast<unsigned char>('.'));
		mFIGURE(false);
	}
	else {
	}
	
	}
	{
	if ((LA(1)==static_cast<unsigned char>('e'))) {
		mEXPONENT(false);
	}
	else {
	}
	
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mEXPONENT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = EXPONENT;
	int _saveIndex;
	
	match(static_cast<unsigned char>('e'));
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('-'):
	{
		match(static_cast<unsigned char>('-'));
		break;
	}
	case static_cast<unsigned char>('0'):
	case static_cast<unsigned char>('1'):
	case static_cast<unsigned char>('2'):
	case static_cast<unsigned char>('3'):
	case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):
	case static_cast<unsigned char>('6'):
	case static_cast<unsigned char>('7'):
	case static_cast<unsigned char>('8'):
	case static_cast<unsigned char>('9'):
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	mFIGURE(false);
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mQUOTE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = QUOTE;
	int _saveIndex;
	
	mTICK(false);
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mSKIP_RULE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = SKIP_RULE;
	int _saveIndex;
	
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>(' '):
	{
		match(static_cast<unsigned char>(' '));
		break;
	}
	case static_cast<unsigned char>('\t'):
	{
		match(static_cast<unsigned char>('\t'));
		break;
	}
	case static_cast<unsigned char>('\r'):
	{
		match(static_cast<unsigned char>('\r'));
		break;
	}
	case static_cast<unsigned char>('\n'):
	{
		{
		match(static_cast<unsigned char>('\n'));
#line 118 "KIF.g"
		newline();
#line 1345 "KIFLexer.cpp"
		}
		break;
	}
	case static_cast<unsigned char>('\14'):
	{
		match(static_cast<unsigned char>('\14'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
#line 119 "KIF.g"
	_ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP;
#line 1362 "KIFLexer.cpp"
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mCOMMENT_LINE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = COMMENT_LINE;
	int _saveIndex;
	
	match(static_cast<unsigned char>(';'));
	{
	for (;;) {
		if ((_tokenSet_2.member(LA(1)))) {
			{
			match(_tokenSet_2);
			}
		}
		else {
			goto _loop37;
		}
		
	}
	_loop37:;
	}
#line 124 "KIF.g"
	_ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP;
#line 1393 "KIFLexer.cpp"
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mLPAR(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = LPAR;
	int _saveIndex;
	
	match(static_cast<unsigned char>('('));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mRPAR(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = RPAR;
	int _saveIndex;
	
	match(static_cast<unsigned char>(')'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mLESS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = LESS;
	int _saveIndex;
	
	match(static_cast<unsigned char>('<'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mMORE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = MORE;
	int _saveIndex;
	
	match(static_cast<unsigned char>('>'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mLESSEQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = LESSEQ;
	int _saveIndex;
	
	match("<=");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mGEQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = GEQ;
	int _saveIndex;
	
	match(">=");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mIMPLY(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = IMPLY;
	int _saveIndex;
	
	match("=>");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void KIFLexer::mEQUIVALENCE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = EQUIVALENCE;
	int _saveIndex;
	
	match("<=>");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}


const unsigned long KIFLexer::_tokenSet_0_data_[] = { 0UL, 4093635698UL, 2281701375UL, 1207959550UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet KIFLexer::_tokenSet_0(_tokenSet_0_data_,10);
const unsigned long KIFLexer::_tokenSet_1_data_[] = { 4294966271UL, 4294967291UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet KIFLexer::_tokenSet_1(_tokenSet_1_data_,16);
const unsigned long KIFLexer::_tokenSet_2_data_[] = { 4294966271UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet KIFLexer::_tokenSet_2(_tokenSet_2_data_,16);

