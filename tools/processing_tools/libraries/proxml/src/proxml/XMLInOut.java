package proxml;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.Reader;
import java.io.StringReader;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Hashtable;

import processing.core.PApplet;

/**
 * Use XMLInOut for simple loading and saving of XML files. If you load a xml file
 * the parsed XMLElement is passed to the xmlEvent() method in your sketch. To be
 * able to load xml files you have to implement this function, other wise you get an 
 * exception. It is also possible to implement this function in another object, to
 * do so you have to give the constructor a reference to your object.
 * 
 * 
 * @example proxml
 * @related XMLElement
 */
public class XMLInOut{

	/**
	 * Loader for loading XML in background while running the sketch.
	 * @author tex
	 *
	 */
	private class Loader implements Runnable{

		/**
		 * String to keep the String of the document to parse
		 */
		Reader document;

		/**
		 * String to keep the String of the document to parse
		 */
		Reader keep;
		
		/**
		 * Object handling the incoming XML
		 */
		Object xmlHandler;

		Loader(final Reader i_document, final Object i_xmlHandler){
			document = i_document;
			xmlHandler = i_xmlHandler;
		}

		/**
		 * Returns the source of the desired document
		 * @return
		 */
		String getSource(){
			int iChar;
			StringBuffer result = new StringBuffer();
			try{
				while ((iChar = keep.read()) != -1){
					result.append((char) iChar);
				}
			}catch (Exception e){
				return ("fails");
			}
			return result.toString();
		}

		private boolean firstTag = false;

		private boolean rootNode = false;

		private int line = 0;

		/**
		 * Parses a given String and gives back box with the parsed Element and the
		 * String still have to be parsed.
		 * @param toParse String
		 * @return BoxToParseElement
		 */
		private XMLElement parseDocument(Reader document){

			firstTag = true;
			rootNode = true;

			int iChar; //keeps the int value of the current char
			char cChar; //keeps the char value of the current char

			StringBuffer sbText = new StringBuffer(); //StringBuffer to parse words in
			boolean bText = false; //has a word been parsed
			try{
				while ((iChar = document.read()) != -1){ //as long there is something to read
					cChar = (char) iChar; //get the current char value
					switch (cChar){ //check the char value
						case '\b':
							break;
						case '\n':
							line++;
							break;
						case '\f':
							break;
						case '\r':
							break;
						case '\t':
							break;
						case '<': //this opens a tag so...
							if (bText){
								bText = false;
								actualElement.addChild(new XMLElement(sbText.toString(), true));
								sbText = new StringBuffer();
							}
							if ((iChar = document.read()) != -1){ //check the next sign...
								cChar = (char) iChar; //get its char value..

								if (cChar == '/'){ //in this case we have an end tag
									document = handleEndTag(result, document); // and handle it
									break;
								}else if (cChar == '!'){ //this could be a comment, but we need a further test
									if ((iChar = document.read()) != -1){ //you should know this now
										cChar = (char) iChar; //also this one
										if (cChar == '-'){ //okay its a comment
											document = handleComment(document); //handle it
											break;
										}else if (cChar == '['){//seems to be CDATA Section
											document = handleCDATASection(document);
											break;
										}else if (cChar == 'D'){//seems to be Doctype Section
											document = handleDoctypeSection(document);
											break;
										}
									}
								}
							}

							document = handleStartTag(document, new StringBuffer().append(cChar));

							break;
						default:
							if (!(cChar == ' ' && !bText)){
								bText = true;
								if (cChar == '&'){
									document = handleEntity(document, sbText);
								}else{
									sbText.append(cChar);
								}
							}
					}
				}
			}catch (Exception e){
				e.printStackTrace();
			}
			return result;
		}

		/**
		 * Parses a TemplateTag and extracts its Name and Attributes.
		 * @param page Reader
		 * @param alreadyParsed StringBuffer
		 * @return Reader
		 * @throws Exception
		 */
		private Reader handleStartTag(Reader page, StringBuffer alreadyParsed) throws Exception{
			int iChar;
			char cChar;

			boolean bTagName = true;
			boolean bSpaceBefore = false;
			boolean bLeftAttribute = false;

			StringBuffer sbTagName = alreadyParsed;
			StringBuffer sbAttributeName = new StringBuffer();
			StringBuffer sbAttributeValue = new StringBuffer();
			StringBuffer sbActual = sbTagName;

			Hashtable attributes = new Hashtable();
			boolean inValue = false;
			char oChar = ' ';

			while ((iChar = page.read()) != -1){
				cChar = (char) iChar;
				switch (cChar){
					case '\b':
						break;
					case '\f':
						break;
					case '\r':
						break;
					case '\n':
						line++;
					case '\t':
					case ' ':
						if (!bSpaceBefore){
							if (!inValue){
								if (bTagName){
									bTagName = false;
								}else{
									String sAttributeName = sbAttributeName.toString();
									String sAttributeValue = sbAttributeValue.toString();
									attributes.put(sAttributeName, sAttributeValue);

									sbAttributeName = new StringBuffer();
									sbAttributeValue = new StringBuffer();
									bLeftAttribute = false;
								}
								sbActual = sbAttributeName;
							}else{
								sbActual.append(cChar);
							}
						}
						bSpaceBefore = true;
						break;
					case '=':
						if (!inValue){
							sbActual = sbAttributeValue;
							bLeftAttribute = true;
						}else{
							sbActual.append(cChar);
						}
						break;
					case '"':
						inValue = !inValue;
						try{
							if (!inValue && sbActual.charAt(sbActual.length() - 1) == ' '){
								sbActual.deleteCharAt(sbActual.length() - 1);
							}
						}catch (java.lang.StringIndexOutOfBoundsException e){
							System.out.println(sbActual.toString());
						}
						bSpaceBefore = false;
						break;
					case '\'':
						break;
					case '/':
						if (inValue)
							sbActual.append(cChar);
						break;
					case '>':
						if (bLeftAttribute){
							String sAttributeName = sbAttributeName.toString();
							String sAttributeValue = sbAttributeValue.toString();
							attributes.put(sAttributeName, sAttributeValue);
						}
						String sTagName = sbTagName.toString();
						if (firstTag){
							firstTag = false;
							if (!(sTagName.equals("doctype") || sTagName.equals("?xml")))
								throw new RuntimeException("XML File has no valid header");
						}else{
							if (rootNode && !firstTag){
								rootNode = false;
								result = new XMLElement(sTagName, attributes);
								actualElement = result;
							}else{
								XMLElement keep = new XMLElement(sTagName, attributes);
								actualElement.addChild(keep);
								if (oChar != '/')
									actualElement = keep;
							}
						}

						return page;

					default:
						bSpaceBefore = false;
						sbActual.append(cChar);
				}
				oChar = cChar;
			}

			throw new RuntimeException("Error in line:"+line);
		}

		/**
		 * Parses the end tags of a XML document
		 * 
		 * @param toParse Reader
		 * @return Reader
		 * @throws Exception
		 */
		private Reader handleEndTag(XMLElement xmlElement, Reader toParse) throws Exception{
			int iChar;
			char cChar;
			while ((iChar = toParse.read()) != -1){

				cChar = (char) iChar;
				switch (cChar){
					case '\b':
						break;
					case '\n':
						line++;
						break;
					case '\f':
						break;
					case '\r':
						break;
					case '\t':
						break;
					case '>':
						if (!actualElement.equals(xmlElement))
							actualElement = actualElement.getParent();
						return toParse;
					default:
				}
			}
			throw new RuntimeException("Error in line:"+line);
		}

		/**
		 * Parses the comments of a XML document
		 * 
		 * @param toParse Reader
		 * @return Reader
		 * @throws Exception
		 */
		private Reader handleComment(Reader toParse) throws Exception{
			int iChar;
			char cChar;
			char prevChar = ' ';

			while ((iChar = toParse.read()) != -1){
				cChar = (char) iChar;
				if (prevChar == '-' && cChar == '>'){
					return toParse;
				}
				prevChar = cChar;
			}
			throw new RuntimeException("Comment is not correctly closed in Line:"+line);
		}
		
		/**
		 * Parses the Doctype section of a XML document
		 * 
		 * @param toParse Reader
		 * @return Reader
		 * @throws Exception
		 */
		private Reader handleDoctypeSection(Reader toParse) throws Exception{
			int iChar;
			char cChar;
			char prevChar = ' ';
			
			boolean entities = false;

			while ((iChar = toParse.read()) != -1){
				cChar = (char) iChar;
				if(cChar == '[')entities = true;
				if (cChar == '>'){
					if(prevChar == ']' && entities || !entities)
					return toParse;
				}
				prevChar = cChar;
			}
			throw new RuntimeException("Comment is not correctly closed in Line:"+line);
		}

		/**
		 * Parses Entities of a document
		 * 
		 * @param toParse
		 * @param stringBuffer
		 * @return
		 * @throws Exception
		 */
		private Reader handleEntity(Reader toParse, final StringBuffer stringBuffer) throws Exception{
			int iChar;
			char cChar;
			final StringBuffer result = new StringBuffer();
			int counter = 0;

			while ((iChar = toParse.read()) != -1){
				cChar = (char) iChar;
				result.append(cChar);
				if (cChar == ';'){
					final String entity = result.toString().toLowerCase();
					if (entity.equals("lt;"))
						stringBuffer.append("<");
					else if (entity.equals("gt;"))
						stringBuffer.append(">");
					else if (entity.equals("amp;"))
						stringBuffer.append("&");
					else if (entity.equals("quot;"))
						stringBuffer.append("\"");
					else if (entity.equals("apos;"))
						stringBuffer.append("'");
					break;
				}
				counter++;
				if (counter > 4)
					throw new RuntimeException("Illegal use of &. Use &amp; entity instead. Line:"+line);
			}

			return toParse;
		}

		/**
		 * Parses a CData Section of a document
		 * @param toParse
		 * @return
		 * @throws Exception
		 */
		private Reader handleCDATASection(Reader toParse) throws Exception{
			int iChar;
			char cChar;
			StringBuffer result = new StringBuffer();
			int counter = 0;
			boolean checkedCDATA = false;

			while ((iChar = toParse.read()) != -1){
				cChar = (char) iChar;
				if (cChar == ']'){
					XMLElement keep = new XMLElement(result.toString());
					keep.cdata = true;
					keep.pcdata = true;
					actualElement.addChild(keep);
					break;
				}
				result.append(cChar);
				counter++;
				if (counter > 5 && !checkedCDATA){
					checkedCDATA = true;
					if (!result.toString().toUpperCase().equals("CDATA["))
						throw new RuntimeException(
							"Illegal use of <![. " + 
							"These operators are used to start a CDATA section. <![CDATA[]]>" +
							" Line:" + line
						);
					result = new StringBuffer();
				}
			}

			if ((char) toParse.read() != ']')
				throw new RuntimeException("Wrong Syntax at the end of a CDATA section <![CDATA[]]> Line:"+line);
			if ((char) toParse.read() != '>')
				throw new RuntimeException("Wrong Syntax at the end of a CDATA section <![CDATA[]]> Line:"+line);

			//XMLElement keep = new XMLElement(sTagName,attributes);
			//actualElement.addChild(keep);
			//if(oChar != '/')actualElement = keep;
			return toParse;
		}
		XMLElement xmlElement;
		public void run(){
			xmlElement = parseDocument(document);
			
			if(xmlHandler == null)  return;
			
			try{
				xmlEventMethod.invoke(xmlHandler, new Object[] {xmlElement});
			}catch (IllegalAccessException e){
				// TODO Auto-generated catch block
				e.printStackTrace();
			}catch (InvocationTargetException e){
				// TODO Auto-generated catch block
				e.printStackTrace();
			}catch(NullPointerException e){
				throw new RuntimeException("You need to implement the xmlEvent() function to handle the loaded xml files.");
			}
		}

	}

	/**
	 * XML document start
	 */
	private static final String docStart = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";

	/**
	 * the parent element to put the children elements in while parsing
	 */
	private XMLElement actualElement;

	/**
	 * the result element for loading a document
	 */
	private XMLElement result;

	/**
	 * Parent PApplet instance
	 */
	private final PApplet pApplet;
	private final Object parent;

	/**
	 * Method to call when xml is loaded
	 */
	private Method xmlEventMethod;

	/**
	 * Initializes a new XMLInOut Object for loading and saving XML files. If you give
	 * the constructor only a reference to your application it looks for the xmlEvent
	 * method in your sketch. If you also give it a reference to a further object it
	 * calls the xmlEvent method in this object.
	 * @param pApplet PApplet, the Applet proXML is running in
	 */
	public XMLInOut(final PApplet pApplet){
		this.pApplet = pApplet;
		parent = pApplet;

		try{
			xmlEventMethod = pApplet.getClass().getMethod("xmlEvent", new Class[] {XMLElement.class});
		}catch (Exception e){
			
		}
	}

	/**
	 * @param pApplet PApplet, the Applet proXML is running in
	 * @param i_parent Object, the object that contains the xmlEvent function
	 */
	public XMLInOut(final PApplet pApplet, final Object i_parent){
		this.pApplet = pApplet;
		parent = i_parent;

		try{
			xmlEventMethod = i_parent.getClass().getMethod("xmlEvent", new Class[] {XMLElement.class});
		}catch (Exception e){
		
		}
	}

	/**
	 * Modified openStream Method from PApplet.
	 * @param filename
	 * @return InputStream
	 */
	private InputStream openStream(String filename){
		InputStream stream = null;

		try{
			URL url = new URL(filename);
			stream = url.openStream();
			return stream;

		}catch (MalformedURLException e){
			// not a url, that's fine

		}catch (IOException e){
			throw new RuntimeException("Error downloading from URL " + filename);
		}

		// if not online, check to see if the user is asking for a file
		// whose name isn't properly capitalized. this helps prevent issues
		// when a sketch is exported to the web, where case sensitivity
		// matters, as opposed to windows and the mac os default where
		// case sensitivity does not.
		if (!pApplet.online){
			try{
				// first see if it's in a data folder
				File file = new File(pApplet.sketchPath + File.separator + "data", filename);
				if (!file.exists()){
					// next see if it's just in this folder
					file = new File(pApplet.sketchPath, filename);
				}
				if (file.exists()){
					try{
						String path = file.getCanonicalPath();
						String filenameActual = new File(path).getName();
						// if the actual filename is the same, but capitalized
						// differently, warn the user. unfortunately this won't
						// work in subdirectories because getName() on a relative
						// path will return just the name, while 'filename' may
						// contain part of a relative path.
						if (filenameActual.equalsIgnoreCase(filename) && !filenameActual.equals(filename)){
							throw new RuntimeException("This file is named " + filenameActual + " not " + filename + ".");
						}
					}catch (IOException e){
					}
				}

				// if this file is ok, may as well just load it
				stream = new FileInputStream(file);
				if (stream != null)
					return stream;

				// have to break these out because a general Exception might
				// catch the RuntimeException being thrown above
			}catch (IOException ioe){
			}catch (SecurityException se){
			}
		}

		try{
			// by default, data files are exported to the root path of the jar.
			// (not the data folder) so check there first.
			stream = pApplet.getClass().getResourceAsStream(filename);
			if (stream != null)
				return stream;

			// hm, check the data subfolder
			stream = pApplet.getClass().getResourceAsStream("data/" + filename);
			if (stream != null)
				return stream;

			// attempt to load from a local file, used when running as
			// an application, or as a signed applet
			try{ // first try to catch any security exceptions
				try{
					File file = new File(pApplet.sketchPath, filename);
					stream = new FileInputStream(file);
					if (stream != null)
						return stream;

				}catch (Exception e){
				} // ignored

				try{
					stream = new FileInputStream(new File("data", filename));
					if (stream != null)
						return stream;
				}catch (IOException e2){
				}

				try{
					stream = new FileInputStream(filename);
					if (stream != null)
						return stream;
				}catch (IOException e1){
				}

			}catch (SecurityException se){
			} // online, whups

			if (stream == null){
				throw new IOException("openStream() could not open " + filename);
			}
		}catch (Exception e){
		}
		return null; // #$(*@ compiler
	}

	/**
	 * Use this method to load an xml file. If the given String is xml it is
	 * directly parsed and converted to a XMLElement. Be aware that it has to
	 * start with &quot;&lt;?xml&quot to be detected as xml.
	 * If you call the function with an url the according file is loaded. You 
	 * can load xml files from your harddisk or the internet. Both works in
	 * an application if you export it as an applet it is not possible to 
	 * directly load xml from external sources, because of java security resctictions.
	 * If you want to load external sources you have to use an application on
	 * the serverside that passes the file to your applet. You will find
	 * examples using php in the processing forum.
	 * 
	 * To handle the loaded XML File you have to implement the method xmlEvent(XMLElement element).
	 * If the xml file is loaded it is send to this method.
	 * 
	 * @param documentUrl String, url from where the Element has to be loaded
	 * @example proxml
	 * @shortdesc Loads the XMLElement from a given path.
	 * @related XMLInOut
	 * @related loadElementFrom ( )
	 * @related saveElement ( )
	 */
	public void loadElement(final String documentUrl){

		Thread loader;
		if (documentUrl.startsWith("<?xml")){
			loader = new Thread(new Loader(new StringReader(documentUrl),parent));
		}else{
			try{
				InputStream test = openStream(documentUrl);
				loader = new Thread(new Loader(new BufferedReader(new InputStreamReader(test)),parent));
			}catch (Exception e){
				throw new RuntimeException("proXML was not able to load the given xml-file: " + documentUrl + " Please check if you have entered the correct url.");
			}
		}
		try{
			loader.start();
		}catch (Exception e){
			throw new RuntimeException("proXML was not able to read the given xml-file: " + documentUrl + " Please make sure that you load a file that contains valid xml.");
		}
	}
	
	/**
	 * Use this method to load an xml file. If the given String is xml it is
	 * directly parsed and converted to a XMLElement. Be aware that it has to
	 * start with &quot;&lt;?xml&quot to be detected as xml.
	 * If you call the function with an url the according file is loaded. You 
	 * can load xml files from your harddisk or the internet. Both works in
	 * an application if you export it as an applet it is not possible to 
	 * directly load xml from external sources, because of java security resctictions.
	 * If you want to load external sources you have to use an application on
	 * the serverside that passes the file to your applet. You will find
	 * examples using php in the processing forum.
	 * 
	 * @param documentUrl String, url from where the Element has to be loaded
	 * @example proxml_loadElementFrom
	 * @shortdesc Loads the XMLElement from a given path.
	 * @related XMLInOut
	 * @related loadElementFrom ( )
	 * @related saveElement ( )
	 */
	public XMLElement loadElementFrom(final String documentUrl){
		Loader loader;
		if (documentUrl.startsWith("<?xml")){
			loader = new Loader(new StringReader(documentUrl),null);
		}else{
			try{
				InputStream test = openStream(documentUrl);
				loader = new Loader(new BufferedReader(new InputStreamReader(test)),null);
			}catch (Exception e){
				throw new RuntimeException("proXML was not able to load the given xml-file: " + documentUrl + " Please check if you have entered the correct url.");
			}
		}
		try{
			loader.run();
			return loader.xmlElement;
		}catch (Exception e){
			throw new RuntimeException("proXML was not able to read the given xml-file: " + documentUrl + " Please make sure that you load a file that contains valid xml.");
		}
	}

	/**
	 * Saves the XMLElement to a given Filename.
	 * 
	 * @param documentUrl String, url to save the XMLElement as XML File 
	 * @example proxml
	 * @shortdesc Saves the XMLElement to a given File.
	 * @related XMLInOut
	 * @related loadElement ( )
	 * @usage Application
	 */
	public void saveElement(final XMLElement xmlElement, String filename){
		try{
			File file;
			if (!pApplet.online){
				file = new File(pApplet.sketchPath + File.separator + "data", filename);
				System.out.println(pApplet.sketchPath + File.separator + "data");
				if (!file.exists()){
					final String parent = file.getParent();

					if (parent != null){
						File unit = new File(parent);
						if (!unit.exists())
							unit.mkdirs();
					}
				}
			}else{
				file = new File(pApplet.getClass().getResource("data/" + filename).toURI());
			}

			PrintWriter output = new PrintWriter(new FileOutputStream(file));
			output.println(docStart);
			xmlElement.printElementTree(output, "  ");
			output.flush();
			output.close();
		}catch (Exception e){
			e.printStackTrace();
			System.out.println(pApplet.sketchPath + File.separator + "data");
			System.out.println("You cannot write to this destination. Make sure destionation is a valid path");
		}
	}

	/**
	 * The following methods are for parsing the XML Files
	 */

}
