package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: SynPredBlock.java,v 1.1.1.1 2002/08/16 18:22:57 jchen Exp $
 */

class SynPredBlock extends AlternativeBlock {


	public SynPredBlock(Grammar g) {
		super(g);
	}
	public SynPredBlock(Grammar g, int line) {
		super(g, line, false);
	}
	public void generate() {
		grammar.generator.gen(this);
	}
	public Lookahead look(int k) {
		return grammar.theLLkAnalyzer.look(k, this);
	}
	public String toString() {
		return super.toString() + "=>";
	}
}
