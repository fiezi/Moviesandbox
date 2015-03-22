import processing.serial.*;

/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
import processing.serial.*;
 
 Serial myPort;        // The serial port
 float inByte; 
 int count=0; 

OscP5 oscP5;
NetAddress myRemoteLocation;

slider vectorX;
slider vectorY;
slider vectorZ;

RampUpDown[] myRamps=new RampUpDown[12];

boolean bUseArduino=false;

//prefix is the indicator for the sensor number
int prefix1 = 1;
//flag for the end of the signal
boolean myCatch1 = true;



int ring=0;
 

void setup() {
  size(600,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  //we dont really need to listen, so we leave at port 12000
  oscP5 = new OscP5(this,12000);
  myRemoteLocation = new NetAddress("192.168.2.102",4444);  
  OscMessage myMessage = new OscMessage(Integer.toString(count));

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}

 
void draw() {
  background(64,128,128);  

}

void keyReleased(){
  
  OscMessage myMessage = new OscMessage("Testing...");

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 

}

void mouseReleased() {
  count++;
  if (count >3)
    count=1;
  OscMessage myMessage = new OscMessage(Integer.toString(count));

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 

}



