#include "mymodel.h"

MyModelFormat::MyModel::MyModel()
{

}

MyModelFormat::MyModel::~MyModel()
{
    for(MyModelFormat::MyAnimation* animation : vAnimation)
    {
        delete animation;
    }
}

std::vector<glm::vec4> &MyModelFormat::MyModel::getPositions()
{
    return vPosition;
}

std::vector<glm::vec3> &MyModelFormat::MyModel::getNormals()
{
    return vNormal;
}

std::vector<glm::vec2> &MyModelFormat::MyModel::getUVs()
{
    return vUV;
}

std::vector<uint32_t> &MyModelFormat::MyModel::getIndices()
{
    return vIndex;
}

std::vector<MyModelFormat::MyNode> &MyModelFormat::MyModel::getBones()
{
    return vBone;
}

std::vector<MyModelFormat::MyAnimation *> &MyModelFormat::MyModel::getAnimations()
{
    return vAnimation;
}

MyModelFormat::MyNode *MyModelFormat::MyModel::findBone(std::string name)
{
    for(MyModelFormat::MyNode& bone : vBone)
    {
        if(bone.getName() == name)
            return &bone;
    }

    return NULL;
}
