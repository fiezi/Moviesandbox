/*
This example loads a xml file and prints out the parent of the first child
of the root node which is the root node itself
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.firstChild().getParent());
}

void draw(){
}
