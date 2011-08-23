/*
This example loads a xml file and prints out the root node.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  String root = element.toString();
  println(root);
}

void draw(){
}
