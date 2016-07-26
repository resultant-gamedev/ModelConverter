#include "mynode.h"

MyModelFormat::MyNode::MyNode(std::string name,
                              MyModelFormat::MyNode *parent)
    : mParent(parent)
    , name(name)
    , bindPose(0.0f)
    , invBindPose(0.0f)
{

}

MyModelFormat::MyNode::~MyNode()
{

}

void MyModelFormat::MyNode::setParent(MyModelFormat::MyNode *parent)
{
    mParent = parent;
}

void MyModelFormat::MyNode::addChild(MyModelFormat::MyNode *child)
{
    mChildren.emplace_back(child);
}

void MyModelFormat::MyNode::addBoneDep(int vertexIndex, float boneWeight)
{
    boneData.emplace_back(vertexIndex, boneWeight);
}

MyModelFormat::MyNode *MyModelFormat::MyNode::getParent()
{
    return mParent;
}

std::vector<MyModelFormat::MyNode *> &MyModelFormat::MyNode::getChildren()
{
    return mChildren;
}

std::string &MyModelFormat::MyNode::getName()
{
    return name;
}

void MyModelFormat::MyNode::setBindPose(glm::mat4 m)
{
    bindPose = m;
    invBindPose = glm::inverse(bindPose);
}

glm::mat4 MyModelFormat::MyNode::getBindPose()
{
    return bindPose;
}

glm::mat4 MyModelFormat::MyNode::getInvBindPose()
{
    return invBindPose;
}

void MyModelFormat::MyNode::addNodeAnimation(MyModelFormat::MyNodeAnimation *nodeAnim)
{
    nodeAnimations.emplace_back(nodeAnim);
}

MyModelFormat::Bone &MyModelFormat::MyNode::getBoneDep(unsigned int index)
{
    assert(index < boneData.size());

    return boneData[index];
}

unsigned int MyModelFormat::MyNode::getBoneDepCount()
{
    return boneData.size();
}
