package antlr;

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: FileLineFormatter.java,v 1.1.1.1 2002/08/16 18:22:55 jchen Exp $
 */

public abstract class FileLineFormatter {
    
    private static FileLineFormatter formatter = new DefaultFileLineFormatter();
    
    public static FileLineFormatter getFormatter() {
	return formatter;
    }
    
    public static void setFormatter(FileLineFormatter f) {
	formatter = f;
    }
    
    public abstract String getFormatString(String fileName, int line);
}
