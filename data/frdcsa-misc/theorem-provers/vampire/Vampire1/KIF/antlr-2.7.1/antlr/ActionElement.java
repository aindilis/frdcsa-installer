package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: ActionElement.java,v 1.1.1.1 2002/08/16 18:22:54 jchen Exp $
 */

class ActionElement extends AlternativeElement {
	protected String actionText;
	protected boolean isSemPred = false;


	public ActionElement(Grammar g, Token t) {
		super(g);
		actionText = t.getText();
		line = t.getLine();
	}
	public void generate() {
		grammar.generator.gen(this);
	}
	public Lookahead look(int k) {
		return grammar.theLLkAnalyzer.look(k, this);
	}
	public String toString() {
		return " "+actionText + (isSemPred?"?":"");
	}
}
