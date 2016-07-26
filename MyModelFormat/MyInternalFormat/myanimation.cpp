#include "myanimation.h"
#include "mynodeanimation.h"
#include "mynode.h"

MyModelFormat::MyAnimation::MyAnimation(std::string name, float duration)
    : name(name)
    , duration(duration)
{

}

MyModelFormat::MyAnimation::~MyAnimation()
{
    for(MyModelFormat::MyNodeAnimation* nodeAni : nodeAnimations)
    {
        delete nodeAni;
    }
}

std::string &MyModelFormat::MyAnimation::getName()
{
    return name;
}

float MyModelFormat::MyAnimation::getDuration()
{
    return duration;
}

void MyModelFormat::MyAnimation::addNodeAnimation(MyModelFormat::MyNode *node)
{
    nodeAnimations.emplace_back(new MyModelFormat::MyNodeAnimation(node));
    node->addNodeAnimation(nodeAnimations.back());
}

MyModelFormat::MyNodeAnimation *MyModelFormat::MyAnimation::getLastNodeAnim()
{
    return nodeAnimations.back();
}
