#include "mymodel.h"

MyModel::MyModel()
{

}

MyModel::~MyModel()
{
    for(MyAnimation* animation : vAnimation)
    {
        delete animation;
    }
}

std::vector<glm::vec4> &MyModel::getPositions()
{
    return vPosition;
}

std::vector<glm::vec3> &MyModel::getNormals()
{
    return vNormal;
}

std::vector<glm::vec2> &MyModel::getUVs()
{
    return vUV;
}

std::vector<uint32_t> &MyModel::getIndices()
{
    return vIndex;
}

std::vector<MyNode> &MyModel::getBones()
{
    return vBone;
}

std::vector<MyAnimation *> &MyModel::getAnimations()
{
    return vAnimation;
}

MyNode *MyModel::findBone(std::string name)
{
    for(MyNode& bone : vBone)
    {
        if(bone.getName() == name)
            return &bone;
    }

    return NULL;
}
