package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: CommonAST.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

import antlr.collections.AST;

/** Common AST node implementation */
public class CommonAST extends BaseAST {
	int ttype = Token.INVALID_TYPE;
	String text;


	/** Get the token text for this node */
	public String getText() { return text; }

	/** Get the token type for this node */
	public int getType() { return ttype; }

	public void initialize(int t, String txt) {
		setType(t);
		setText(txt);
	}

	public void initialize(AST t) {
		setText(t.getText());
		setType(t.getType());
	}

	public CommonAST() {
	}

	public CommonAST(Token tok) {
		initialize(tok);
	}

	public void initialize(Token tok) {
		setText(tok.getText());
		setType(tok.getType());
	}
	/** Set the token text for this node */
	public void setText(String text_) { 
		text = text_; 
	}
	/** Set the token type for this node */
	public void setType(int ttype_) { 
		ttype = ttype_; 
	}
}
