package proxml;

import java.util.*;
import java.io.*;

/**
 * XMLElement is the basic class of proXML. You can build a XMLElement and add 
 * Attributes and children, or load it from a certain path using XMLInOut. Text
 * is also handled as XMLElement, so if you want to get the text of an element
 * you have to call element.firstChild().getText(). If you have a part where
 * xml nodes are inside text they seperate the text into several elements, so for
 * example &quot;this is &lt;bold&gt;bold&lt;/bold&gt; ..&quot; would result in the following
 * element list: &quot;this is &quot;,&lt;bold&gt;,&quot;bold&quot;,&lt;/bold&gt;,&quot; ..&quot;
 * @example proxml
 * @related XMLInOut
 */

public class XMLElement{
	/**
     * Holds the values and keys of the elements attributes.
     */
    Hashtable attributes;

    /**
     * Vector keeping the children of this Element
     */
    private Vector children;
	
	/**
	 * true if this element is empty 
	 */
	private boolean empty = true;
	
	/**
	 * true if this element is a pcdata section
	 */
	boolean pcdata;
	
	boolean cdata = false;
	
	 /**
     * Holds the parent of this Element
     */
	private XMLElement parent;
	
    /**
     * String holding the kind of the Element (the tagname)
     */
    private String element;
	
	/**
     * Initializes a new XMLElement with the given name, attributes and children.
     * @param name String, name of the element
     * @param attributes Hashtable, attributes for the element, with names and values
     * @param children Vector, the children of the element
     * @param pcdata boolean, true if the element is a pcdata section
     */
    private XMLElement (
		final String name, 
		final Hashtable attributes, 
		final Vector children, 
		final boolean pcdata
	) {
		this.element = name;
		this.attributes = attributes;
        this.children = children;
		this.pcdata = pcdata;
    }
	
	/**
	 * Initializes a new XMLElement with the given name.
	 * @param name String, name of the element
	 * @param pcdata boolean, true if the element is a pcdata section
	 */
	public XMLElement(
		final String name, 
		final boolean pcdata
	){
		this(name,new Hashtable(),new Vector(),pcdata);
	}

    /**
     * Initializes a new XMLElement with the given name, attributes and children.
     * @param name String, name of the element
     * @param attributes Hashtable, attributes for the element, with names and values
     * @param children Vector, the children of the element
     */
    public XMLElement (
		final String name, 
		final Hashtable attributes, 
		final Vector children
	) {
		this(name,attributes,children,false);
    }
	
	/**
     * Initializes a XMLElement with the given name, but without children and attributes.
     * @param name String, name of the element
     */

    public XMLElement (
		final String name
	) {
        this(name, new Hashtable(), new Vector());
    }

    /**
     * Initializes a XMLElement with the given name and children.
     * @param name String, name of the element
     * @param children Vector, children of the element
     */

    public XMLElement (
		final String name, 
		final Vector children
	) {
        this(name, new Hashtable(), children);
    }

    /**
     * Initializes a new XMLElement with the given name and attributes.
     * @param name String, name of the element
     * @param attributes Hashtable, attributes of the element, with names and values
     */

    public XMLElement (
		final String name, 
		final Hashtable attributes
	){
        this(name, attributes, new Vector());
    }
	
	
	
	/**
     * Checks if a Vector has Content
     * @param toCheck Vector
     * @return boolean
     */
    private boolean has (Vector toCheck) {
        if (toCheck.isEmpty() || toCheck == null) {
            return false;
        } else {
            return true;
        }
    }
	
	/**
	 * Use this method to check if the XMLElement is a text element. 
	 * @return boolean, true if the XMLElement is a PCDATA section.
	 * @example proxml_isPCDATA
	 * @shortdesc Checks if a XMLElement is a text element
	 * @related XMLElement
	 * @related getElement ( )
	 */
	public boolean isTextElement(){
		return pcdata;
	}
	
	/**
	 * Use this method to get the name of a XMLElement. If the XMLElement is a 
	 * PCDATA section getElement() gives you its text.
	 * @return String, the name of the element or the text if it is a text element
	 * @example proxml_getElement
	 * @shortdesc Use this method to get the name or text of an XMLElement. 
	 * @related XMLElement
	 * @related isTextElement ( )
	 */
	public String getElement(){
		return element;
	}
	
	/**
	 * Returns the name of the XML object this is the name of the 
	 * tag that represents the element in the XML file. For example, TITLE is the elementName 
	 * of an HTML TITLE tag. If the XML object is a text element this method returns null.
	 * @return String the name of the element
	 * @shortdesc Returns the name of the element.
	 */
	public String getName(){
		if(isTextElement())return null;
		return element;
	}
	
	/**
	 * If the XML object is a text element this method return the text of the element, otherwise
	 * it returns null.
	 * @return String, the text of a text element
	 * @shortdesc Returns the text of the element.
	 */
	public String getText(){
		if(isTextElement())return element;
		return null;
	}

    /**
     * Returns a String Array with all attribute names of an Element. Use 
     * getAttribute() to get the value for an attribute.
     * @return String[], Array with the Attributes of an Element
     * @example proxml_getAttributes
     * @shortdesc Returns a String Array with all attribute names of an Element.
     * @related XMLElement
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
     */
    public String[] getAttributes () {
		Object[] attributeArray = attributes.keySet().toArray();
		String[] result = new String[attributeArray.length];
		for(int i = 0; i < attributeArray.length; i++){
			result[i] = (String)attributeArray[i];
		}
        return result;
    }

    /**
     * Use getAttribute() to get the value of an attribute as a string. If your are
     * sure, the value is an int or a float value you can also use getIntAttribute() or 
     * getFloatAttribute() to get the numeric value without a cast.
     * @param key String, the name of the attribute you want the value of
     * @return String, the value to the given attribute
     * @example proxml_getAttributes
     * @shortdesc Returns the value of a given attribute.
	  * @related XMLElement
     * @related getAttributes ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
     */
    public String getAttribute (String key) {
		String result = (String)attributes.get(key);
		if(result == null)throw new InvalidAttributeException(this.element,key);
        return result;
    }
	
	/**
	 * Use getIntAttribute() to get the value of an attribute as int value. You 
	 * can only use this method on attributes that are numeric, otherwise you get 
	 * a InvalidAttributeException. 
	 * @param key String, the name of the attribute you want the value of
	 * @return int, the value of the attribute
	 * @example proxml
	 * @shortdesc Use getIntAttribute() to get the value of an attribute as int value.
	  * @related XMLElement
	 * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
	 */
	public int getIntAttribute (String key){
		String attributeValue = (String)attributes.get(key);
		if(attributeValue==null)throw new InvalidAttributeException(this.element,key);
		try{
			return Integer.parseInt((String)attributes.get(key));
		}catch (NumberFormatException e){
			throw new InvalidAttributeException(this.element,key,"int");
		}
	}
	
	/**
	 * Use getFloatAttribute() to get the value of an attribute as float value. You 
	 * can only use this method on attributes that are numeric, otherwise you get 
	 * a InvalidAttributeException. 
	 * @param key String, the name of the attribute you want the value of
	 * @return float, the value of the attribute
	 * @example proxml
	 * @shortdesc Use getFloatAttribute() to get the value of an attribute as float value.
	  * @related XMLElement
	 * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
	 */
	public float getFloatAttribute (String key){
		String attributeValue = (String)attributes.get(key);
		if(attributeValue==null)throw new InvalidAttributeException(this.element,key);
		try{
			return Float.parseFloat((String)attributes.get(key));
		}catch (NumberFormatException e){
			throw new InvalidAttributeException(this.element,key,"int");
		}
	}

    /**
     * Use this method to check if the XMLElement has attributes.
     * @return boolean, true if the XMLElement has attributes
     * @example proxml_hasAttributes
	  * @related XMLElement
     * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
     */
    public boolean hasAttributes () {
        return!attributes.isEmpty();
    }

    /**
     * This method checks if the XMLElement has the given Attribute.
     * @param key String, attribute you want to check
     * @return boolean, true if the XMLElement has the given attribute
     * @example proxml_hasAttribute
     * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related countAttributes ( )
     * @related addAttribute ( )
     */
    public boolean hasAttribute (String key) {
        return attributes.containsKey(key);
    }
	
	/**
	 * Use this method to count the attributes of a XMLElement.
	 * @return int, the number of attributes
	 * @example proxml_countAttributes
	  * @related XMLElement
	 * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related addAttribute ( )
	 */
	public int countAttributes(){
		return attributes.size();
		
	}

    /**
     * With addAttribute() you can add attributes to a XMLElement. The value
     * of attribute can be a String a float or an int. 
     * @param key String, name of the attribute
     * @param value String, int or float: value of the attribute
     * @example proxml
     * @shortdesc With addAttribute() you can add attributes to a XMLElement.
	  * @related XMLElement
     * @related getAttributes ( )
     * @related getAttribute ( )
     * @related getIntAttribute ( )
     * @related getFloatAttribute ( )
     * @related hasAttributes ( )
     * @related hasAttribute ( )
     * @related countAttributes ( )
     */
    public void addAttribute (String key, String value) {
		if(isTextElement())throw new InvalidAttributeException(key);
        attributes.put(key, value);
    }
	
    public void addAttribute (String key, int value) {
		addAttribute(key, value+"");
    }

    public void addAttribute (String key, float value) {
		addAttribute(key, value+"");
    }

    /**
     * With getParent() you can get the parent of a XMLElement. If the 
     * XMLElement is the root element it returns null.
     * @return XMLElement, the parent of the XMLElement or null 
     * if the XMLElement is the root element
     * @example proxml_getParent
     * @shortdesc With getParent() you can get the parent of a XMLElement.
	  * @related XMLElement
     * @related addChild ( )
     * @related countChildren ( )
     * @related getChild ( )
     * @related getChildren ( )
     * @related hasChildren ( )
     */
    public XMLElement getParent () {
        return parent;
    }

    /**
     * Use getChildren() to get an array with all children of an element. 
     * Each element in the array is a reference to an XML object that represents 
     * a child element.
     * @return XMLElement[], an Array of child elements
     * @example proxml_getChildren
     * @shortdesc Returns an Array with all the children of an element.
	  * @related XMLElement
     * @related addChild ( )
     * @related countChildren ( )
     * @related getChild ( )
     * @related getParent ( )
     * @related hasChildren ( )
     */
    public XMLElement[] getChildren () {
		Object[] childArray = children.toArray();
		XMLElement[] result = new XMLElement[childArray.length];
		for(int i = 0; i < childArray.length; i++){
			result[i] = (XMLElement)childArray[i];
		}
        return result;
    }
    
    /**
     * Evaluates the specified XML element and references the first child in the 
     * parent element's child list or null if the element does not 
     * have children.
     * @return XMLElement, the first child element
     * @shortdesc Returns the first child of the element.
     */
    public XMLElement firstChild(){
   	 if(hasChildren())return getChild(0);
   	 else return null;
    }
    
    /**
     * Returns the last child in the element's child list or null if the element does 
     * not have children.
     * @return XMLElement, the last child of the element
     * @shortdesc Returns the last child of the element.
     */
    public XMLElement lastChild(){
   	 if(hasChildren())return getChild(countChildren()-1);
   	 else return null;
    }
    
    /**
     * Returns the next sibling in the parent elements's child list or null if the node does 
     * not have a next sibling element.
     * @return XMLElement, the next sibling of the element
     * @shortdesc Returns the next sibling of the element.
     */
    public XMLElement nextSibling(){
   	 if(parent == null)return null;
   	 
   	 final int index = parent.children.indexOf(this);
   	 
   	 if(index < parent.countChildren()-1){
   		 return parent.getChild(index+1);
   	 }
   	
   	 return null;
    }
    
    /**
     * Returns the previous sibling in the parent node's child list or null if the node does 
     * not have a previous sibling node.
     * @return XMLElement, the previous sibling of the element.
     * @shortdesc Returns the previous sibling of the element.
     */
    public XMLElement previousSibling(){
   	 if(parent == null)return null;
   	 
   	 final int index = parent.children.indexOf(this);
   	 
   	 if(index > 0){
   		 return parent.getChild(index-1);
   	 }
   	
   	 return null;
    }
	
	/**
	 * Use getChild() to get a certain child element of a XMLElement. 
	 * With countAllChildren() you get the number of all children.
	 * @param i int, number of the child
	 * @return XMLElement, the child
	 * @example proxml
	 * @shortdesc Use getChild() to get a certain child element of a XMLElement.
	 * @related XMLElement
	 * @related addChild ( )
    * @related countChildren ( )
    * @related getChildren ( )
    * @related getParent ( )
    * @related hasChildren ( )
	 */
	public XMLElement getChild(int i){
		return ((XMLElement)children.get(i));
	}

    /**
     * Specifies whether or not the XML object has child nodes.
     * @return boolean, true if the specified XMLElement has one or more child nodes; otherwise false.
     * @example proxml_hasChildren
	  * @related XMLElement
     * @related addChild ( )
     * @related countChildren ( )
     * @related getChild ( )
     * @related getChildren ( )
     * @related getParent ( )
     */
    public boolean hasChildren () {
        return has(children);
    }
	
	/**
	 * With countChildren() you get the number of children of a XMLElement.
	 * @return int, the number of children
	 * @example proxml
	 * @related XMLElement
	 * @related addChild ( )
    * @related getChild ( )
    * @related getChildren ( )
    * @related getParent ( )
    * @related hasChildren ( )
	 */
	public int countChildren(){
		return children.size();
	}

   /**
	 * Adds the specified node to the XML element's child list. This method
    * operates directly on the element referenced by the childElement parameter; it
	 * does not append a copy of the element. If the element to be added already
	 * exists in another tree structure, appending the element to the new
	 * location will remove it from its current location. If the childElement
	 * parameter refers to a element that already exists in another XML tree
	 * structure, the appended child element is placed in the new tree structure
	 * after it is removed from its existing parent element.
	 * 
	 * @param element XMLElement, element you want to add as child
	 * @example proxml
	 * @shortdesc Adds the specified node to the XML element's child list.
	 * @related XMLElement
	 * @related addChild ( )
	 * @related countChildren ( )
	 * @related getChildren ( )
	 * @related getParent ( )
	 * @related hasChildren ( )
	 */
	public void addChild(XMLElement element){
		empty = false;
		element.parent = this;
		children.add(element);
	}
	
   /**
	 * @param element XMLElement, element you want to add as child
	 * @param position int, position where you want to insert the element
	 */
   public void addChild (XMLElement element, int position){
   	empty = false;
		element.parent = this;
      children.add(position, element);
    }
    
    /**
     * Removes the specified XML element from its parent. Also deletes all descendants of the element.
     * @param childNumber int, the number of the child to remove
     * @shortdesc Removes the specified XML element from its parent.
     * @related XMLElement
     * @related addChild ( )
     * @related countChildren ( )
     */
    public void removeChild(int childNumber){
   	 children.remove(childNumber);
   	 empty = children.size() == 0;
    }

    /**
     * Use getDepth to get the maximum depth of an Element to one of its leaves.
     * @return int, the maximum depth of an Element to one of its leaves
     * @example proxml_getDepth
	  * @related XMLElement
     * @related countAllChildren ( )
     * @related countAttributes ( )
     * @related countChildren ( )
     */
    public int getDepth () {
        int result = 0;
        XMLElement[] children = getChildren();
        for (int i = 0; i < children.length; i++) {
            result = Math.max(result, children[i].getDepth());
        }
        return 1 + result;
    }

    /**
     * This method returns the number of all nodes of a XMLElement.
     * @return int, the number of all decendents of an Element
     * @example proxml_countAllChildren
	  * @related XMLElement
     * @related getParent ( )
     * @related getDepth ( )
     * @related countAttributes ( )
     * @related countChildren ( )
     */
    public int countAllChildren () {
        int result = 0;
		XMLElement[] children = getChildren();
        for (int i = 0; i < children.length; i++) {
            result += children[i].countAllChildren();
        }
        return 1 + result;
    }

    /**
     * Gives back a vector with elements of the given kind being decendents of this Element
     * @param element String
     * @return Vector
     * @invisible
     */
    public Vector getSpecificElements (String element) {
        Vector result = new Vector();
		XMLElement[] children = getChildren();
        for (int i = 0; i < children.length; i++) {
            if (!children[i].isTextElement()) {
                result.addAll(children[i].getSpecificElements(element));
            }
            if (children[i].element.equals(element)) {
                result.add(children[i]);
            }
        }
        return result;
    }
	
    /**
     * Use toString to get the String representation of a XMLElement. The 
     * Methode gives you the starttag with the name and its attributes, or its text if 
     * it is a PCDATA section.
     * @return String, String representation of the XMLElement
     * @example proxml_toString
     * @shortdesc Use toString to get the String representation of a XMLElement.
	  * @related XMLElement
     * @related printElementTree ( )
     * @related getElement ( )
     * @related isTextElement ( )
     */
    public String toString () {
		if(isTextElement()){
			if(this.cdata){
				final StringBuffer result = new StringBuffer();
				result.append("<![CDATA[");
				result.append(getElement());
				result.append("]]>");
				return result.toString();
			}else{
				String result = getElement();
				result = result.replaceAll("&","&amp;");
				result = result.replaceAll("<","&lt;");
				result = result.replaceAll(">","&gt;");
				result = result.replaceAll("\"","&quot;");
				result = result.replaceAll("'","&apos;");
				return result;
			}
		}
        final StringBuffer result = new StringBuffer();
		result.append("<");
		result.append(element);
        for (Iterator it = attributes.keySet().iterator(); it.hasNext(); ) {
            String key = (String)it.next();
            result.append(" ");
			result.append(key);
			result.append("=\"");
			result.append(attributes.get(key));
			result.append("\"");
        }
		if(empty)result.append("/>");
		else result.append(">");
        return result.toString();
    }
	
 	/**
     * Use this method for a simple trace of the XML structure, beginning from a certain 
     * XMLElement.
     * @shortdesc Prints out the XML  content of the element.
     * @example proxml_printElementTree
	  * @related XMLElement
     * @related toString ( )
     */
    public void printElementTree(){
   	 printElementTree(" ");
    }
    
    /**
     * @param dist String, String for formating the output
     */
    public void printElementTree (String dist) {
        printElementTree("", dist);
    }
	
    /**
     * @param start String, String to put before the element
     */
    void printElementTree(String start, String dist){
		System.out.println(start + this);
		for (int i = 0; i < children.size(); i++){
			((XMLElement) children.get(i)).printElementTree(start + dist, dist);
		}
		if (!empty){
			System.out.println(start + "</" + element + ">");
		}
	}

    /**
     * Prints the tree of this Element with the given distance
     * 
     * @param dist String
     * @param output PrintWriter
     * @related XMLElement
     */
    void printElementTree (PrintWriter output, String dist) {
        printElementTree(output,"", dist);
    }

    /**
     * Prints the tree of this Element with the given distance and start string.
     * @param start String
     * @param dist String
     * @param output PrintWriter
     */
    void printElementTree (PrintWriter output,String start, String dist) {
		output.println(start + this);
        for (int i = 0; i < children.size(); i++) {
            ((XMLElement)children.get(i)).printElementTree(output,start + dist, dist);
        }
		if(!empty){
			output.println(start + "</" + element + ">");
		}
    }
	
	
	
}
