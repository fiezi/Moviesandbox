/*
this is a more complex example demonstrating the possibilities
of proxml. In the example the user can draw ellipses that are saved
into a xml file. This file is loaded on the next start of the program.
If there exists an xml file it is loaded and the
content is drawn as ellipses. Otherwise the xml element is created.
*/

import proxml.*;

//to store the background after painting an ellipse
PImage back;

//xml element to store and load the drawn ellipses
XMLElement ellipses;
XMLInOut xmlInOut;

int xPos = 0;
int yPos = 0;

void setup(){
  size(400,400);
  smooth();
  background(255);

  //load ellipses from file if it exists
  xmlInOut = new XMLInOut(this);
  try{
    xmlInOut.loadElement("ellipse.xml"); 
  }catch(Exception e){
    //if the xml file could not be loaded it has to be created
    xmlEvent(new XMLElement("ellipses"));
  }
}

void xmlEvent(XMLElement element){
  ellipses = element;
  initEllipses();
    //initialise PImage for background
  back = new PImage(width,height);
  loadPixels();
  back.pixels = pixels;
}

void draw(){
}

//draw all ellipses saved in the xml file
void initEllipses(){
  ellipses.printElementTree(" ");
  XMLElement ellipse;
  XMLElement position;
  XMLElement size;
  
  for(int i = 0; i < ellipses.countChildren();i++){
    ellipse = ellipses.getChild(i);
    position = ellipse.getChild(0);
    size = ellipse.getChild(1);
    ellipse(
      position.getIntAttribute("xPos"),
      position.getIntAttribute("yPos"),
      size.getFloatAttribute("Xsize"),
      size.getFloatAttribute("Ysize")
    );
  }
}

void mousePressed(){
  xPos = mouseX;
  yPos = mouseY;
}

void mouseDragged(){
  background(back);
  ellipse(xPos,yPos,abs(xPos-mouseX),abs(yPos-mouseY));
}

void mouseReleased(){
  XMLElement ellipse = new XMLElement("ellipse");
  ellipses.addChild(ellipse);
  XMLElement position = new XMLElement("position");
  position.addAttribute("xPos",xPos);
  position.addAttribute("yPos",yPos);
  ellipse.addChild(position);
  XMLElement size = new XMLElement("size");
  size.addAttribute("Xsize",abs(xPos-mouseX));
  size.addAttribute("Ysize",abs(yPos-mouseY));
  ellipse.addChild(size);
  xmlInOut.saveElement(ellipses,"ellipse.xml");
  loadPixels();
  back.pixels = pixels;
}
