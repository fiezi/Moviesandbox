class RampUpDown{
  
float speed=0.3;
float currentTime=0.0;
float alphaValue=0.0;
boolean bActive=false;

  void update(){
    //this can be set via trigger
    if (bActive){
      //fade in and out for alpha
      alphaValue=sin(currentTime);
      currentTime+=speed;
      //if faded out, reset timer and set to inactive
      if (alphaValue<0.0){
        alphaValue=0.0;
        currentTime=0.0;
        bActive=false;
      }
    }
    ellipse(mouseX,mouseY,alphaValue*100.0,alphaValue*100.0);  
  }

}


