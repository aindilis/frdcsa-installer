package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: CommonHiddenStreamToken.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

public class CommonHiddenStreamToken extends CommonToken {
    protected CommonHiddenStreamToken hiddenBefore;
    protected CommonHiddenStreamToken hiddenAfter;
    
    public CommonHiddenStreamToken() {
	super();
    }

    public CommonHiddenStreamToken(int t, String txt) {
	super(t, txt);
    }

    public CommonHiddenStreamToken(String s) {
	super(s);
    }

    public CommonHiddenStreamToken getHiddenAfter() {
	return hiddenAfter;
    }

    public CommonHiddenStreamToken getHiddenBefore() {
	return hiddenBefore;
    }

    protected void setHiddenAfter(CommonHiddenStreamToken t) {
	hiddenAfter = t;
    }

    protected void setHiddenBefore(CommonHiddenStreamToken t) {
	hiddenBefore = t;
    }
}
