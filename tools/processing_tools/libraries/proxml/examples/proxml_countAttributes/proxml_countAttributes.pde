/*
This example loads an xml file and prints the number of
attributes of the root node.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.countAttributes());
}

void draw(){
}
