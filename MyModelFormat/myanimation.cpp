#include "myanimation.h"
#include "mynodeanimation.h"
#include "mynode.h"

MyAnimation::MyAnimation()
{

}

MyAnimation::~MyAnimation()
{
    for(MyNodeAnimation* nodeAni : nodeAnimations)
    {
        delete nodeAni;
    }
}

void MyAnimation::addNodeAnimation(MyNode *node)
{
    nodeAnimations.emplace_back(new MyNodeAnimation);
    node->addNodeAnimation(nodeAnimations.back());
}

MyNodeAnimation *MyAnimation::getLastNodeAnim()
{
    return nodeAnimations.back();
}
