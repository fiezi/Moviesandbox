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
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("192.168.2.102",4444);
  
 // List all the available serial ports
 //println(Serial.list());
 // I know that the first port in the serial list on my mac
 // is always my  Arduino, so I open Serial.list()[0].
 // Open whatever port is the one you're using.
 //myPort = new Serial(this, Serial.list()[0], 9600);
 // don't generate a serialEvent() unless you get a newline character:
 //myPort.bufferUntil('\n');
  for (int i=0;i<12;i++)
    myRamps[i]=new RampUpDown();
}

 
void draw() {
  background(64,128,128);  
  
  for (int i=0;i<12;i++)
    myRamps[i].update();

mouseReleased();
}

void keyPressed(){


}


void mousePressed(){
  
myRamps[ring].bActive=true;
ring++;
if (ring==12)
  ring=0;
}

void mouseReleased() {
    OscMessage myMessage = new OscMessage("/pilot/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f");

    for (int i=0; i<12;i++){
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(myRamps[i].alphaValue); /* add a float to the osc message */
    //println(myRamps[0].alphaValue);
    }
 
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
