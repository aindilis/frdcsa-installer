package antlr.collections;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: AST.java,v 1.1.1.1 2002/08/16 18:22:58 jchen Exp $
 */

import antlr.Token;

/** Minimal AST node interface used by ANTLR AST generation
 * and tree-walker.
 */
public interface AST {
    /** Add a (rightmost) child to this node */
    public void addChild(AST c);
    public boolean equals(AST t);
    public boolean equalsList(AST t);
    public boolean equalsListPartial(AST t);
    public boolean equalsTree(AST t);
    public boolean equalsTreePartial(AST t);
    public ASTEnumeration findAll(AST tree);
    public ASTEnumeration findAllPartial(AST subtree);
    /** Get the first child of this node; null if no children */
    public AST getFirstChild();
    /** Get	the next sibling in line after this one */
    public AST getNextSibling();
    /** Get the token text for this node */
    public String getText();
    /** Get the token type for this node */
    public int getType();
    public void initialize(int t, String txt);
    public void initialize(AST t);
    public void initialize(Token t);
    /** Set the first child of a node. */
    public void setFirstChild(AST c);
    /** Set the next sibling after this one. */
    public void setNextSibling(AST n);
    /** Set the token text for this node */
    public void setText(String text);
    /** Set the token type for this node */
    public void setType(int ttype);
    public String toString();
    public String toStringList();
    public String toStringTree();
}
