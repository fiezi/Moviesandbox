/*
This example loads an xml file and 
checks if the first child of the root element
is text
*/

import proxml.*;

XMLInOut xmlIO; 

void setup(){
  size(400,400);

  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("my-xml-file.xml");
}

void xmlEvent(XMLElement element){
println(element.firstChild().isTextElement());
}

void draw(){
}

