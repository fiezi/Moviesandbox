package proxml.test;
import processing.core.PApplet;
import processing.core.PImage;
import proxml.*;

public class Test extends PApplet{

	//	to store the background after painting an ellipse
	PImage back;

	//	xml element to store and load the drawn ellipses
	XMLElement ellipses;
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
			xmlInOut.loadElement("weather.xml");
			ellipses.printElementTree(" ");
		}catch (InvalidDocumentException ide){
			println("File does not exist");
		}

		//initialise PImage for background
		back = new PImage(width, height);
		loadPixels();
		back.pixels = pixels;
	}

	public void draw(){
	}

	public static void main(String[] args){
		PApplet.main(new String[] {Test.class.getName()});
	}

}