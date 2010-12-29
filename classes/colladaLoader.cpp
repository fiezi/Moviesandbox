#include "colladaLoader.h"


ColladaLoader::ColladaLoader(){

vertexWeights=NULL; //4 weights per vertex, we select the 4 most influential
boneReference=NULL; //4 bone References per vertex, we select the 4 most influential

normals=NULL;                               //normal array pointer
vertices=NULL;                              //vertex array pointer
texCoords=NULL;                              //vertex array pointer
renderer=Renderer::getInstance();
}

ColladaLoader::~ColladaLoader(){
}

bool ColladaLoader::loadColladaMesh( string filename, string meshID ){


    //check if meshID already exists!
    if (renderer->vboList[meshID]){
        renderer->vboList.erase(meshID);
        }

    // XML File Open

    cout << "Loading file..." << filename <<endl;

    TiXmlDocument doc( filename );
    if (!doc.LoadFile()) return false;

    renderer->vboList[meshID]=new MeshData;
    renderer->vboList[meshID]->drawType=DRAW_VBOMESH;

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


    /****************************************/
    //looking for Controller information...
    /****************************************/

    cout << "checking for Controller information..." << endl;

    //make sure we don't have a hole in our mesh
    renderer->vboList[meshID]->bIsHead=false;

    //no vertex color in Collada!
    if (loadColladaBones(meshID, hRoot)){
        renderer->vboList[meshID]->bIsSkeletal=true;
        renderer->vboList[meshID]->bVertexColor=false;
        }
    else{
        renderer->vboList[meshID]->bIsSkeletal=false;
        renderer->vboList[meshID]->bVertexColor=false;
        renderer->vboList[meshID]->boneCount=0;
        }




    //****************************************
    //geometry sources
    //****************************************
    TiXmlElement * sourceElement=hRoot.FirstChild("library_geometries").FirstChild("geometry").FirstChild("mesh").FirstChild("source").Element();
    loadColladaVertices(meshID, sourceElement);

    sourceElement=sourceElement->NextSiblingElement("source");
    loadColladaNormals(meshID, sourceElement);

    sourceElement=sourceElement->NextSiblingElement("source");
    loadColladaTexCoords(meshID, sourceElement);

    //add tangents/biNormals here...

    //change to use index buffer objects?
    createVBOs(meshID, sourceElement);

    //now free our resources:

    free(vertexWeights);
    free(boneReference);

    free(normals);
    free(vertices);
    free(texCoords);

    doc.Clear();
    boneNames.clear();

    cout << "finished loading Collada Mesh" << endl;

    return true;
}


/*******************************************************************************/
// Loading skeletal information
//
/*******************************************************************************/


bool ColladaLoader::loadColladaBones(string meshID, TiXmlHandle hRoot){

            //find bone hirarchy through visual scene nodes!
            TiXmlElement * sourceElement= hRoot.FirstChild("library_visual_scenes").FirstChild("visual_scene").FirstChild("node").Element();
            recurseThroughNodes(sourceElement,NULL,meshID);

            //look for controllers
            sourceElement=hRoot.FirstChild("library_controllers").FirstChild("controller").FirstChild("skin").FirstChild("bind_shape_matrix").Element();

            if (!sourceElement){
                cout << "no controller object found..." << endl;
                return false;
                }

            loadBindShapeMatrix(meshID, sourceElement);

            sourceElement=hRoot.FirstChild("library_controllers").FirstChild("controller").FirstChild("skin").FirstChild("source").Element();

            loadBoneNames(meshID, sourceElement);

            sourceElement=sourceElement->NextSiblingElement("source");

            loadInvBoneMatrices(meshID, sourceElement);

            sourceElement=sourceElement->NextSiblingElement("source");

            loadPerVertexWeights(meshID, sourceElement);

    return true;
}

void ColladaLoader::recurseThroughNodes(TiXmlElement* element, bone* parentBone, string meshID){

    bone* myBone=NULL;
    //check for element being a joint...
    //has to have an <sid> and a <type> - sid is the name referenced in the controller section of collada
    if (element->Attribute("type") && element->Attribute("sid")){
        if (!strcmp("JOINT",element->Attribute("type"))){
            //Houston, we have a Bone
            myBone=new bone;
            myBone->boneMatrix=new Matrix4f;
            //myBone->name=element->Attribute("sid");   //3DStudio changes bone sid after import - so we keep names as msb actor names
            myBone->name=element->Attribute("name");
            myBone->parentBone=parentBone;

            //COLLADA allowsd storing bone transforms in matrix form or in translation/rotation form...
            TiXmlElement* boneElement=element->FirstChildElement("matrix");
            if (boneElement){
                //read vertex information to char array!
                const char* bnStr =boneElement->GetText();
                char* bonePart = strtok((char*)bnStr, " "); // Splits spaces between words in string and advances
                float f;
                for (int i=0;i<16;i++){             //fill one matrix
                    sscanf(bonePart, "%f", &f);
                    myBone->boneMatrix->data[i]=f;
                    bonePart= strtok(NULL, " "); // advance
                    }
                *myBone->boneMatrix=myBone->boneMatrix->transpose();    //collada matrices are in row major order, we store them in column major order...
                renderer->vboList[meshID]->bones.push_back(myBone);
                }
            //TODO: code for supporting COLLADA bone translate and rotate goes here
            boneElement=element->FirstChildElement("translate");
            //sometimes the model has a specific scene rotation,
            //in this case we need to seperatly read out the rotation and the joint rotation
            boneElement=element->FirstChildElement("rotate");


            cout << "new bone: " << myBone->name <<  endl;
            if (myBone->parentBone)
              cout << "our parent is: " << myBone->parentBone->name << endl;
            else
              cout << "we dont have parents..." << endl;
            //cout << "matrix is: " << *myBone->boneMatrix << endl;

            }
        }
    if (element->FirstChildElement())   //if we have a child, give this as a parent
      recurseThroughNodes(element->FirstChildElement(), myBone, meshID);
    if (element->NextSiblingElement())  //if we have a sibling, give our parent as parent
      recurseThroughNodes(element->NextSiblingElement(), parentBone, meshID);
}


bool ColladaLoader::loadBindShapeMatrix(string meshID, TiXmlElement * sourceElement){

    /****************************************/
    //BindShapeMatrix
    /****************************************/


    cout << "loading BindShape Matrix" << endl;

    Matrix4f* bindShapeMat=new Matrix4f;//initialise Bind Shape Matrix

    //read matrix information into char array!
    const char* bindShapeMatrixStr =sourceElement->GetText();
    cout << *sourceElement << endl;
    cout << sourceElement->GetText() << endl;
    char* bindShapeMatrixPart = strtok((char*)bindShapeMatrixStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<16;i++){
            float f=0.0;
            sscanf(bindShapeMatrixPart, "%f", &f);
            bindShapeMat->data[i]=f;
            bindShapeMatrixPart= strtok(NULL, " "); // advance
            }
    *bindShapeMat=bindShapeMat->transpose();    //collada matrices are in row major order, we store them in column major order...
    renderer->vboList[meshID]->bindShapeMatrix=bindShapeMat;

    return true;
}

bool ColladaLoader::loadBoneNames(string meshID, TiXmlElement * sourceElement){

    /****************************************/
    //Bone Names
    /****************************************/

    int boneCount=0;

    cout << "gathering bone names..." << endl;

    //navigate to name list
    //for maya file
    TiXmlElement * boneNameArrayElement=sourceElement->FirstChildElement("Name_array");

    //for collada example:
    if (!boneNameArrayElement)
        boneNameArrayElement=sourceElement->FirstChildElement("IDREF_array");


    boneNameArrayElement->Attribute("count",&boneCount);

    //read vertex information to char array!
    const char* bnStr =boneNameArrayElement->GetText();
    char* boneNamePart = strtok((char*)bnStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<boneCount;i++){
        boneNames.push_back(boneNamePart);                //x
        if (boneNames[i]=="mouthUp")
            renderer->vboList[meshID]->bIsHead=true;
        boneNamePart = strtok(NULL, "  "); //advance
        cout << "found bone name: " << boneNames[i] << endl;
        }

    renderer->vboList[meshID]->boneCount=boneCount;
    cout << "our bone count is: " << boneCount << endl;
    return true;
}

bool ColladaLoader::loadInvBoneMatrices(string meshID, TiXmlElement * sourceElement){


    /****************************************/
    // inverse absolute Bone Matrices
    /****************************************/

    int invBoneMatrixCount=0;
    Matrix4f* invBoneMatrices;

    TiXmlElement * invBoneMatrixArrayElement=sourceElement->FirstChildElement("float_array");

    cout << "gathering inverse Bone matrices" << endl;

    invBoneMatrixArrayElement->Attribute("count",&invBoneMatrixCount);

    invBoneMatrices=new Matrix4f[invBoneMatrixCount/16];               //initialise Bone Matrix Array as an array of 4x4 Matrices

    //read matrix information into char array!
    const char* invMatrixStr =invBoneMatrixArrayElement->GetText();
    char* invMatrixPart = strtok((char*)invMatrixStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<invBoneMatrixCount/16;i++){
        float f;
        for (int j=0;j<16;j++){             //fill one matrix
            sscanf(invMatrixPart, "%f", &f);
            invBoneMatrices[i].data[j]=f;
            invMatrixPart= strtok(NULL, " "); // advance
            }
        invBoneMatrices[i]=invBoneMatrices[i].transpose();    //collada matrices are in row major order, we store them in column major order...
        }

    //bones are in different order in the <controller> and in the <visual_scene> nodes
    //sort bones to bring them in the same order as in <controller>
    for (uint i=0; i< boneNames.size();i++){
        cout << "currently reorganizing bones..." << endl;
        for (uint j=0; j<renderer->vboList[meshID]->bones.size();j++){

            if (boneNames[i]==renderer->vboList[meshID]->bones[j]->name){
                //put new location in container
                bone* interim=renderer->vboList[meshID]->bones[i];
                //move old location to new location
                renderer->vboList[meshID]->bones[i]=renderer->vboList[meshID]->bones[j];
                //move interim to old location
                renderer->vboList[meshID]->bones[j]=interim;
                }
            renderer->vboList[meshID]->bones[i]->invBoneMatrix=&invBoneMatrices[i];
            }
        }
    return true;
}



bool ColladaLoader::loadPerVertexWeights(string meshID, TiXmlElement * sourceElement){


    /****************************************/
    //Weight Array
    /****************************************/

    float* weightsArray;
    int weightCount=0;

    TiXmlElement * weightValuesElement=sourceElement->FirstChildElement("float_array");   //this specifies the number of weights per vertex

    weightValuesElement->Attribute("count",&weightCount);
    cout << "reading " << weightCount << " vertex weights" << endl;

    weightsArray=new float[weightCount];

    //read weight information into char array!
    const char* weightStr =weightValuesElement->GetText();

    char* weightPart = strtok((char*)weightStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<weightCount;i++){
        float f;
        sscanf(weightPart, "%f", &f);
        weightsArray[i]=f;
        weightPart= strtok(NULL, " "); // advance
        }


    /****************************************/
    //per Vertex Weights
    //4 bones per vertex
    /****************************************/

    int vertexWeightCount=0; //should be as many as there are vertices
    int* wpvArray;

    sourceElement=sourceElement->NextSiblingElement("vertex_weights");

    TiXmlElement * weightCountElement=sourceElement->FirstChildElement("vcount");   //this specifies the number of weights per vertex
    TiXmlElement * weightReferenceElement=sourceElement->FirstChildElement("v");    //this includes weight references and bone numbers

    cout << "referencing vertex weights" << endl;

    sourceElement->Attribute("count",&vertexWeightCount);

    //initialise vertex weight arrays - they are still relative to the vertices and need to be referenced according to the vertex referencing above!
    vertexWeights=new Vector4f[vertexWeightCount];
    boneReference=new Vector4f[vertexWeightCount];

    //initialise vertex Weight Arrays
    for (int i=0;i<vertexWeightCount;i++){
        vertexWeights[i]=Vector4f(0,0,0,0);
        boneReference[i]=Vector4f(0,0,0,0);
        }


    const char* weightCountStr =weightCountElement->GetText();  //how many weights per vertex?
    char* weightCountPart = strtok((char*)weightCountStr, " "); // Splits spaces between words in string and advances

    //fill weight count per vertex array
    wpvArray=new int[vertexWeightCount];
    for (int i=0;i<vertexWeightCount;i++){
        int wpv=0;
        sscanf(weightCountPart, "%d", &wpv);      //find number of weights for this vertex
        weightCountPart= strtok(NULL, " ");       // advance
        wpvArray[i]=wpv;
        }

    const char* weightReferenceStr =weightReferenceElement->GetText();  //reference to weight number
    char* weightReferencePart = strtok((char*)weightReferenceStr, " "); // Splits spaces between words in string and advances

    for (int i=0;i<vertexWeightCount;i++){
            for (int j=0;j<wpvArray[i];j++){
                int br=-1;    //bone Reference
                int wr=-1;    //weight Reference
                sscanf(weightReferencePart, "%d", &br);      //the bone number
                weightReferencePart= strtok(NULL, " ");
                sscanf(weightReferencePart, "%d", &wr);      //the weightReference
                weightReferencePart= strtok(NULL, " ");

                if (j<4){       //fill in the first 4 weights
                    boneReference[i][j]=br;
                    vertexWeights[i][j]=weightsArray[wr];
                    }
                else{           //push in if bigger than our smallest
                    float smallest=1.0f;
                    int index=0;

                    for (int s=0;s<4;s++)          //go through our weights
                        if (vertexWeights[i][s]<smallest){   //compare to curent smallest
                            index=s;                         //store reference to smallest
                            smallest=vertexWeights[i][index];
                            }
                    if (wr>vertexWeights[i][index]){            //if current is bigger than THE SMALLEST in our already read weight list
                        boneReference[i][index]=br;
                        vertexWeights[i][index]=weightsArray[wr];
                        }
                    }
                if (br==-1){
                    cout << "yay! something's wrong with the vertex weighting!" << endl;
                    exit(0);
                    }
                }

            //now normalise weights, so we end up with 1.0 if we add them all together

            float fullWeight=vertexWeights[i].x+vertexWeights[i].y+vertexWeights[i].z+vertexWeights[i].w;
            for (int j=0;j<4;j++)
                vertexWeights[i][j]=vertexWeights[i][j]/fullWeight;

        }

    free(weightsArray);

    return true;
}

/*******************************************************************************/
//  GEOMETRY
//
//  loads Vertices, Normals and Texture Mapping Information
//
/*******************************************************************************/


bool ColladaLoader::loadColladaVertices(string meshID, TiXmlElement * sourceElement){

    int coordinatesPerVertex=3;
    renderer->vboList[meshID]->verticesPerShapeCount=3;
    int vertexCount=0;

    //Vertices
    cout << "gathering vertices..." << endl;

    //navigate to vertex
    TiXmlElement * vertexArrayElement=sourceElement->FirstChildElement("float_array");

    vertexArrayElement->Attribute("count",&vertexCount);
    vertices=new Vector3f[vertexCount/coordinatesPerVertex];               //initialise Vertex Array

    //****************************************
    //read vertex information to char array!
    //****************************************
    const char* vStr =vertexArrayElement->GetText();
    char* vertexPart = strtok((char*)vStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<vertexCount/coordinatesPerVertex;i++){
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
        }

return true;
}


bool ColladaLoader::loadColladaNormals(string meshID, TiXmlElement * sourceElement){

    int coordinatesPerVertex=3;       //load stride from file!
    int normalCount=0;

   //****************************************
    //read normal information to char array!
    //****************************************
    cout << "gathering normals..." << endl;

    TiXmlElement * normalArrayElement=sourceElement->FirstChildElement("float_array");

    normalArrayElement->Attribute("count",&normalCount);
    normals=new Vector3f[normalCount/coordinatesPerVertex];               //initialise Normal Array

    //read normal information into char array!
    const char* nStr =normalArrayElement->GetText();
    char* normalPart = strtok((char*)nStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<normalCount/coordinatesPerVertex;i++){
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
    return true;
}


bool ColladaLoader::loadColladaTexCoords(string meshID, TiXmlElement* sourceElement){


    int texCoordsPerVertex=3;       //load stride from file!
    int texCoordCount=0;

    //****************************************
    //read texCoord information to char array!
    //****************************************

    cout << "gathering texCoords..." << endl;

    TiXmlElement * texCoordArrayElement=sourceElement->FirstChildElement("float_array");

    texCoordArrayElement->Attribute("count",&texCoordCount);
    texCoordArrayElement->NextSiblingElement("technique_common")->FirstChildElement("accessor")->Attribute("stride",&texCoordsPerVertex);          //how many texcoords we have per vertex

    texCoords=new Vector3f[texCoordCount/texCoordsPerVertex];               //initialise texCoord Array (divide by 2 because we map single floats to a Vector2f
    renderer->vboList[meshID]->texCoordPerVertexCount=texCoordsPerVertex;
    //read normal information into char array!
    const char* tStr =texCoordArrayElement->GetText();
    char* texCoordPart = strtok((char*)tStr, " "); // Splits spaces between words in string and advances
    for (int i=0;i<texCoordCount/texCoordsPerVertex;i++){
        float f;
        sscanf(texCoordPart, "%f", &f);
        texCoords[i].x=f;

        texCoordPart = strtok(NULL, " "); // advance
        sscanf(texCoordPart, "%f", &f);
        texCoords[i].y=f;
        texCoordPart = strtok(NULL, " "); // advance

        //support 3 texCoords per vertex
        if (texCoordsPerVertex>2){
            sscanf(texCoordPart, "%f", &f);
            texCoords[i].z=f;
            texCoordPart = strtok(NULL, " "); // advance
            }
        else
            texCoords[i].z=0.0;
        }
    return true;
}


bool ColladaLoader::createVBOs(string meshID, TiXmlElement* sourceElement){

    int* vertexReferenceArray=NULL;
    int* normalReferenceArray=NULL;
    int* texCoordReferenceArray=NULL;

    Vector3f* finalVertexArray=NULL;
    Vector4f* finalVertexWeights=NULL;        //weights 1-4 per vertex
    Vector4f* finalBoneReferences=NULL;        //boneReferences
    Vector3f* finalNormalArray=NULL;
    Vector3f* finalTexCoordArray=NULL;


    int referenceCount=0;
    int verticesPerFace=3;

    //****************************************
    // everything from here downwards will create a new VBO
    //****************************************

    //this should be done using index buffers!! much easier!!
    cout << "start referencing..." << endl;
    //Vertex References
    TiXmlElement * referenceElement=sourceElement->NextSiblingElement("triangles");

    //while (referenceElement){


            referenceElement->Attribute("count",&referenceCount);
            vertexReferenceArray=new int[referenceCount*verticesPerFace];               //Vertex Array
            normalReferenceArray=new int[referenceCount*verticesPerFace];               //Normal Array
            texCoordReferenceArray=new int[referenceCount*verticesPerFace];             //texCoord Array

            //we need to make sure that we know how much information is encoded in our vertex reference array,
            //so we can skip through things like tangents

            TiXmlElement* inputData=referenceElement->FirstChildElement("input");

            int numberOfInputTypes=0;

            while (inputData){
              numberOfInputTypes++;
              inputData=inputData->NextSiblingElement("input");
            }

            cout << "found " << numberOfInputTypes << " types of data in vertex reference array!" << endl;

            //read vertex Reference information to char array!
            TiXmlElement* dataElement=referenceElement->FirstChildElement("p");

            const char* rStr =dataElement->GetText();
            char* refPart = strtok((char*)rStr, " "); // Splits spaces between words in st

            cout << "connecting references..." << endl;

            for (int i=0;i<referenceCount*verticesPerFace;i++){
                int a;
                sscanf(refPart, "%d", &a);
                vertexReferenceArray[i]=a;   //for vertices

                refPart = strtok(NULL, " "); // one for normals
                sscanf(refPart, "%d", &a);
                normalReferenceArray[i]=a;

                refPart = strtok(NULL, " "); // one for uvs
                sscanf(refPart, "%d", &a);
                texCoordReferenceArray[i]=a;

                refPart = strtok(NULL, " "); // one to the next position

                for (int i=4;i<=numberOfInputTypes;i++){
                    //cout << "jumping over reference "<< i << " of " << numberOfInputTypes << "..." << endl;
                    refPart = strtok(NULL, " "); // one to the next position
                    }
            }

            finalVertexArray=new Vector3f[referenceCount*verticesPerFace];
            finalVertexWeights=new Vector4f[referenceCount*verticesPerFace];

            if (renderer->vboList[meshID]->bIsSkeletal)
                finalBoneReferences=new Vector4f[referenceCount*verticesPerFace];

            finalNormalArray=new Vector3f[referenceCount*verticesPerFace];
            finalTexCoordArray=new Vector3f[referenceCount*verticesPerFace];

            for (int j=0;j<referenceCount*verticesPerFace;j++){
                finalVertexArray[j]=vertices[vertexReferenceArray[j]];  //vertices

                if (renderer->vboList[meshID]->bIsSkeletal){
                    finalVertexWeights[j]=vertexWeights[vertexReferenceArray[j]]; //vertex weights 1-4
                    finalBoneReferences[j]=boneReference[vertexReferenceArray[j]]; //bone references
                    }
                finalNormalArray[j]=normals[normalReferenceArray[j]];  //normals
                finalTexCoordArray[j]=texCoords[texCoordReferenceArray[j]];  //texCoords
                }



    //after creating final Arrays, we can free the reference arrays!
    free(vertexReferenceArray);
    free(normalReferenceArray);
    free(texCoordReferenceArray);

    cout << "creating BufferObjects..." << endl;

    GLuint vertexBuffer=0;
    GLuint vertexWeightsBuffer=0;
    GLuint boneReferenceBuffer=0;
    GLuint normalBuffer=0;
    GLuint texCoordBuffer=0;

    //vertex buffer
    glGenBuffersARB(1, &vertexBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector3f), finalVertexArray , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->vertexBufferObject.push_back(vertexBuffer);
    renderer->vboList[meshID]->vertexCount.push_back(referenceCount*verticesPerFace);
    renderer->vboList[meshID]->vertexInterpretation=GL_TRIANGLES;

    //bind bone References
    if ( renderer->vboList[meshID]->bIsSkeletal){
        //bind weights 1 to 4
        glGenBuffersARB(1, &vertexWeightsBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexWeightsBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector4f), finalVertexWeights , GL_STATIC_DRAW_ARB);

        glGenBuffersARB(1, &boneReferenceBuffer);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneReferenceBuffer);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector4f), finalBoneReferences , GL_STATIC_DRAW_ARB);

        renderer->vboList[meshID]->vertexWeightsObject.push_back(vertexWeightsBuffer);
        renderer->vboList[meshID]->boneReferenceObject.push_back(boneReferenceBuffer);
        }

    //normal Buffer
    glGenBuffersARB(1, &normalBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector3f), finalNormalArray , GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->normalBufferObject.push_back(normalBuffer);

    //texCoord Buffer
    glGenBuffersARB(1, &texCoordBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
    if (renderer->vboList[meshID]->texCoordPerVertexCount==2)
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector2f), finalTexCoordArray, GL_STATIC_DRAW_ARB);
    else if (renderer->vboList[meshID]->texCoordPerVertexCount==3)
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, referenceCount*verticesPerFace*sizeof(Vector3f), finalTexCoordArray, GL_STATIC_DRAW_ARB);

    renderer->vboList[meshID]->texCoordBufferObject.push_back(texCoordBuffer);


    //after pushing all the final Arrays to the graphics card, we can clean them up as well!

    free(finalBoneReferences);
    free(finalNormalArray);
    free(finalTexCoordArray);
    free(finalVertexArray);
    free(finalVertexWeights);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    return true;
}
