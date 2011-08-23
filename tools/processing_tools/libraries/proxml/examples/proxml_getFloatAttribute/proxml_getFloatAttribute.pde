import proxml.*;

//to store the background after painting an ellipse
PImage back;

//xml element to store and load the drawn ellipses
final XMLElement ellipses = new XMLElement("ellipses");

int xPos = 0;
int yPos = 0;

void setup(){
  size(400,400);
  smooth();
  background(255);
  
  //load ellipses from file if it exists
  try{
    //folder is a field of PApplet giving 
    //you the path to your sketch folder
    ellipses.loadFrom(folder+"/ellipse.xml"); 
    initEllipses();
  }catch(InvalidDocumentException ide){
    println("File does not exist");
  }
  
  //initialise PImage for background
  back = new PImage(width,height);
  loadPixels();
  back.pixels = pixels;
}

void draw(){
}

//draw all ellipses saved in the xml file
void initEllipses(){
  XMLElement[] children = ellipses.getChildren();
  XMLElement ellipse;
  XMLElement position;
  XMLElement size;
  for(int i = 0; i < ellipses.countChildren();i++){
    ellipse = ellipses.getChild(i);
    position = ellipse.getChild(0);
    size = ellipse.getChild(1);
    ellipse(
      position.getFloatAttribute("xPos"),
      position.getFloatAttribute("yPos"),
      size.getFloatAttribute("xSize"),
      size.getFloatAttribute("ySize")
    );
  }
}

void mousePressed(){
  xPos = mouseX;
  yPos = mouseY;
}

void mouseDragged(){
  background(back);
  ellipse(xPos,yPos,abs(xPos-mouseX),abs(yPos-mouseY));
}

void mouseReleased(){
  XMLElement ellipse = new XMLElement("ellipse");
  ellipses.addChild(ellipse);
  XMLElement position = new XMLElement("position");
  position.addAttribute("xPos",xPos);
  position.addAttribute("yPos",yPos);
  ellipse.addChild(position);
  XMLElement size = new XMLElement("size");
  size.addAttribute("xSize",abs(xPos-mouseX));
  size.addAttribute("ySize",abs(yPos-mouseY));
  ellipse.addChild(size);
  ellipses.saveTo(folder+"/ellipse.xml");
  loadPixels();
  back.pixels = pixels;
}
