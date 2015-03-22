class P5XBoxController{
  
  float xPos,yPos;
  
  float initialX=0.0;
  float initialY=0.0;
  float initialZ=0.0;
  
  float multX=0.1;
  float multY=0.1;
  float multZ=0.1;
 
  
  float initialP=0.0;
  float initialYaw=0.0;
  float initialR=0.0;
  
  float multP=1.0;
  float multYaw=1.0;
  float multR=1.0;
  
  boolean bPressedButton;
   
  ControllDevice joypad;

  ControllSlider slider1;
  ControllSlider slider2;
  ControllSlider slider3;
  ControllSlider slider4;

  ControllButton button;
  
  slider vectorX;
  slider vectorY;
  slider vectorZ;
  slider vectorW;

  
  void setup(ControllIO controll, int num){
   
  xPos=0;
  yPos=0;  
  
  joypad = controll.getDevice(num);

  println( joypad.getNumberOfButtons() );
  println( joypad.getNumberOfSticks() );
  println( joypad.getNumberOfSliders() );


  slider1=joypad.getSlider(0);
  slider2=joypad.getSlider(1);
  slider3=joypad.getSlider(2);
  slider4=joypad.getSlider(3);
  
  button=joypad.getButton(0);

  vectorX=new slider(100,20);
  vectorY=new slider(220,20);
  vectorZ=new slider(340,20);
  vectorW=new slider(460,20);
  
  bPressedButton=false;

  }  
 
 
 void draw(){
 
  if (button.getValue()>0)
    bPressedButton=true;
  else
    bPressedButton=false;
   
  vectorX.value= (slider1.getValue()+1.0)*50.0;
  vectorY.value= (slider2.getValue()+1.0)*50.0;
  vectorZ.value= (slider3.getValue()+1.0)*50.0;
  vectorW.value= (slider4.getValue()+1.0)*50.0;

  if (abs(slider2.getValue())>0.15)
    initialX-=slider2.getValue() * multX;

  if (abs(slider1.getValue())>0.15)
    initialY-=slider1.getValue() * multY;
    
  //set upright when right Stick pushes upright
  if (slider3.getValue()<-0.5){
    initialR=0;
  }

  if (slider3.getValue()>0.5){
    initialR=180;
  }

  if (slider4.getValue()>0.5){
    initialR=270;
  }

  if (slider4.getValue()<-0.5){
    initialR=90;
  }

  //initialR=sin(slider4.getValue()/2.0)*PI*60;
  if (slider3.getValue()>0.0){
    initialR=sin(slider4.getValue()/2.0)*PI*60;
    initialR-=cos(slider3.getValue()/2.0)*PI*60;
  }
  else{
    initialR=-sin(slider4.getValue()/2.0)*PI*60;
    initialR-=180+cos(slider3.getValue()/2.0)*PI*60;
  }
  
  //deadzone for rotation!
  if (abs(slider3.getValue())+abs(slider4.getValue())<0.2)
    initialR=0;
  pushMatrix();
  translate(xPos,yPos);
  vectorX.draw();
  vectorY.draw();
  vectorZ.draw();
  vectorW.draw();

  if (bPressedButton)
    fill(204, 102, 0);
  else  
    fill(255);
  
  rect(10,10,50,50);
  popMatrix();
 
 }
  
}
