class slider{

  float xPos,yPos;
  float value=25;
  
  slider(float x, float y){
  xPos=x;
  yPos=y;  
  }
  
  void draw(){
  
    //check mouse Input
    if (mouseX>xPos && mouseX<xPos+30)
      if (mouseY>yPos && mouseY<yPos+100)
        if (mousePressed)
          value=mouseY-yPos;
          
    stroke(255);
    fill(255);
    color(255);
    rect(xPos,yPos,30,100);
    color(200,0,0);
    fill(200,0,0);
    rect(xPos,yPos+value, 10,10);
   }

};
