package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: CharStreamIOException.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

import java.io.IOException;

/**
 * Wrap an IOException in a CharStreamException
 */
public class CharStreamIOException extends CharStreamException {
    public IOException io;

    public CharStreamIOException(IOException io) {
	super(io.getMessage());
	this.io = io;
    }
}
