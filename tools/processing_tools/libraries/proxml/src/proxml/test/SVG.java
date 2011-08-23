package proxml.test;
import processing.core.PApplet;
import processing.core.PImage;
import proxml.*;

public class SVG extends PApplet{

	//	xml element to store and load the drawn ellipses
	XMLElement svg;
	XMLInOut xmlInOut;

	int xPos = 0;

	int yPos = 0;

	public void setup(){
		size(400, 400);
		smooth();
		background(255);

		//load ellipses from file if it exists
		try{
			//folder is a field of PApplet 
			//giving you the path to your sketch folder
			xmlInOut = new XMLInOut(this);
			xmlInOut.loadElement("svg.xml");
			
		}catch (InvalidDocumentException ide){
			ide.printStackTrace();
			println("File does not exist");
		}
	}
	
	public void xmlEvent(XMLElement i_element){
		svg = i_element;
		svg.printElementTree(" ");
		println(svg.getChild(0).countChildren());
	}

	public void draw(){
	}

	public static void main(String[] args){
		PApplet.main(new String[] {SVG.class.getName()});
	}

}