#include "myparser.h"

MyParser::MyParser()
{

}

MyParser::~MyParser()
{
}

void MyParser::importFromFile(const string &filename)
{
    pManager = FbxManager::Create();

    FbxIOSettings *ios = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(ios);

    FbxImporter* lImporter = FbxImporter::Create(pManager,"");

    if(!lImporter->Initialize(filename.c_str(), -1, pManager->GetIOSettings()))
    {
        std::cout << "cannot open file " << filename << std::endl;
        std::cout << "Error returned: " << lImporter->GetStatus().GetErrorString() << std::endl;
        exit(-1);
    }

    scene = FbxScene::Create(pManager,"myScene");

    lImporter->Import(scene);
    lImporter->Destroy();

    loadNode(scene->GetRootNode());

    std::cout << data.size() << std::endl;
    std::cout << bones.size() << std::endl;

    for(MyNode& bone : bones)
    {
        std::cout << bone.getName() << std::endl;
    }

    pManager->Destroy();
}

void MyParser::exportToFile(const string &filename)
{

}

void MyParser::loadNode(FbxNode *node)
{
    FbxNode* parent = node->GetParent();
    std::string parentName("");

    if(parent)
         parentName = parent->GetName();

    bones.emplace_back(node->GetName(), findNode(parentName));

    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        FbxNodeAttribute *nodeAttributeFbx = node->GetNodeAttributeByIndex(i);
        FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();

        if(attributeType == FbxNodeAttribute::eMesh)
        {
            FbxMesh* mesh = node->GetMesh();
            if(mesh)
            {
                if(mesh->IsTriangleMesh())
                {
                    loadMesh(mesh);
                    loadSkelett(mesh);
                }
                else
                {
                    FbxGeometryConverter conv(pManager);
                    conv.Triangulate(node->GetScene(), true);

                    loadMesh(node->GetMesh());
                    loadSkelett(node->GetMesh());
                }
            }

            std::cout << "root node: ";
            loadNodeKeyframe(node);
        }
    }

    for(int i = 0; i < node->GetChildCount(); i++)
        loadNode(node->GetChild(i));
}

void MyParser::loadMesh(FbxMesh *pMesh)
{
    std::vector<glm::vec4> vPosition;
    std::vector<int> indices;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::vec2> vUV;

    // ======================= get vertices =========================
    for(int i = 0; i < pMesh->GetControlPointsCount(); i++)
    {
        FbxVector4 vert = pMesh->GetControlPointAt(i);
        vPosition.emplace_back(
                    glm::make_vec4(reinterpret_cast<float*>(vert.Buffer())));
    }

    indices.resize(pMesh->GetPolygonVertexCount());
    indices.assign(pMesh->GetPolygonVertices(),
                   pMesh->GetPolygonVertices() + pMesh->GetPolygonVertexCount());


    // ======================== get normals =========================
    FbxGeometryElementNormal* pNormalElem = pMesh->GetElementNormal();
    if(!pNormalElem)
    {
        std::cerr << "mesh has no normals" << std::endl;
        abort();
    }

    if(pNormalElem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
        for(int i = 0; i < pMesh->GetControlPointsCount(); i++)
        {
            int index = 0;

            if(pNormalElem->GetReferenceMode() == FbxGeometryElement::eDirect)
                index = i;

            if(pNormalElem->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                index = pNormalElem->GetIndexArray().GetAt(i);

            FbxVector4 tmp = pNormalElem->GetDirectArray().GetAt(index);
            glm::vec3 normal(tmp[0], tmp[1], tmp[2]);

            vNormal.emplace_back(normal);
        }
    }
    else if(pNormalElem->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        int polygonVertexIndex = 0;

        for(int i = 0; i < pMesh->GetPolygonCount(); i++)
        {
            int polygonSize = pMesh->GetPolygonSize(i);

            for(int k = 0; k < polygonSize; k++)
            {
                int index = 0;

                if(pNormalElem->GetReferenceMode() == FbxGeometryElement::eDirect)
                    index = polygonVertexIndex;

                if(pNormalElem->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                    index = pNormalElem->GetIndexArray().GetAt(polygonVertexIndex);

                FbxVector4 tmp = pNormalElem->GetDirectArray().GetAt(index);
                glm::vec3 normal(tmp[0], tmp[1], tmp[2]);

                vNormal.emplace_back(normal);

                polygonVertexIndex++;
            }
        }
    }

    // ======================== get UVs =========================
    FbxStringList uvNameList;
    pMesh->GetUVSetNames(uvNameList);

    for(int n = 0; n < uvNameList.GetCount(); n++)
    {
        const char* uvName = uvNameList.GetStringAt(n);
        const FbxGeometryElementUV* uvElement = pMesh->GetElementUV(uvName);

        if(!uvElement)
            continue;

        if( (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex) &&
            (uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) )
            continue;

        const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int indexCount = useIndex ? uvElement->GetIndexArray().GetCount() : 0;

        const int polyCount = pMesh->GetPolygonCount();

        if( uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint )
        {
            for(int i = 0; i < polyCount; i++)
            {
                const int polySize = pMesh->GetPolygonSize(i);

                for(int k = 0; k < polySize; k++)
                {
                    int polyVertIndex = pMesh->GetPolygonVertex(i, k);

                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex)
                                           : polyVertIndex;

                    FbxVector2 tmp = uvElement->GetDirectArray().GetAt(uvIndex);
                    glm::vec2 uv(tmp[0], tmp[1]);

                    vUV.emplace_back(uv);
                }
            }
        }
        else if( uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
        {
            int polygonVertexIndex = 0;

            for(int i = 0; i < polyCount; i++)
            {
                const int polySize = pMesh->GetPolygonSize(i);

                for(int k = 0; k < polySize; k++)
                {
                    if(polygonVertexIndex < indexCount)
                    {
                        int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polygonVertexIndex)
                                               : polygonVertexIndex;

                        FbxVector2 tmp = uvElement->GetDirectArray().GetAt(uvIndex);
                        glm::vec2 uv(tmp[0], tmp[1]);

                        vUV.emplace_back(uv);

                        polygonVertexIndex++;
                    }
                }
            }
        }
    }

    // check correctness
    assert(indices.size() == vNormal.size());
    assert(indices.size() == vUV.size());

    // fill data
    uint32_t size = indices.size();

    for(uint32_t i = 0; i < size; i++)
    {
        assert((uint32_t)indices[i] < vPosition.size());

        data.emplace_back(vPosition[indices[i]],
                vUV[i],
                vNormal[i]);
    }
}

void MyParser::loadNodeKeyframe(FbxNode *node)
{
    std::cout << "current node: " << node->GetName() << std::endl;

    int numAnimations = scene->GetSrcObjectCount<FbxAnimStack>();

    for(int i = 0; i < numAnimations; i++)
    {
        FbxAnimStack *animStack = (FbxAnimStack*)scene->GetSrcObject<FbxAnimStack>(i);

        std::cout << "\tanimStack: " << animStack->GetName() << std::endl;

        for(int k = 0; k < animStack->GetMemberCount(); k++)
        {
            FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(k);

            FbxAnimCurve *translationCurve = node->LclTranslation.GetCurve(animLayer);
            FbxAnimCurve *rotationCurve = node->LclRotation.GetCurve(animLayer);
            FbxAnimCurve *scalingCurve = node->LclScaling.GetCurve(animLayer);

            glm::vec3 scale;
            glm::vec3 translate;
            glm::vec3 rotate;

            if(scalingCurve)
            {
                std::cout << "\tisScalingCurve ";

                for(int n = 0; n < scalingCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = scalingCurve->KeyGetTime(n);
                    FbxDouble3 scalingVector = node->EvaluateLocalScaling(frameTime);
                    scale = glm::vec3(scalingVector[0],
                                      scalingVector[1],
                                      scalingVector[2]);

                    // If needed, get the time of the scaling keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();
                }
            }
            else
            {
                FbxDouble3 scalingVector = node->LclScaling.Get();
                scale = glm::vec3(scalingVector[0],
                                  scalingVector[1],
                                  scalingVector[2]);
            }

            if(translationCurve)
            {
                std::cout << "isTranslationCurve ";

                for(int n = 0; n < translationCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = translationCurve->KeyGetTime(n);
                    FbxDouble3 translationVector = node->EvaluateLocalTranslation(frameTime);
                    translate = glm::vec3(translationVector[0],
                                         translationVector[1],
                                         translationVector[2]);

                    // If needed, get the time of the translation keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();
                }
            }
            else
            {
                FbxDouble3 translationVector = node->LclTranslation.Get();
                translate = glm::vec3(translationVector[0],
                                      translationVector[1],
                                      translationVector[2]);
            }

            if(rotationCurve)
            {
                std::cout << "isRotationCurve" << std::endl;

                for(int n = 0; n < rotationCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = rotationCurve->KeyGetTime(n);
                    FbxDouble3 rotationVector = node->EvaluateLocalRotation(frameTime);
                    rotate = glm::vec3(rotationVector[0],
                                       rotationVector[1],
                                       rotationVector[2]);

                    // If needed, get the time of the rotation keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();
                }
            }
            else
            {
                std::cout << std::endl;

                FbxDouble3 rotationVector = node->LclRotation.Get();
                rotate = glm::vec3(rotationVector[0],
                                   rotationVector[1],
                                   rotationVector[2]);
            }
        }
    }
}

void MyParser::loadSkelett(FbxMesh *mesh)
{
    FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
    if(skin)
    {
        int boneCount = skin->GetClusterCount();
        for (int i = 0; i < boneCount; i++)
        {
            FbxCluster* cluster = skin->GetCluster(i);

            // Get a reference to the bone's node
            FbxNode* bone = cluster->GetLink();
            loadNodeKeyframe(bone);

            std::string name = bone->GetName();
            MyNode* node = findNode(name);

            // Get the bind pose
            // FbxAMatrix bindPoseMatrix;
            // cluster->GetTransformLinkMatrix(bindPoseMatrix);

            int *boneVertexIndices = cluster->GetControlPointIndices();
            double *boneVertexWeights = cluster->GetControlPointWeights();

            // Iterate through all the vertices, which are affected by the bone
            for(int k = 0; k < cluster->GetControlPointIndicesCount(); k++)
            {
                int boneVertexIndex = boneVertexIndices[k];
                float boneWeight = (float)boneVertexWeights[boneVertexIndex];

                node->addBoneDep(boneVertexIndex, boneWeight);

                //std::cout << "bonevertex: " << boneVertexIndex << std::endl;
                //std::cout << "boneweight: " << boneWeight << std::endl;
            }
        }
    }
}

MyNode *MyParser::findNode(string &name)
{
    for(MyNode& bone : bones)
    {
        if(bone.getName() == name)
            return &bone;
    }

    return NULL;
}
