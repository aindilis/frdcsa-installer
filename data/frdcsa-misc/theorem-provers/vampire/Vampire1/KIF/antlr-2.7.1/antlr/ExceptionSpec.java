package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: ExceptionSpec.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

import antlr.collections.impl.Vector;

class ExceptionSpec
{
	// Non-null if this refers to a labeled rule
	// Use a token instead of a string to get the line information
	protected Token label;

	// List of ExceptionHandler (catch phrases)
	protected Vector handlers;


	public ExceptionSpec(Token label_) {
		label = label_;
		handlers = new Vector();
	}
	public void addHandler(ExceptionHandler handler) {
		handlers.appendElement(handler);
	}
}
