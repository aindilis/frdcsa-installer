package antlr.collections.impl;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: IntRange.java,v 1.1.1.1 2002/08/16 18:22:58 jchen Exp $
 */

public class IntRange {
	int begin, end;


	public IntRange(int begin, int end) {
		this.begin = begin;
		this.end = end;
	}
	public String toString() {
		return begin+".."+end;
	}
}
