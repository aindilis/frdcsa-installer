package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: CharRangeElement.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

class CharRangeElement extends AlternativeElement {
	String label;
	protected char begin=0;
	protected char end  =0;
	protected String beginText;
	protected String endText;


	public CharRangeElement(LexerGrammar g, Token t1, Token t2, int autoGenType) {
		super(g);
		begin = (char)ANTLRLexer.tokenTypeForCharLiteral(t1.getText());
		beginText = t1.getText();
		end   = (char)ANTLRLexer.tokenTypeForCharLiteral(t2.getText());
		endText = t2.getText();
		line = t1.getLine();
		// track which characters are referenced in the grammar
		for (int i=begin; i<=end; i++) {
			g.charVocabulary.add(i);
		}
		this.autoGenType = autoGenType;
	}
	public void generate() {
		grammar.generator.gen(this);
	}
	public String getLabel() {
		return label;
	}
	public Lookahead look(int k) {
		return grammar.theLLkAnalyzer.look(k, this);
	}
	public void setLabel(String label_) { 
		label = label_; 
	}
	public String toString() {
		if ( label!=null ) return " "+label+":"+beginText+".."+endText;
		else return " "+beginText+".."+endText;
	}
}
