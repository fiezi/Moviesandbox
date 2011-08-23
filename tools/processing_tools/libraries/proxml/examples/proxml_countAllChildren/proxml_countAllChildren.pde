/*
This example loads a xml file and counts all elements in the file.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  println(element.countAllChildren());
}

void draw(){
}
