/*
Tis example loads an xml file, loads the attributes
of the rootnode and prints out their values.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
  String[] attributes = element.getAttributes();
  for(int i = 0; i < attributes.length; i++){
    println(attributes[i]+":"+
      element.getAttribute(attributes[i]));
  }
}

void draw(){
}
