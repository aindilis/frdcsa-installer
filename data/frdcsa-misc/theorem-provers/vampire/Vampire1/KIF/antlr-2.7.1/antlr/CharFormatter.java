package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: CharFormatter.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

/** Interface used by BitSet to format elements of the set when
  * converting to string
  */
public interface CharFormatter {


	public String escapeChar(int c, boolean forCharLiteral);
	public String escapeString(String s);
	public String literalChar(int c);
	public String literalString(String s);
}
