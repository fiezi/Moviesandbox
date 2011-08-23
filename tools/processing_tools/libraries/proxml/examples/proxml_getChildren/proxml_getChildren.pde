/*
This example loads an xml file and 
prints out the children of the root element
*/

import proxml.*;

XMLInOut xmlIO; 

void setup(){
  size(400,400);

  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  XMLElement[] children = element.getChildren();

  for(int i = 0; i < children.length;i++){
    XMLElement child = children[i];
    println(child);
  }
}

void draw(){
}
