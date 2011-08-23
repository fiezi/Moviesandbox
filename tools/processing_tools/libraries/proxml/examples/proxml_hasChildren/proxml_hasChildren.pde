/*
This example loads an xml file and 
 checks if it has children
 */

import proxml.*;

XMLInOut xmlIO; 

void setup(){
  size(400,400);

  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.hasChildren());
}

void draw(){
}
