/*
This example loads a xml file and prints it.
 */

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);

  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  element.printElementTree();
}

void draw(){
}
