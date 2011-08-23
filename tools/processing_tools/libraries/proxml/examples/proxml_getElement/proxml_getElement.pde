/*
This example loads a xml file and prints out the name of the root element.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.getElement());
}

void draw(){
}
