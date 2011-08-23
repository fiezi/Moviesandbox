import procontroll.*;
import java.io.*;

ControllIO controll;
ControllDevice device;
ControllSlider sliderX;
ControllSlider sliderY;
ControllButton button;

void setup(){
  size(400,400);
  
  controll = ControllIO.getInstance(this);

  device = controll.getDevice("Logitech RumblePad 2 USB");
  device.setTolerance(0.05f);
  
  sliderX = device.getSlider("X-Achse");
  sliderY = device.getSlider("Y-Achse");
  
  button = device.getButton("Taste 0");
  
  fill(0);
  rectMode(CENTER);
}

void draw(){
  background(255);
  
  if(button.pressed()){
    fill(255,0,0);
  }else{
    fill(0);
  }
  
  float x = sliderX.getTotalValue() + width/2;
  float y = sliderY.getTotalValue() + height/2;
  
  if(x > width + 20 || x < - 20 || y > height + 20 || y < - 20){
    sliderX.reset();
    sliderY.reset();
  }
  
  rect(x,y,20,20);
}
