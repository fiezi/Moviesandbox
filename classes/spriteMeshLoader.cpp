#include "spriteMeshLoader.h"
#include "renderer.h"
#include "input.h"
#include "base64Encode.h"



SpriteMeshLoader::SpriteMeshLoader(){

vertexWeights=NULL; //4 weights per vertex, we select the 4 most influential
boneReference=NULL; //4 bone References per vertex, we select the 4 most influential

normals=NULL;                               //normal array pointer
vertices=NULL;                              //vertex array pointer
texCoords=NULL;                              //vertex array pointer
renderer=Renderer::getInstance();

vertexCount=0;
}

SpriteMeshLoader::~SpriteMeshLoader(){
}



bool SpriteMeshLoader::saveSpriteMesh( string filename, SkeletalDrawing* myDrawing){

    //generate arrays from location

	MeshData* myMesh=renderer->vboList[myDrawing->vboMeshID];

	vertexCount=myMesh->vData.size();

	vertices=new Vector4f[vertexCount];

	//TODO: change to 8 bit unsigned Integer!
	colors=new Vector4f[vertexCount];
	secondaryColors=new Vector3f[vertexCount];
	normals=new Vector3f[vertexCount];

	texCoords=new Vector3f[vertexCount];

	//TODO: change to 8 bit unsigned Integer!
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

    saveVertices(root);
	delete(vertices);

    saveNormals(root);
	delete(normals);

    saveTexCoords(root);
	delete(texCoords);

    saveColors(root);
	delete(colors);

    saveSecondaryColors(root);
	delete(secondaryColors);

    saveBoneReferences(root);
	delete(boneReference);

    saveVertexWeights(root);
	delete(vertexWeights);

    saveBones(root, myDrawing);

    ///Bones


    doc.SaveFile( filename );
    doc.Clear();


	//createVBOs(myDrawing->vboMeshID);
	loadSpriteMesh(filename, myDrawing->vboMeshID);
    return true;
}


void SpriteMeshLoader::saveVertices(TiXmlElement* root){

    ///Vertices
	cout << "saving vertices..." << endl;

        TiXmlElement * vertexElement=new TiXmlElement("vertices");

        vertexElement->SetAttribute("vertexInterpretation",GL_POINTS);
        vertexElement->SetAttribute("count",vertexCount);

///Base64 Encoding!
        char* vertexPointer=(char*)(&vertices[0].x);
        vector<unsigned char> vertexBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector4f)); i++)
            vertexBytes.push_back(vertexPointer[i]);

        string vertexString=Base64::Encode(vertexBytes);

        TiXmlText * vertexText = new TiXmlText( vertexString );
        vertexElement->LinkEndChild( vertexText );
        root->LinkEndChild(vertexElement);



}

//saves as bytes
void SpriteMeshLoader::saveNormals(TiXmlElement* root){

    ///Normals

	cout << "saving normals..." << endl;

        TiXmlElement * normalElement=new TiXmlElement("normals");
        normalElement->SetAttribute("count",vertexCount);

        Vector3b* byteNormals=new Vector3b[vertexCount];
        for (int i=0;i<vertexCount;i++){
            byteNormals[i]=normals[i] * 127.0;
        }

        char* nPointer=(char*)(&byteNormals[0].x);
        vector<unsigned char> nBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector3b)); i++)
            nBytes.push_back(nPointer[i]);

        string nString=Base64::Encode(nBytes);

        TiXmlText * normalText = new TiXmlText( nString );
        normalElement->LinkEndChild( normalText );

        root->LinkEndChild(normalElement);
		delete(byteNormals);
}

//saves as integers
void SpriteMeshLoader::saveTexCoords(TiXmlElement* root){

	cout << "saving texCoords..." << endl;

	///TexCoords


        TiXmlElement * texElement=new TiXmlElement("texCoords");
        texElement->SetAttribute("count",vertexCount);

		Vector3i* byteCoords=new Vector3i[vertexCount];
		for (int i=0;i<vertexCount;i++){
			byteCoords[i]=texCoords[i] * 65537.0;
		}

        char* tPointer=(char*)(&byteCoords[0].x);
        vector<unsigned char> tBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector3i)); i++)
            tBytes.push_back(tPointer[i]);

        string tString=Base64::Encode(tBytes);

        TiXmlText * texText = new TiXmlText( tString );
        texElement->LinkEndChild( texText );

        root->LinkEndChild(texElement);
	delete(byteCoords);
}

//saves as bytes
void SpriteMeshLoader::saveColors(TiXmlElement* root){

	cout << "saving colors..." << endl;

    ///Color
        TiXmlElement * colorElement=new TiXmlElement("colors");
        colorElement->SetAttribute("count",vertexCount);

        //convert to signed byte
        Vector4b* byteColors=new Vector4b[vertexCount];
        for (int i=0;i<vertexCount;i++){
            byteColors[i]=colors[i] * 255.0;
        }

        char* cPointer=(char*)(&byteColors[0].x);
        vector<unsigned char> cBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector4b)); i++)
            cBytes.push_back(cPointer[i]);

        string cString=Base64::Encode(cBytes);

        TiXmlText * colorText = new TiXmlText( cString );
        colorElement->LinkEndChild( colorText );

        root->LinkEndChild(colorElement);

	delete(byteColors);
}

//saves as bytes
void SpriteMeshLoader::saveSecondaryColors(TiXmlElement* root){

	cout << "saving secondary colors..." << endl;

    ///SecondaryColor
        TiXmlElement * secondaryColorElement=new TiXmlElement("secondaryColors");
        secondaryColorElement->SetAttribute("count",vertexCount);

		//convert to signed byte
		Vector3b* byteColors=new Vector3b[vertexCount];
		for (int i=0;i<vertexCount;i++){
			byteColors[i]=secondaryColors[i] * 255.0;
		}


		char* scPointer=(char*)(&byteColors[0].x);
        vector<unsigned char> scBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector3b)); i++)
            scBytes.push_back(scPointer[i]);

        string scString=Base64::Encode(scBytes);

        TiXmlText * sColorText = new TiXmlText( scString );
        secondaryColorElement->LinkEndChild( sColorText );
        root->LinkEndChild(secondaryColorElement);

	delete(byteColors);
}

//saves as unsigned bytes
void SpriteMeshLoader::saveBoneReferences(TiXmlElement* root){

	cout << "saving bone references..." << endl;

    ///BoneReferences

        TiXmlElement * boneRefElement=new TiXmlElement("boneReferences");
        boneRefElement->SetAttribute("count",vertexCount);

		Vector4b* byteReference=new Vector4b[vertexCount];
		for (int i=0;i<vertexCount;i++){
			byteReference[i]=boneReference[i];
		}


        char* brPointer=(char*)(&byteReference[0].x);
        vector<unsigned char> brBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector4b)); i++)
            brBytes.push_back(brPointer[i]);

        string brString=Base64::Encode(brBytes);

        TiXmlText * boneRefText = new TiXmlText( brString );
        boneRefElement->LinkEndChild( boneRefText );

        root->LinkEndChild(boneRefElement);
	delete(byteReference);
}

// saves as integers
void SpriteMeshLoader::saveVertexWeights(TiXmlElement* root){

	cout << "saving vertexWeights..." << endl;

    ///vertexWeights

        TiXmlElement * vertWeightElement=new TiXmlElement("vertexWeights");
        vertWeightElement->SetAttribute("count",vertexCount);

		Vector4i* byteWeights= new Vector4i[vertexCount];
		for (int i=0;i<vertexCount;i++){
			byteWeights[i]=vertexWeights[i] * 65535.0;
		}


        char* vwPointer=(char*)(&byteWeights[0].x);
        vector<unsigned char> vwBytes;

        for (int i=0;i<(int)(vertexCount*sizeof(Vector4i)); i++)
            vwBytes.push_back(vwPointer[i]);

        string vwString=Base64::Encode(vwBytes);

        TiXmlText * weightText = new TiXmlText( vwString);
        vertWeightElement->LinkEndChild( weightText );

        root->LinkEndChild(vertWeightElement);
	delete(byteWeights);
}

void SpriteMeshLoader::saveBones(TiXmlElement* root, SkeletalDrawing* myDrawing){

	cout << "saving bones..." << endl;


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
}

bool SpriteMeshLoader::createVBOs(string meshID){


    GLuint vertexIDBuffer=0;
    GLuint vertexBuffer=0;
    GLuint normalBuffer=0;
    GLuint colorBuffer=0;
    GLuint secondaryColorBuffer=0;
    GLuint texCoordBuffer=0;
    GLuint vertexWeightsBuffer=0;
    GLuint boneReferenceBuffer=0;

	MeshData* myMesh = renderer->vboList[meshID];
	vertexCount= myMesh->vData.size();

	//clear any previous stuff...
	renderer->vboList[meshID]->vertexCount.clear();
    renderer->vboList[meshID]->vertexBufferObject.clear();
	renderer->vboList[meshID]->vertexWeightsObject.clear();
	renderer->vboList[meshID]->boneReferenceObject.clear();
    renderer->vboList[meshID]->colorBufferObject.clear();
    renderer->vboList[meshID]->secondaryColorBufferObject.clear();
    renderer->vboList[meshID]->normalBufferObject.clear();
    renderer->vboList[meshID]->texCoordBufferObject.clear();

    cout << "setting up vertexCount" << endl;

    //vertexID Buffer
    vertexIDs=new float[vertexCount];
    for (int i=0;i<vertexCount;i++){
        vertexIDs[i]=i;
    }

    cout << "setting up vertexCount Buffer" << endl;

    glGenBuffersARB(1, &vertexIDBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexIDBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(float), &vertexIDs[0] , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->vertexIDObject.push_back(vertexIDBuffer);
    delete(vertexIDs);

    cout << "setting up vertexBuffer" << endl;

	//vertex buffer
    glGenBuffersARB(1, &vertexBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &vertices[0].x , GL_STATIC_DRAW_ARB);


    renderer->vboList[meshID]->vertexBufferObject.push_back(vertexBuffer);
	renderer->vboList[meshID]->vertexCount.push_back(vertexCount);


    //bind bone References
    if ( renderer->vboList[meshID]->bIsSkeletal){

		//bind weights 1 to 4
        glGenBuffersARB(1, &vertexWeightsBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexWeightsBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &vertexWeights[0].x, GL_STATIC_DRAW_ARB);

        glGenBuffersARB(1, &boneReferenceBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneReferenceBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &boneReference[0].x , GL_STATIC_DRAW_ARB);

        renderer->vboList[meshID]->vertexWeightsObject.push_back(vertexWeightsBuffer);
        renderer->vboList[meshID]->boneReferenceObject.push_back(boneReferenceBuffer);
	}

    cout << "setting up colorBuffer" << endl;

    glGenBuffersARB(1, &colorBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &colors[0].r , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->colorBufferObject.push_back(colorBuffer);

	glGenBuffersARB(1, &secondaryColorBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, secondaryColorBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &secondaryColors[0].r , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->secondaryColorBufferObject.push_back(secondaryColorBuffer);

    //normal Buffer
    glGenBuffersARB(1, &normalBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &normals[0].x , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->normalBufferObject.push_back(normalBuffer);

    //texCoord Buffer
    glGenBuffersARB(1, &texCoordBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &texCoords[0].x , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->texCoordBufferObject.push_back(texCoordBuffer);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    return true;
}



bool SpriteMeshLoader::loadSpriteMesh( string filename, string meshID ){


    //check if meshID already exists!
    if (renderer->vboList[meshID]){
        renderer->vboList.erase(meshID);
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
    renderer->vboList[meshID]=myMesh;


    loadVertices(meshID, hRoot.FirstChild("vertices").Element());
    loadNormals(meshID, hRoot.FirstChild("normals").Element());
    loadTexCoords(meshID, hRoot.FirstChild("texCoords").Element());

    loadColors(meshID, hRoot.FirstChild("colors").Element());
    loadSecondaryColors(meshID, hRoot.FirstChild("secondaryColors").Element());

    loadBoneReferences(meshID, hRoot.FirstChild("boneReferences").Element());
    loadVertexWeights(meshID, hRoot.FirstChild("vertexWeights").Element());

    cout << "loading bones..." << endl;

    loadBones(meshID, hRoot.FirstChild("bone").Element());


	//fill vertex data for editing
	if (!renderer->vboList[meshID]->vData.empty())
		renderer->vboList[meshID]->vData.clear();

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
        renderer->vboList[meshID]->vData.push_back(myVData);
    }

    cout << "before creating vbos..." << endl;


    createVBOs(meshID);

    cout << "after creating vbos..." << endl;


   //now free our resources:
	delete(vertices);
	delete(normals);
	delete(texCoords);
	delete(colors);
	delete(secondaryColors);
	delete(vertexWeights);
	delete(boneReference);

    doc.Clear();

    cout << "finished loading SpriteMesh" << endl;

    return true;
}


void SpriteMeshLoader::loadBones(string meshID, TiXmlElement * sourceElement){

    for (TiXmlElement * boneElement=sourceElement;boneElement!=NULL;boneElement=boneElement->NextSiblingElement()){
        //create new bone in MeshData object
        bone * myBone=new bone;
        renderer->vboList[meshID]->bones.push_back(myBone);

        myBone->name=boneElement->Attribute("boneName");
        if (myBone->name=="mouthUp")
            renderer->vboList[meshID]->bIsHead=true;

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
            renderer->vboList[meshID]->bones[currentBone]->parentBone=renderer->vboList[meshID]->bones[parentBone];
            //cout << "set Parent bone!" << endl;
            }
        else
            renderer->vboList[meshID]->bones[currentBone]->parentBone=NULL;

        currentBone++;
    }

    if (currentBone>1){

        renderer->vboList[meshID]->bIsSkeletal=true;
        //renderer->vboList[meshID]->bVertexColor=false;
        renderer->vboList[meshID]->bindShapeMatrix=new Matrix4f;
        renderer->vboList[meshID]->boneCount=currentBone;

    }

}

void SpriteMeshLoader::loadVertices(string meshID, TiXmlElement * sourceElement){

    ///Vertices

    //Vertices
    cout << "gathering vertices..." << endl;

    //navigate to vertex
    TiXmlElement * vertexArrayElement=sourceElement;

    vertexArrayElement->Attribute("count",&vertexCount);
    vertices=new Vector4f[vertexCount];               //initialise Vertex Array

    char* vStr =(char*)vertexArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    memcpy(vertices,&vChars[0],vChars.size());

	renderer->vboList[meshID]->vertexCount.push_back(vertexCount);
}

//loads as bytes
void SpriteMeshLoader::loadNormals(string meshID, TiXmlElement * sourceElement){

    ///Normals
    int normalCount=0;

   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering normals..." << endl;

    TiXmlElement * normalArrayElement=sourceElement;

    normalArrayElement->Attribute("count",&normalCount);
    normals=new Vector3f[normalCount];               //initialise Normal Array

    char* vStr =(char*)normalArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    Vector3b* byteNormals=new Vector3b[normalCount];

    memcpy(byteNormals,&vChars[0],vChars.size());

    for (int i=0;i<normalCount;i++){
        normals[i]=(Vector3f)(byteNormals[i]) /127.0;
    }
	delete(byteNormals);
}

//load as integer
void SpriteMeshLoader::loadTexCoords(string meshID, TiXmlElement * sourceElement){

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

	Vector3i* byteCoords = new Vector3i[texCount];

    char* vStr =(char*)texArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    memcpy(byteCoords,&vChars[0],vChars.size());

	for (int i=0;i<texCount;i++){
        texCoords[i]=Vector3f(byteCoords[i])/32767.0;
    }
	delete(byteCoords);
}

//loads as unsigned bytes
void SpriteMeshLoader::loadColors(string meshID, TiXmlElement * sourceElement){

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

    Vector4ub* byteColors=new Vector4ub[colorCount];

    char* vStr =(char*)colorArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    memcpy(byteColors,&vChars[0],vChars.size());

    for (int i=0;i<colorCount;i++){
        colors[i]=Vector4f(byteColors[i]) / 255.0;
    }
	delete(byteColors);
}

//loads as bytes
void SpriteMeshLoader::loadSecondaryColors(string meshID, TiXmlElement * sourceElement){

    ///SecondaryColors
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

	Vector3ub* byteColors=new Vector3ub[sColorCount];


    char* vStr =(char*)sColorArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    memcpy(byteColors,&vChars[0],vChars.size());

    for (int i=0;i<sColorCount;i++){
        secondaryColors[i]=Vector3f(byteColors[i]) / 255.0;
    }
	delete(byteColors);
}

//loads as bytes
void SpriteMeshLoader::loadBoneReferences(string meshID, TiXmlElement * sourceElement){          // from color!

    int boneRefCount=0;

    //Vertices
    cout << "gathering boneReferences..." << endl;

    //navigate to vertex
    TiXmlElement * boneRefArrayElement=sourceElement;

    boneRefArrayElement->Attribute("count",&boneRefCount);
    boneReference=new Vector4f[boneRefCount];               //initialise Vertex Array

	Vector4ub* byteReference=new Vector4ub[boneRefCount];


    char* vStr =(char*)boneRefArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);

    memcpy(byteReference,&vChars[0],vChars.size());

	for (int i=0;i<boneRefCount;i++){
        boneReference[i]=Vector4f(byteReference[i]);
    }
	delete(byteReference);
}

//loads as integer
void SpriteMeshLoader::loadVertexWeights(string meshID, TiXmlElement * sourceElement){          // from color!


    ///vertexWeights
    int vertexWeightCount=0;

    //Vertices
    cout << "gathering vertexWeights..." << endl;

    //navigate to vertex
    TiXmlElement * vertexWeightArrayElement=sourceElement;

    vertexWeightArrayElement->Attribute("count",&vertexWeightCount);
    vertexWeights=new Vector4f[vertexWeightCount];               //initialise Vertex Array


    cout << "allocating memory for byteWeights..." << endl;
	Vector4i* byteWeights=new Vector4i[vertexWeightCount];



    char* vStr =(char*)vertexWeightArrayElement->GetText();
    vector<unsigned char> vChars;
    vChars=Base64::Decode(vStr);
    memcpy(byteWeights,&vChars[0],vChars.size());

    cout << "after memcpy for byteWeights..." << endl;


	for (int i=0;i<vertexWeightCount;i++){
        vertexWeights[i]=byteWeights[i]/65535.0;
    }
	delete(byteWeights);
}

