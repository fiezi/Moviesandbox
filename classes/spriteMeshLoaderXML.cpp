#include "spriteMeshLoaderXML.h"
#include "renderer.h"
#include "input.h"

SpriteMeshLoaderXML::SpriteMeshLoaderXML(){

    vertexWeights=NULL; //4 weights per vertex, we select the 4 most influential
    boneReference=NULL; //4 bone References per vertex, we select the 4 most influential

    normals=NULL;                               //normal array pointer
    vertices=NULL;                              //vertex array pointer
    texCoords=NULL;                              //vertex array pointer
    renderer=Renderer::getInstance();

    vertexCount=0;

    input=Input::getInstance();
    renderer=Renderer::getInstance();
    sceneData=SceneData::getInstance();
}

SpriteMeshLoaderXML::~SpriteMeshLoaderXML(){
}

bool SpriteMeshLoaderXML::saveSpriteMesh( string filename, SkeletalActor* myDrawing){

    //generate arrays from location

	MeshData* myMesh=sceneData->vboList[myDrawing->vboMeshID];

	vertexCount=myMesh->vData.size();

	vertices=new Vector4f[vertexCount];
	normals=new Vector3f[vertexCount];

	texCoords=new Vector3f[vertexCount];
	colors=new Vector4f[vertexCount];
	secondaryColors=new Vector3f[vertexCount];

	boneReference=new Vector4f[vertexCount];
	vertexWeights=new Vector4f[vertexCount];

    for (int i=0;i<(int)vertexCount;i++){
		vertices[i]=myMesh->vData[i].location;
        normals[i]=myMesh->vData[i].normal;

        texCoords[i]=Vector3f(myMesh->vData[i].texCoord);
        colors[i]=myMesh->vData[i].color;
        secondaryColors[i]=myMesh->vData[i].secondaryColor;

        vertexWeights[i]=myMesh->vData[i].vertexWeights;
        boneReference[i]=myMesh->vData[i].boneReferences;
    }



    //setup XML
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "MSBPointSpriteMesh" );
    doc.LinkEndChild( root );

    ///Vertices

    TiXmlElement * vertexElement=new TiXmlElement("vertices");

    vertexElement->SetAttribute("vertexInterpretation",GL_POINTS);
    vertexElement->SetAttribute("count",vertexCount);

	char xCoord[4 * vertexCount];
	memcpy(xCoord, &vertices[0].x, 4 * vertexCount);

    char value[64];
    std::string allVertices;

    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f %f ",vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].w);
        allVertices+=value;
    }

	TiXmlText * vertexText = new TiXmlText( allVertices );
    vertexElement->LinkEndChild( vertexText );
    root->LinkEndChild(vertexElement);

    ///Normals

    TiXmlElement * normalElement=new TiXmlElement("normals");

    normalElement->SetAttribute("count",vertexCount);

    std::string allNormals;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f ",normals[i].x,normals[i].y,normals[i].z);
        allNormals+=value;
    }
    TiXmlText * normalText = new TiXmlText( allNormals );
    normalElement->LinkEndChild( normalText );

    root->LinkEndChild(normalElement);

    ///TexCoords

    TiXmlElement * texElement=new TiXmlElement("texCoords");

    texElement->SetAttribute("count",vertexCount);

    std::string allTex;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f ",texCoords[i].x,texCoords[i].y,texCoords[i].z);
        allTex+=value;
    }
    TiXmlText * texText = new TiXmlText( allTex );
    texElement->LinkEndChild( texText );

    root->LinkEndChild(texElement);


    ///Color
    TiXmlElement * colorElement=new TiXmlElement("colors");

    colorElement->SetAttribute("count",vertexCount);

    std::string allColors;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f %f ",colors[i].x,colors[i].y,colors[i].z, colors[i].w);
        allColors+=value;
    }
    TiXmlText * colorText = new TiXmlText( allColors );
    colorElement->LinkEndChild( colorText );

    root->LinkEndChild(colorElement);


    ///SecondaryColor
    TiXmlElement * secondaryColorElement=new TiXmlElement("secondaryColors");

    secondaryColorElement->SetAttribute("count",vertexCount);

    std::string allSecColors;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f ",secondaryColors[i].x,secondaryColors[i].y,secondaryColors[i].z);
        allSecColors+=value;
    }
    TiXmlText * secColorText = new TiXmlText( allSecColors );
    secondaryColorElement->LinkEndChild( secColorText );

    root->LinkEndChild(secondaryColorElement);

    ///BoneReferences
    TiXmlElement * boneRefElement=new TiXmlElement("boneReferences");

    boneRefElement->SetAttribute("count",vertexCount);

    std::string allBoneRefs;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f %f ",boneReference[i].x,boneReference[i].y, boneReference[i].z, boneReference[i].w);
        allBoneRefs+=value;
    }
    TiXmlText * boneRefText = new TiXmlText( allBoneRefs );
    boneRefElement->LinkEndChild( boneRefText );

    root->LinkEndChild(boneRefElement);

    ///vertexWeights
    TiXmlElement * vertWeightElement=new TiXmlElement("vertexWeights");

    vertWeightElement->SetAttribute("count",vertexCount);

    std::string allWeightRefs;
    for (int i=0; i<(int)vertexCount;i++){
        sprintf(value,"%f %f %f %f ",vertexWeights[i].x, vertexWeights[i].y, vertexWeights[i].z, vertexWeights[i].w);
        allWeightRefs+=value;
    }
    TiXmlText * weightText = new TiXmlText( allWeightRefs );
    vertWeightElement->LinkEndChild( weightText );

    root->LinkEndChild(vertWeightElement);

    ///Bones

    for (uint i=0;i<myDrawing->bones.size();i++){

        TiXmlElement * boneElement=new TiXmlElement("bone");
        boneElement->SetAttribute("boneName",myDrawing->bones[i]->name);

        if (myDrawing->bones[i]->base)
            cout << "Bone base name: " << myDrawing->bones[i]->base->name << endl;

        boneElement->SetAttribute("parent",-1);

        //find parent by going through all of our bones and looking for baseActor
        for (uint parentPos=0;parentPos<myDrawing->bones.size();parentPos++){
            if (myDrawing->bones[parentPos] == myDrawing->bones[i]->base)
                boneElement->SetAttribute("parent",parentPos);               //parent found
        }

        ///Transform Matrix
        char matValue[512];
        Matrix4f mat4f;

        if (myDrawing->bones[i]->base)
            //mat4f=myDrawing->bones[i]->baseMatrix * myDrawing->bones[i]->base->baseMatrix.inverse();
            mat4f=myDrawing->bones[i]->originalMatrix * myDrawing->bones[i]->transformMatrix;


        TiXmlElement * transMatElement=new TiXmlElement("transformMatrix");
        sprintf(matValue,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
                    ,mat4f.data[0], mat4f.data[1], mat4f.data[2], mat4f.data[3],
                     mat4f.data[4], mat4f.data[5], mat4f.data[6], mat4f.data[7],
                     mat4f.data[8], mat4f.data[9], mat4f.data[10],mat4f.data[11],
                     mat4f.data[12],mat4f.data[13],mat4f.data[14],mat4f.data[15]);

        TiXmlText * transText= new TiXmlText( matValue );
        transMatElement->LinkEndChild(transText);
        boneElement->LinkEndChild(transMatElement);



        Matrix4f tMat=mat4f;

        ///Inverse Absolute Transform Matrix
        //seems cpmlicated, but we need to remove the drawing's baseMatrix, as we'll place it someplace else when we create it again!
        mat4f=(myDrawing->bones[i]->baseMatrix * myDrawing->baseMatrix.inverse()).inverse() ;

        TiXmlElement * invTransMatElement=new TiXmlElement("inverseTransformMatrix");
        sprintf(matValue,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
                    ,mat4f.data[0], mat4f.data[1], mat4f.data[2], mat4f.data[3],
                     mat4f.data[4], mat4f.data[5], mat4f.data[6], mat4f.data[7],
                     mat4f.data[8], mat4f.data[9], mat4f.data[10],mat4f.data[11],
                     mat4f.data[12],mat4f.data[13],mat4f.data[14],mat4f.data[15]);

        TiXmlText * invTransText= new TiXmlText( matValue);
        invTransMatElement->LinkEndChild(invTransText);
        boneElement->LinkEndChild(invTransMatElement);

        root->LinkEndChild(boneElement);
    }


    doc.SaveFile( filename );
    doc.Clear();


	//createVBOs(myDrawing->vboMeshID);
	loadSpriteMesh(filename, myDrawing->vboMeshID);
    return true;
}



bool SpriteMeshLoaderXML::createVBOs(string meshID){

    GLuint vertexBuffer=0;
    GLuint normalBuffer=0;
    GLuint colorBuffer=0;
    GLuint secondaryColorBuffer=0;
    GLuint texCoordBuffer=0;
    GLuint vertexWeightsBuffer=0;
    GLuint boneReferenceBuffer=0;

	MeshData* myMesh = sceneData->vboList[meshID];
	vertexCount= myMesh->vData.size();

	//clear any previous stuff...
	sceneData->vboList[meshID]->vertexCount.clear();
    sceneData->vboList[meshID]->vertexBufferObject.clear();
	sceneData->vboList[meshID]->vertexWeightsObject.clear();
	sceneData->vboList[meshID]->boneReferenceObject.clear();
    sceneData->vboList[meshID]->colorBufferObject.clear();
    sceneData->vboList[meshID]->secondaryColorBufferObject.clear();
    sceneData->vboList[meshID]->normalBufferObject.clear();
    sceneData->vboList[meshID]->texCoordBufferObject.clear();

	//vertex buffer
    glGenBuffersARB(1, &vertexBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &vertices[0].x , GL_STATIC_DRAW_ARB);


    sceneData->vboList[meshID]->vertexBufferObject.push_back(vertexBuffer);
	sceneData->vboList[meshID]->vertexCount.push_back(vertexCount);

    //bind bone References
    if ( sceneData->vboList[meshID]->bIsSkeletal){

		//bind weights 1 to 4
        glGenBuffersARB(1, &vertexWeightsBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexWeightsBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &vertexWeights[0].x, GL_STATIC_DRAW_ARB);

        glGenBuffersARB(1, &boneReferenceBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneReferenceBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &boneReference[0].x , GL_STATIC_DRAW_ARB);

        sceneData->vboList[meshID]->vertexWeightsObject.push_back(vertexWeightsBuffer);
        sceneData->vboList[meshID]->boneReferenceObject.push_back(boneReferenceBuffer);
	}

    glGenBuffersARB(1, &colorBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &colors[0].r , GL_STATIC_DRAW_ARB);

    sceneData->vboList[meshID]->colorBufferObject.push_back(colorBuffer);

	glGenBuffersARB(1, &secondaryColorBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, secondaryColorBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &secondaryColors[0].r , GL_STATIC_DRAW_ARB);

    sceneData->vboList[meshID]->secondaryColorBufferObject.push_back(secondaryColorBuffer);

    //normal Buffer
    glGenBuffersARB(1, &normalBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &normals[0].x , GL_STATIC_DRAW_ARB);

    sceneData->vboList[meshID]->normalBufferObject.push_back(normalBuffer);

    //texCoord Buffer
    glGenBuffersARB(1, &texCoordBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &texCoords[0].x , GL_STATIC_DRAW_ARB);

    sceneData->vboList[meshID]->texCoordBufferObject.push_back(texCoordBuffer);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    return true;
}



bool SpriteMeshLoaderXML::loadSpriteMesh( string filename, string meshID ){

    cout << "yay!" << meshID << endl;

    //check if meshID already exists!
    if (sceneData->vboList[meshID]){
        sceneData->vboList.erase(meshID);
        }

    // XML File Open

    cout << "Loading file..." << filename <<endl;

    TiXmlDocument doc( filename );
    if (!doc.LoadFile()) return false;

    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Skip over first Element
    //***********************************************************************
    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return false;

    // save this for later
    hRoot=TiXmlHandle(element);
    //end XML file open;

    //setup new MeshData
    MeshData* myMesh=new MeshData;
    myMesh->bIsSkeletal=false;
    myMesh->bIsHead=false;
    myMesh->bVertexColor=true;
    myMesh->boneCount=0;
    myMesh->texCoordPerVertexCount=3;
    myMesh->verticesPerShapeCount=4;
    myMesh->vertexInterpretation=GL_POINTS;
    myMesh->drawType=DRAW_VBOMESH;
    sceneData->vboList[meshID]=myMesh;


    loadVertices(meshID, hRoot.FirstChild("vertices").Element());
    loadNormals(meshID, hRoot.FirstChild("normals").Element());
    loadTexCoords(meshID, hRoot.FirstChild("texCoords").Element());

    loadColors(meshID, hRoot.FirstChild("colors").Element());
    loadSecondaryColors(meshID, hRoot.FirstChild("secondaryColors").Element());

    loadBoneReferences(meshID, hRoot.FirstChild("boneReferences").Element());
    loadVertexWeights(meshID, hRoot.FirstChild("vertexWeights").Element());

    loadBones(meshID, hRoot.FirstChild("bone").Element());


	//fill vertex data for editing
	if (!sceneData->vboList[meshID]->vData.empty())
		sceneData->vboList[meshID]->vData.clear();

	for (int i=0;i<vertexCount;i++){
        vertexData myVData;
        myVData.location=vertices[i];
        myVData.normal=normals[i];
        myVData.color=colors[i];
        myVData.secondaryColor=secondaryColors[i];
        myVData.birth=0.0f;
        myVData.texCoord=texCoords[i];
        myVData.vertexWeights=vertexWeights[i];
        myVData.boneReferences=boneReference[i];
        sceneData->vboList[meshID]->vData.push_back(myVData);
    }


    createVBOs(meshID);

   //now free our resources:
    free(vertexWeights);
    free(boneReference);

    free(vertices);
    free(normals);
    free(texCoords);

    doc.Clear();

    cout << "finished loading SpriteMesh" << endl;

    return true;
}


void SpriteMeshLoaderXML::loadBones(string meshID, TiXmlElement * sourceElement){

    for (TiXmlElement * boneElement=sourceElement;boneElement!=NULL;boneElement=boneElement->NextSiblingElement()){
        //create new bone in MeshData object
        bone * myBone=new bone;
        sceneData->vboList[meshID]->bones.push_back(myBone);

        myBone->name=boneElement->Attribute("boneName");
        if (myBone->name=="mouthUp")
            sceneData->vboList[meshID]->bIsHead=true;

        //fill the MeshData object with all our DATA
        myBone->invBoneMatrix=new Matrix4f;
        myBone->boneMatrix=new Matrix4f;


        TiXmlElement *boneTransElement=boneElement->FirstChildElement("transformMatrix");
        TiXmlElement *invMatrixElement=boneElement->FirstChildElement("inverseTransformMatrix");
        //helper! so we don't have to call static functions!
        Actor A;

        *myBone->invBoneMatrix=A.readMatrix4f((char*)invMatrixElement->GetText());   //a bit hacky - should probably be a static function or in a different class?
        *myBone->boneMatrix=A.readMatrix4f((char*)boneTransElement->GetText());

        //cout << "found new bone!" << endl;
        //cout << *myBone->invBoneMatrix << endl;
        //cout << *myBone->boneMatrix << endl;
    }

    int currentBone=0;
    for (TiXmlElement* boneParentElement = sourceElement;boneParentElement!=NULL;boneParentElement=boneParentElement->NextSiblingElement()){

        int parentBone=-1;
        boneParentElement->Attribute("parent", &parentBone);

        if (parentBone>=0){
            sceneData->vboList[meshID]->bones[currentBone]->parentBone=sceneData->vboList[meshID]->bones[parentBone];
            //cout << "set Parent bone!" << endl;
            }
        else
            sceneData->vboList[meshID]->bones[currentBone]->parentBone=NULL;

        currentBone++;
    }

    if (currentBone>1){

        sceneData->vboList[meshID]->bIsSkeletal=true;
        //sceneData->vboList[meshID]->bVertexColor=false;
        sceneData->vboList[meshID]->bindShapeMatrix=new Matrix4f;
        sceneData->vboList[meshID]->boneCount=currentBone;

    }

}

void SpriteMeshLoaderXML::loadVertices(string meshID, TiXmlElement * sourceElement){


    ///Vertices

    //Vertices
    cout << "gathering vertices..." << endl;

    //navigate to vertex
    TiXmlElement * vertexArrayElement=sourceElement;

    vertexArrayElement->Attribute("count",&vertexCount);
    vertices=new Vector4f[vertexCount];               //initialise Vertex Array



    //****************************************
    //read vertex information to char array!
    //****************************************
    const char* vStr =vertexArrayElement->GetText();


	char* vertexPart = strtok((char*)vStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<vertexCount;i++){
        float f;
        sscanf(vertexPart, "%f", &f);
        vertices[i].x=f;                //x
        vertexPart = strtok(NULL, " "); //advance
        sscanf(vertexPart, "%f", &f);
        vertices[i].y=f;                //y
        vertexPart = strtok(NULL, " "); //advance
        sscanf(vertexPart, "%f", &f);
        vertices[i].z=f;                //z
        vertexPart = strtok(NULL, " "); //advance
        sscanf(vertexPart, "%f", &f);
        vertices[i].w=f;                //z
        vertexPart = strtok(NULL, " "); //advance
        }

	sceneData->vboList[meshID]->vertexCount.push_back(vertexCount);
}

void SpriteMeshLoaderXML::loadNormals(string meshID, TiXmlElement * sourceElement){

    ///Normals
    int normalCount=0;

   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering normals..." << endl;

    TiXmlElement * normalArrayElement=sourceElement;

    normalArrayElement->Attribute("count",&normalCount);
    normals=new Vector3f[normalCount];               //initialise Normal Array

    //read normal information into char array!
    const char* nStr =normalArrayElement->GetText();
    char* normalPart = strtok((char*)nStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<normalCount;i++){
        float f;
        sscanf(normalPart, "%f", &f);
        normals[i].x=f;
        normalPart = strtok(NULL, " "); //advance
        sscanf(normalPart, "%f", &f);
        normals[i].y=f;
        normalPart = strtok(NULL, " "); //advance
        sscanf(normalPart, "%f", &f);
        normals[i].z=f;
        normalPart = strtok(NULL, " "); //advance
        }


}

void SpriteMeshLoaderXML::loadTexCoords(string meshID, TiXmlElement * sourceElement){

    ///TexCoords
    int texCount=0;

    if (!sourceElement)
        return;
   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering texCoords..." << endl;

    TiXmlElement * texArrayElement=sourceElement;

    texArrayElement->Attribute("count",&texCount);
    texCoords=new Vector3f[texCount];               //initialise tex Array

    //read tex information into char array!
    const char* nStr =texArrayElement->GetText();
    char* texPart = strtok((char*)nStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<texCount;i++){
        float f;
        sscanf(texPart, "%f", &f);
        texCoords[i].x=f;
        texPart = strtok(NULL, " "); //advance
        sscanf(texPart, "%f", &f);
        texCoords[i].y=f;
        texPart = strtok(NULL, " "); //advance
        sscanf(texPart, "%f", &f);
        texCoords[i].z=f;
        texPart = strtok(NULL, " "); //advance
        }


}

void SpriteMeshLoaderXML::loadColors(string meshID, TiXmlElement * sourceElement){

    ///Colors
    int colorCount=0;

    if (!sourceElement)
        return;
   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering colors..." << endl;

    TiXmlElement * colorArrayElement=sourceElement;

    colorArrayElement->Attribute("count",&colorCount);
    colors=new Vector4f[colorCount];               //initialise tex Array

    //read tex information into char array!
    const char* nStr =colorArrayElement->GetText();
    char* colorPart = strtok((char*)nStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<colorCount;i++){
        float f;
        sscanf(colorPart, "%f", &f);
        colors[i].r=f;
        colorPart = strtok(NULL, " "); //advance
        sscanf(colorPart, "%f", &f);
        colors[i].g=f;
        colorPart = strtok(NULL, " "); //advance
        sscanf(colorPart, "%f", &f);
        colors[i].b=f;
        colorPart = strtok(NULL, " "); //advance
        sscanf(colorPart, "%f", &f);
        colors[i].a=f;
        colorPart = strtok(NULL, " "); //advance
        }


}


void SpriteMeshLoaderXML::loadSecondaryColors(string meshID, TiXmlElement * sourceElement){

    ///Colors
    int sColorCount=0;

    if (!sourceElement)
        return;
   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering secondary colors..." << endl;

    TiXmlElement * sColorArrayElement=sourceElement;

    sColorArrayElement->Attribute("count",&sColorCount);
    secondaryColors=new Vector3f[sColorCount];               //initialise tex Array

    //read tex information into char array!
    const char* nStr =sColorArrayElement->GetText();
    char* sColorPart = strtok((char*)nStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<sColorCount;i++){
        float f;
        sscanf(sColorPart, "%f", &f);
        secondaryColors[i].r=f;
        sColorPart = strtok(NULL, " "); //advance
        sscanf(sColorPart, "%f", &f);
        secondaryColors[i].g=f;
        sColorPart  = strtok(NULL, " "); //advance
        sscanf(sColorPart, "%f", &f);
        secondaryColors[i].b=f;
        sColorPart  = strtok(NULL, " "); //advance
        }


}


void SpriteMeshLoaderXML::loadBoneReferences(string meshID, TiXmlElement * sourceElement){          // from color!

    int boneRefCount=0;

    //Vertices
    cout << "gathering boneReferences..." << endl;

    //navigate to vertex
    TiXmlElement * boneRefArrayElement=sourceElement;

    boneRefArrayElement->Attribute("count",&boneRefCount);
    boneReference=new Vector4f[boneRefCount];               //initialise Vertex Array

    //****************************************
    //read vertex information to char array!
    //****************************************
    const char* vStr =boneRefArrayElement->GetText();
    char* boneRefPart = strtok((char*)vStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<boneRefCount;i++){
        float f;
        sscanf(boneRefPart, "%f", &f);
        boneReference[i].x=f;                //x
        boneRefPart= strtok(NULL, " "); //advance
        sscanf(boneRefPart, "%f", &f);
        boneReference[i].y=f;                //y
        boneRefPart= strtok(NULL, " "); //advance
        sscanf(boneRefPart, "%f", &f);
        boneReference[i].z=f;                //z
        boneRefPart= strtok(NULL, " "); //advance
        sscanf(boneRefPart, "%f", &f);
        boneReference[i].w=f;                //z
        boneRefPart= strtok(NULL, " "); //advance
        }
}

void SpriteMeshLoaderXML::loadVertexWeights(string meshID, TiXmlElement * sourceElement){          // from color!


    ///Color
    int vertexWeightCount=0;

    //Vertices
    cout << "gathering vertexWeights..." << endl;

    //navigate to vertex
    TiXmlElement * vertexWeightArrayElement=sourceElement;

    vertexWeightArrayElement->Attribute("count",&vertexWeightCount);
    vertexWeights=new Vector4f[vertexWeightCount];               //initialise Vertex Array

	//****************************************
    //read vertex information to char array!
    //****************************************
    const char* vStr =vertexWeightArrayElement->GetText();
    char* vertexWeightPart = strtok((char*)vStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<vertexWeightCount;i++){
        float f;
        sscanf(vertexWeightPart, "%f", &f);
        vertexWeights[i].x=f;                //x
        vertexWeightPart= strtok(NULL, " "); //advance
        sscanf(vertexWeightPart, "%f", &f);
        vertexWeights[i].y=f;                //y
        vertexWeightPart= strtok(NULL, " "); //advance
        sscanf(vertexWeightPart, "%f", &f);
        vertexWeights[i].z=f;                //z
        vertexWeightPart= strtok(NULL, " "); //advance
        sscanf(vertexWeightPart, "%f", &f);
        vertexWeights[i].w=f;                //z
        vertexWeightPart= strtok(NULL, " "); //advance
        }
}

