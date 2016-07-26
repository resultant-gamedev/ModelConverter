#include "myanimation.h"
#include "mynodeanimation.h"
#include "mynode.h"

MyAnimation::MyAnimation(std::string name, float duration)
    : name(name)
    , duration(duration)
{

}

MyAnimation::~MyAnimation()
{
    for(MyNodeAnimation* nodeAni : nodeAnimations)
    {
        delete nodeAni;
    }
}

std::string &MyAnimation::getName()
{
    return name;
}

float MyAnimation::getDuration()
{
    return duration;
}

void MyAnimation::addNodeAnimation(MyNode *node)
{
    nodeAnimations.emplace_back(new MyNodeAnimation(node));
    node->addNodeAnimation(nodeAnimations.back());
}

MyNodeAnimation *MyAnimation::getLastNodeAnim()
{
    return nodeAnimations.back();
}
