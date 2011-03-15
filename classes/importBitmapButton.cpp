

#include "importBitmapButton.h"
#include "renderer.h"
#include "input.h"
#include "drawingWidget.h"
#include "meshData.h"
#include "drawTool.h"

ImportBitmapButton::ImportBitmapButton(){

buttonProperty="NULL";
color=Vector4f(0.5,0.5,0.5,1.0);

listWidth=200;
listColor=Vector4f(0.4,0.4,0.4,1.0);
listShader="color";
}

ImportBitmapButton::~ImportBitmapButton(){}

void ImportBitmapButton::mouseDrag(){}

void ImportBitmapButton::clickedLeft(){

    input->deselectButtons(0);
    bDrawName=true;

    input->getAllImages();

    MsbObject * myParent=parent;

    LoadButton::clickedLeft();

    parent=myParent;

    bDrawName=false;
}

void ImportBitmapButton::trigger(MsbObject*other){

ListButton::trigger(other);
if (other==scrollBar)
    return;

if (sceneData->brush->drawing){
    //clean up particleSystem
    sceneData->brush->drawing->drawType=4;
    for (int i=sceneData->vboList[sceneData->brush->drawing->vboMeshID]->vData.size()-1;i>-1;i--)
        sceneData->brush->drawing->deleteParticle(i);
    loadFile(other->name);
    sceneData->brush->drawing->sceneShaderID="drawing";
    }
input->deselectButtons(0);
}

void ImportBitmapButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<input->userImages.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(input->userImages[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_flat");
      }
}

void ImportBitmapButton::placeScrollBar(){

    scrollBar->setLocation(Vector3f(location.x-17, location.y- scale.y -( maxListItems * listButton[0]->scale.y ), 0 ) );
    scrollBar->bFlipValue=true;

}

void ImportBitmapButton::loadFile(string filename){


cout << "loading image " << filename << endl;
string fullName="resources/images/";
fullName.append(filename);
FIBITMAP * myBitmap = FreeImage_Load(FIF_TARGA,fullName.c_str(),0);
int imageWidth=FreeImage_GetWidth(myBitmap);
int imageHeight=FreeImage_GetHeight(myBitmap);
//never used:
//int channels=FreeImage_GetBPP(myBitmap)/8;
if (input->controller->tool!=TOOL_DRAW)
	input->controller->switchTool(TOOL_DRAW);
//aquire brush if existing
assembleImage(myBitmap,imageWidth, imageHeight, 1.0f);
FreeImage_Unload(myBitmap);

//this is for backside loading!!!
    if (1){
        fullName="resources/images/";

        int pos = filename.find(".tga");    // position of ".tga" in str
        string str = filename.substr(0,pos);   // get until ".tga" to the end
        str.append("_back.tga");
        fullName.append(str);
        //fullName.append("asian_back.tga");
        myBitmap = FreeImage_Load(FIF_TARGA,fullName.c_str(),0);
        imageWidth=FreeImage_GetWidth(myBitmap);
        imageHeight=FreeImage_GetHeight(myBitmap);

        assembleImage(myBitmap,imageWidth, imageHeight, -1.0f);
        FreeImage_Unload(myBitmap);
    }

}


void ImportBitmapButton::assembleImage(FIBITMAP* myBitmap, int imageWidth, int imageHeight, float flip){


    sceneData->brush->bMouseControlled=false;
    //go through all of the image
    for (int h=0;h<imageHeight;h++){
    //reset after every line
        for (int w=0;w<imageWidth;w++){

            RGBQUAD *myColor=new RGBQUAD;

            FreeImage_GetPixelColor(myBitmap,w,h,myColor);
            sceneData->brush->color.r=(float)myColor->rgbRed/255.0f;
            sceneData->brush->color.g=(float)myColor->rgbGreen/255.0f;
            sceneData->brush->color.b=(float)myColor->rgbBlue/255.0f;

            Vector3f bLoc;
            bLoc.x=sceneData->brush->drawing->location.x- 10 * (float) ((float)imageWidth/2.0 - (float)w)/(float)imageWidth;
            bLoc.y=sceneData->brush->drawing->location.y+ 10 *  (float)h/(float)imageHeight;
            bLoc.z=sceneData->brush->drawing->location.z- flip * (float)myColor->rgbReserved/32.0f;
            sceneData->brush->setLocation(bLoc);
            //flip y and z when shift down
            if (input->bShiftDown){
                bLoc.z=sceneData->brush->drawing->location.z+ 10 *  (float)h/(float)imageHeight;
                bLoc.y=sceneData->brush->drawing->location.y+ flip * (float)myColor->rgbReserved/32.0f;
                sceneData->brush->setLocation(bLoc);
            }

            Vector3f xyzNormal;

            if ((int)myColor->rgbReserved>0){

                input->mouseVector=Vector3f(0.1,0.1,0.1);   //HACK so that we actually paint something!
                sceneData->brush->setLocation(sceneData->brush->location*sceneData->brush->drawing->scale);
				input->mouse3D=sceneData->brush->location;
                xyzNormal=genNormal(myBitmap,w,h,imageWidth,imageHeight,flip);
                xyzNormal+=genNormal(myBitmap,w+1,h,imageWidth,imageHeight,flip);
                xyzNormal+=genNormal(myBitmap,w-1,h,imageWidth,imageHeight,flip);
                xyzNormal+=genNormal(myBitmap,w,h+1,imageWidth,imageHeight,flip);
                xyzNormal+=genNormal(myBitmap,w,h-1,imageWidth,imageHeight,flip);
                xyzNormal.normalize();
                sceneData->brush->pNormal=xyzNormal;

                ((DrawTool*)input->controller->currentTool)->paint();

            ///fill holes
/*
                float myDepth=(float)myColor->rgbReserved;
                float fillStep=1.0;

                //find lowest fillDepth around us
                FreeImage_GetPixelColor(myBitmap,w-1,h,myColor);
                float fillDepth =(float)myColor->rgbReserved;

                FreeImage_GetPixelColor(myBitmap,w+1,h,myColor);
                float fillDepthRight =(float)myColor->rgbReserved;
                if (fillDepthRight<fillDepth)
                    fillDepth=fillDepthRight;

                FreeImage_GetPixelColor(myBitmap,w,h-1,myColor);
                float fillDepthDown =(float)myColor->rgbReserved;
                if (fillDepthDown<fillDepth)
                    fillDepth=fillDepthDown;

                FreeImage_GetPixelColor(myBitmap,w,h+1,myColor);
                float fillDepthUp =(float)myColor->rgbReserved;
                if (fillDepthUp<fillDepth)
                    fillDepth=fillDepthUp;

                if (fillDepth + fillStep/32.0f <myDepth){
                    int i=1;
                    while (fillDepth<myDepth){
                        sceneData->brush->setLocation(sceneData->brush->location+ Vector3f(0,0,fillStep/32.0f));
                        input->mouse3D=sceneData->brush->location;
						 ((DrawTool*)input->controller->currentTool)->paint();
                        fillDepth+=fillStep;
                        i++;
                    }
                }

        ///end fill holes
*/
                //flip y and z when shift down
                if (input->bShiftDown){
                    float interim=xyzNormal.y;
                    xyzNormal.y=xyzNormal.z;
                    xyzNormal.z=interim;
                }

            sceneData->brush->pNormal=xyzNormal;

            }//filling holes
        }//inner for loop
    }//outer for loop

    sceneData->brush->bMouseControlled=true;
}

Vector3f ImportBitmapButton::genNormal(FIBITMAP* myBitmap, int w, int h, int width, int height, float flip){

        //remember folks - the image is flipped!

        Vector4f influences=Vector4f(0,0,0,0);

        RGBQUAD *myColor=new RGBQUAD;

        Vector3f myNormal;

        FreeImage_GetPixelColor(myBitmap,w,h,myColor);
        float myInfluence=(float)myColor->rgbReserved;
        if (myInfluence==0){
            //cout << "bad Normal!!!" << endl;
            return Vector3f(0.0,0.0,0.0);
        }
          //Gathering influences
        if (w>=2 && w<width-2 && h>=2 && h<height-2){

            FreeImage_GetPixelColor(myBitmap,w-1,h,myColor);
            influences.x=(int)myColor->rgbReserved;

            FreeImage_GetPixelColor(myBitmap,w+1,h,myColor);
            influences.z=(int)myColor->rgbReserved;


            FreeImage_GetPixelColor(myBitmap,w,h+1,myColor);
            influences.y=(int)myColor->rgbReserved;


            FreeImage_GetPixelColor(myBitmap,w,h-1,myColor);
            influences.w=(int)myColor->rgbReserved;
        }


        float xN,yN,zN, falloffN;

        falloffN= 16.0;
        //edges
        if (influences.x==0){
            myNormal.x+=-1.0;
            myNormal.z=0.0;
        }

        if (influences.z==0){
            myNormal.x+=1.0;
            myNormal.z=0.0;
        }

        if (influences.y==0){
            myNormal.y+=1.0;
            myNormal.z=0.0;
        }

        if (influences.w==0){
            myNormal.y+=-1.0;
            myNormal.z=0.0;
        }

        if (myNormal!=Vector3f(0,0,0)){
            return myNormal;
        }

        //make linear, although we know it's wrong... it looks good :-)
        xN = (influences.x-influences.z)/falloffN;
        yN = (influences.w-influences.y)/falloffN;
        zN = -max(0.0,1.0-xN-yN);


        myNormal=Vector3f(xN,yN,zN * flip);

        myNormal.normalize();
        return myNormal;
}

void ImportBitmapButton::create(){sceneData->addButton(this);}
