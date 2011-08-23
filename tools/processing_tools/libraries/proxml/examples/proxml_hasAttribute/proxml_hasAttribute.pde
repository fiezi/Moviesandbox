/*
This example loads a xml file and checks if the root node 
has the attribute "attribute"
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.hasAttribute("attribute"));
}

void draw(){
}
