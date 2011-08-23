package proxml;

/**
 * An InvalidAttributeException occurs when a XMLElement does not have the requested 
 * attribute, or when you use getIntAttribute() or getFloatAttribute() for Attributes 
 * that are not numeric. Another reason could be that you try to add an attribute to a PCDATA 
 * section.
 * @nosuperclasses
 */

public class InvalidAttributeException extends RuntimeException{

	public InvalidAttributeException(String attributeName){
		super("You can't add the attribute " + attributeName + " to a PCDATA section.");
	}

	public InvalidAttributeException(String elementName, String attributeName){
		super("The XMLElement " + elementName + " has no attribute " + attributeName + "!");
	}

	public InvalidAttributeException(String elementName, String attributeName, String type){
		super("The XMLElement " + elementName + " has no attribute " + attributeName + " of the type " + type + "!");
	}

}
