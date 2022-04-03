package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: BlockWithImpliedExitPath.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

abstract class BlockWithImpliedExitPath extends AlternativeBlock {
	protected int exitLookaheadDepth;	// lookahead needed to handle optional path
	/** lookahead to bypass block; set
	 * by deterministic().  1..k of Lookahead
	 */
	protected Lookahead[] exitCache = new Lookahead[grammar.maxk+1];


	public BlockWithImpliedExitPath(Grammar g) {
		super(g);
	}
public BlockWithImpliedExitPath(Grammar g, int line) {
	super(g, line, false);
}
}
