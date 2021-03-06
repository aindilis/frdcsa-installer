package antlr.collections;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: List.java,v 1.1.1.1 2002/08/16 18:22:58 jchen Exp $
 */

import java.util.Enumeration;
import java.util.NoSuchElementException;

/**A simple List interface that describes operations
 * on a list.
 */
public interface List {
	public void add(Object o); // can insert at head or append.
	public void append(Object o);
	public Object elementAt(int index) throws NoSuchElementException;
	public Enumeration elements();
	public boolean includes(Object o);
	public int length();
}
