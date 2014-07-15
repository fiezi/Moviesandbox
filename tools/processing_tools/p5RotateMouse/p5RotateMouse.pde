import oscP5.*;
import netP5.*;

OscP5 moveSend;
OscP5 clickSend;
NetAddress myRemoteLocationRotate;
NetAddress myRemoteLocationClick;

float oldMouseX;
float oldMouseY;
float rotationZ;

int iSmooth=0;
int bufSize=20;

float[] myMouseX=new float[bufSize];
float[] myMouseY=new float[bufSize];

float mouseVecX;
float mouseVecY;

float mouseVecXNormal= 0.0;
float mouseVecYNormal= 0.0;

RampUpDown[] myRamps=new RampUpDown[12];
int ring=0;


void setup() {
  size(1440, 1024);
  frameRate(25);

  moveSend = new OscP5(this, 12000);
  clickSend = new OscP5(this, 14000);

  myRemoteLocationRotate = new NetAddress("192.168.0.2", 31845);
  myRemoteLocationClick = new NetAddress("192.168.0.2",31842);

  oldMouseX=0;
  oldMouseY=0;

  for (int i=0;i<bufSize;i++){
    myMouseX[i]=0.0;
    myMouseY[i]=0.0;
  }
  
  //click from here

  for (int i=0;i<12;i++)
    myRamps[i]=new RampUpDown();
  
  
}

void draw() {
  background(128,128,64);  


  getMouseDelta();


  float mouseXBuffered=0.0;
  float mouseYBuffered=0.0;
  
  for (int i=0;i<bufSize;i++){
    mouseXBuffered+=myMouseX[i]/float(bufSize);
    mouseYBuffered+=myMouseY[i]/float(bufSize);
  }
  
  pushMatrix();
  translate(mouseXBuffered,mouseYBuffered);
  rect(0,0,30,30);
  popMatrix();

  pushMatrix();
  translate(100, 100);
  rotate(-rotationZ);
  rect (0, 0, 55, 55);
  popMatrix();

  for (int i=0;i<12;i++)
    myRamps[i].update();

  sendOSC();

}


void getMouseDelta(){
  
  myMouseX[iSmooth]=mouseX;
  myMouseY[iSmooth]=mouseY;
  
  float mouseXBuffered=0.0;
  float mouseYBuffered=0.0;
  
  for (int i=0;i<bufSize;i++){
    mouseXBuffered+=myMouseX[i]/float(bufSize);
    mouseYBuffered+=myMouseY[i]/float(bufSize);
  }
  
  mouseVecX=mouseXBuffered-oldMouseX;
  mouseVecY=mouseYBuffered-oldMouseY;

  oldMouseX=mouseXBuffered;
  oldMouseY=mouseYBuffered;
  
  if (abs(mouseVecX)+abs(mouseVecY)>0) {

    mouseVecXNormal=mouseVecX/(abs(mouseVecX)+abs(mouseVecY));
    mouseVecYNormal=mouseVecY/(abs(mouseVecX)+abs(mouseVecY));
  }

  rotationZ+=(abs(mouseVecX)+abs(mouseVecY))/100.0;

  iSmooth++;
  iSmooth=iSmooth%bufSize;

}


void mousePressed(){
  
myRamps[ring].bActive=true;
ring++;
if (ring==12)
  ring=0;
}


void sendOSC() {
  /* in the following different ways of creating osc messages are shown by example */
  //  OscMessage myMessage = new OscMessage("/pilot/vector3f/vector3f");
  OscMessage myMessage = new OscMessage("/pilot/vector3f/vector3f/vector3f/vector3f");

  //head
  myMessage.add(0.0); /* add a float to the osc message */
  myMessage.add(0.0); /* add a float to the osc message */
  myMessage.add(rotationZ * 180.0/TWO_PI); /* add a float to the osc message */
//  myMessage.add(vectorZ.value/50.0); /* add a float to the osc message */

  myMessage.add(0.0); /* add a float to the osc message */
  myMessage.add(rotationZ * 180.0/TWO_PI); /* add a float to the osc message */
  myMessage.add(0.0); /* add a float to the osc message */

  myMessage.add(rotationZ * 180.0/TWO_PI); /* add a float to the osc message */
  myMessage.add(0.0); /* add a float to the osc message */
  myMessage.add(0.0); /* add a float to the osc message */

  myMessage.add(0.0); /* add a float to the osc message */
  myMessage.add(rotationZ * 180.0/TWO_PI); /* add a float to the osc message */
  myMessage.add(0.0); /* add a float to the osc message */

  /* send the message */
  moveSend.send(myMessage, myRemoteLocationRotate);

  if ((rotationZ *180.0/TWO_PI)%360>100 &&(rotationZ *180.0/TWO_PI)%360<250){
    myMessage = new OscMessage("/trigger");
    myMessage.add("dark");
    moveSend.send(myMessage, myRemoteLocationRotate);
  }
  else{
    myMessage = new OscMessage("/trigger");
    myMessage.add("bright");
    moveSend.send(myMessage, myRemoteLocationRotate);
  }

  
  
  //click from here
   myMessage = new OscMessage("/pilot/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f/vector4f");

    for (int i=0; i<12;i++){
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(1.0); /* add a float to the osc message */
    myMessage.add(myRamps[i].alphaValue); /* add a float to the osc message */
    }
 
  /* send the message */
  clickSend.send(myMessage, myRemoteLocationClick); 
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}

