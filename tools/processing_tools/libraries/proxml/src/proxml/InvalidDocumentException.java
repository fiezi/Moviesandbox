package proxml;

/**
 * An InvalidDocumentException occurs when you try to load a file that does not contain XML.
 * Or the XML file you load has mistakes.
 * @nosuperclasses
 */

public class InvalidDocumentException extends RuntimeException{
    /**
	 * 
	 */
	private static final long serialVersionUID = -3635832302276564720L;

	public InvalidDocumentException () {
        super("This is not a parsable URL");
    }

    public InvalidDocumentException (String url) {
        super(url+" is not a parsable URL");
    }
    
    public InvalidDocumentException (String url, Exception i_exception) {
       super(url+" is not a parsable URL",i_exception);
   }

}
