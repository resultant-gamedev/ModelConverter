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

    createAnimations();
    loadNode(scene->GetRootNode());

    pManager->Destroy();
}

void MyParser::exportToFile(const string &filename)
{
    MyExporter exporter("data/test.my", model);
}

void MyParser::loadNode(FbxNode *node)
{
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
        }
    }

    for(int i = 0; i < node->GetChildCount(); i++)
        loadNode(node->GetChild(i));
}

void MyParser::loadMesh(FbxMesh *pMesh)
{
    // ======================= get vertices =========================
    for(int i = 0; i < pMesh->GetControlPointsCount(); i++)
    {
        FbxVector4 vert = pMesh->GetControlPointAt(i);
        model.getPositions().emplace_back(
                    glm::make_vec4(reinterpret_cast<float*>(vert.Buffer())));
    }

    model.getIndices().resize(pMesh->GetPolygonVertexCount());
    model.getIndices().assign(pMesh->GetPolygonVertices(),
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

            model.getNormals().emplace_back(normal);
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

                model.getNormals().emplace_back(normal);

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

                    model.getUVs().emplace_back(uv);
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

                        model.getUVs().emplace_back(uv);

                        polygonVertexIndex++;
                    }
                }
            }
        }
    }

    // check correctness
    assert(model.getIndices().size() == model.getNormals().size());
    assert(model.getIndices().size() == model.getUVs().size());
}

void MyParser::loadNodeKeyframe(FbxNode *node)
{
    int numAnimations = scene->GetSrcObjectCount<FbxAnimStack>();

    for(int i = 0; i < numAnimations; i++)
    {
        FbxAnimStack *animStack = (FbxAnimStack*)scene->GetSrcObject<FbxAnimStack>(i);

        for(int k = 0; k < animStack->GetMemberCount(); k++)
        {
            FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(k);

            FbxAnimCurve *translationCurve = node->LclTranslation.GetCurve(animLayer);
            FbxAnimCurve *rotationCurve = node->LclRotation.GetCurve(animLayer);
            FbxAnimCurve *scalingCurve = node->LclScaling.GetCurve(animLayer);

            std::string name = node->GetName();
            model.getAnimations()[i].addNodeAnimation(findNode(name));
            MyNodeAnimation* nodeAnim = model.getAnimations()[i].getLastNodeAnim();

            struct OneChannelTrans
            {
                OneChannelTrans(glm::vec3 v,
                                float t)
                    : vec(v)
                    , deadline(t){}

                glm::vec3 vec;
                float deadline;
            };

            std::vector<OneChannelTrans> scale;
            std::vector<OneChannelTrans> translate;
            std::vector<OneChannelTrans> rotate;

            if(scalingCurve)
            {
                for(int n = 0; n < scalingCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = scalingCurve->KeyGetTime(n);
                    FbxDouble3 scalingVector = node->EvaluateLocalScaling(frameTime);
                    glm::vec3 vscale (scalingVector[0],
                                     scalingVector[1],
                                     scalingVector[2]);

                    // If needed, get the time of the scaling keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();

                    scale.emplace_back(vscale, frameSeconds);
                }
            }
            else
            {
                FbxDouble3 scalingVector = node->LclScaling.Get();
                glm::vec3 vscale (scalingVector[0],
                                 scalingVector[1],
                                 scalingVector[2]);

                scale.emplace_back(vscale, -1.0f);
            }

            if(translationCurve)
            {
                for(int n = 0; n < translationCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = translationCurve->KeyGetTime(n);
                    FbxDouble3 translationVector = node->EvaluateLocalTranslation(frameTime);
                    glm::vec3 vtranslate (translationVector[0],
                                         translationVector[1],
                                         translationVector[2]);

                    // If needed, get the time of the translation keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();

                    translate.emplace_back(vtranslate, frameSeconds);
                }
            }
            else
            {
                FbxDouble3 translationVector = node->LclTranslation.Get();
                glm::vec3 vtranslate (translationVector[0],
                                     translationVector[1],
                                     translationVector[2]);

                translate.emplace_back(vtranslate, -1.0f);
            }

            if(rotationCurve)
            {
                for(int n = 0; n < rotationCurve->KeyGetCount(); n++)
                {
                    FbxTime frameTime = rotationCurve->KeyGetTime(n);
                    FbxDouble3 rotationVector = node->EvaluateLocalRotation(frameTime);
                    glm::vec3 vrotate (rotationVector[0],
                                      rotationVector[1],
                                      rotationVector[2]);

                    // If needed, get the time of the rotation keyframe, in seconds
                    float frameSeconds = (float)frameTime.GetSecondDouble();

                    rotate.emplace_back(vrotate, frameSeconds);
                }
            }
            else
            {
                std::cout << std::endl;

                FbxDouble3 rotationVector = node->LclRotation.Get();
                glm::vec3 vrotate (rotationVector[0],
                                  rotationVector[1],
                                  rotationVector[2]);

                rotate.emplace_back(vrotate, -1.0f);
            }

            // best case
            if( (scale.size() == translate.size()) &&
                    (scale.size() == rotate.size()) )
            {
                if(scalingCurve)
                {
                    for(uint32_t i = 0; i < scale.size(); i++)
                    {
                        if( (scale[i].deadline == rotate[i].deadline) &&
                                (scale[i].deadline == translate[i].deadline) )
                        {
                            nodeAnim->addTransformation(scale[i].vec,
                                                        translate[i].vec,
                                                        rotate[i].vec,
                                                        scale[i].deadline);
                        }
                    }
                }
                else
                {
                    nodeAnim->addTransformation(scale[0].vec,
                                                translate[0].vec,
                                                rotate[0].vec,
                                                0.0f);
                }
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

            FbxNode* parent = bone->GetParent();
            std::string parentName("");

            if(parent)
                 parentName = parent->GetName();

            model.getBones().emplace_back(bone->GetName(), findNode(parentName));

            loadNodeKeyframe(bone);

            std::string name = bone->GetName();
            MyNode* node = findNode(name);

            // Get the bind pose
            FbxAMatrix bindPoseMatrix;
            cluster->GetTransformLinkMatrix(bindPoseMatrix);
            node->setBindPose(glm::make_mat4((double*)bindPoseMatrix));

            int *boneVertexIndices = cluster->GetControlPointIndices();
            double *boneVertexWeights = cluster->GetControlPointWeights();

            for(int k = 0; k < cluster->GetControlPointIndicesCount(); k++)
            {
                int boneVertexIndex = boneVertexIndices[k];
                float boneWeight = (float)boneVertexWeights[boneVertexIndex];

                node->addBoneDep(boneVertexIndex, boneWeight);
            }
        }
    }
}

void MyParser::createAnimations()
{
    int numAnimations = scene->GetSrcObjectCount<FbxAnimStack>();

    for(int i = 0; i < numAnimations; i++)
    {
        FbxAnimStack *animStack = (FbxAnimStack*)scene->GetSrcObject<FbxAnimStack>(i);

        model.getAnimations().emplace_back(animStack->GetName(),
                                           animStack->GetLocalTimeSpan().GetDuration().GetSecondDouble());
    }
}

MyNode *MyParser::findNode(string &name)
{
    for(MyNode& bone : model.getBones())
    {
        if(bone.getName() == name)
            return &bone;
    }

    return NULL;
}
