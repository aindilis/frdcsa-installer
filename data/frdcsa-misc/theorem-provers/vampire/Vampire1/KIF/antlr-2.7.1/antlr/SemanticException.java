package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: SemanticException.java,v 1.1.1.1 2002/08/16 18:22:57 jchen Exp $
 */

public class SemanticException extends RecognitionException {
    public SemanticException(String s) {
	super(s);
    }
    public SemanticException(String s, String fileName, int line) {
	super(s, fileName, line);
    }
}
