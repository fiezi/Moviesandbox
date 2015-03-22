import procontroll.*;
import net.java.games.input.*;
import java.io.*;

import processing.serial.*;

/**
 * oscP5sendreceive by andreas schlegel
 * oscP5 website at http://www.sojamo.de/oscP5
 */


/**
 * procontroll by Christian Riekoff
 * procontroll website: http://www.creativecomputing.cc/p5libs/procontroll/
*/

import oscP5.*;
import netP5.*;

ControllIO controll;

OscP5 oscP5;
NetAddress myRemoteLocation;

P5XBoxController playerOne, playerTwo;


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
  myRemoteLocation = new NetAddress("127.0.0.1",31841);
  
  
  playerOne=new P5XBoxController();

  //playerTwo=new P5XBoxController();
  
  controll = ControllIO.getInstance(this);
  controll.printDevices();
  playerOne.setup(controll, 2);
  //playerTwo.setup(controll, 3);
  //playerTwo.yPos=200;
  //playerTwo.initialX=-5;
  
}

void draw() {
  background(128,64,128); 
   playerOne.draw(); 
   //playerTwo.draw();
  sendUDP();
}


void sendUDP() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/pilot/vector3f/vector3f");
  
    myMessage.add(playerOne.initialX); /* add a float to the osc message */
    myMessage.add(playerOne.initialY); /* add a float to the osc message */
    myMessage.add(playerOne.initialZ-0.01); /* add a float to the osc message */
  
  
    myMessage.add(0.0); /* add a float to the osc message */
    myMessage.add(0.0); /* add a float to the osc message */
    myMessage.add(0.0); /* add a float to the osc message */
 
 
 
 
    //myMessage.add(playerTwo.initialX); /* add a float to the osc message */
    //myMessage.add(playerTwo.initialY); /* add a float to the osc message */
    //myMessage.add(playerTwo.initialZ); /* add a float to the osc message */
  
  
    //myMessage.add(0.0); /* add a float to the osc message */
    //myMessage.add(0.0); /* add a float to the osc message */
    //myMessage.add(playerTwo.initialR); /* add a float to the osc message */
 
 
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
  
  if (playerOne.bPressedButtonOne){
    println("sending trigger...");
    myMessage = new OscMessage("/trigger"); 
    myMessage.add("essen");
    oscP5.send(myMessage, myRemoteLocation); 
  }
  if (playerOne.bPressedButtonTwo){
    println("sending trigger...");
    myMessage = new OscMessage("/trigger"); 
    myMessage.add("kacken");
    oscP5.send(myMessage, myRemoteLocation); 
  }

  /*
  if (playerTwo.bPressedButton){
    println("sending trigger...");
    myMessage = new OscMessage("/trigger"); 
    myMessage.add("boxBaer");
    oscP5.send(myMessage, myRemoteLocation); 
  }
  */
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
