#include "mynode.h"

MyNode::MyNode(std::string name,
               MyNode *parent)
    : mParent(parent)
    , name(name)
{

}

MyNode::~MyNode()
{

}

void MyNode::addChild(MyNode *child)
{
    mChildren.emplace_back(child);
}

void MyNode::addBoneDep(Bone bone)
{
    boneData.emplace_back(bone);
}

MyNode *MyNode::getParent()
{
    return mParent;
}

MyNode *MyNode::getChild(unsigned int i)
{
    assert(i < mChildren.size());

    return mChildren[i];
}

unsigned int MyNode::getChildCount()
{
    return mChildren.size();
}

std::string &MyNode::getName()
{
    return name;
}

Bone &MyNode::getBoneDep(unsigned int index)
{
    assert(index < boneData.size());

    return boneData[index];
}

unsigned int MyNode::getBoneDepCount()
{
    return boneData.size();
}
