/*
This example loads a xml file and prints it.
*/

import proxml.*;

XMLInOut xmlIO;

void setup(){
  size(400,400);
  
  xmlIO = new XMLInOut(this);
  xmlIO.loadElement("weather.xml");
}

void xmlEvent(XMLElement xmlElement){
  
}
