package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: TokenSymbol.java,v 1.1.1.1 2002/08/16 18:22:57 jchen Exp $
 */

class TokenSymbol extends GrammarSymbol {
	protected int ttype;
	/** describes what token matches in "human terms" */
	protected String paraphrase = null;

	/** Set to a value in the tokens {...} section */
	protected String ASTNodeType;

	public TokenSymbol(String r) {
		super(r);
		ttype = Token.INVALID_TYPE;
	}

	public String getASTNodeType() {
		return ASTNodeType;
	}

	public void setASTNodeType(String type) {
		ASTNodeType = type;
	}

	public String getParaphrase() {return paraphrase;}

	public int getTokenType() {
		return ttype;
	}

	public void setParaphrase(String p) {paraphrase = p;}

	public void setTokenType(int t) {
		ttype = t;
	}
}
