package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: TokenStreamException.java,v 1.1.1.1 2002/08/16 18:22:57 jchen Exp $
 */

/**
 * Anything that goes wrong while generating a stream of tokens.
 */
public class TokenStreamException extends ANTLRException {
	public TokenStreamException() {
	}
public TokenStreamException(String s) {
	super(s);
}
}
