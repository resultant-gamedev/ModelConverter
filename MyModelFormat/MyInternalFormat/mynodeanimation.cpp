#include "mynodeanimation.h"
#include "myanimation.h"

MyModelFormat::MyNodeAnimation::MyNodeAnimation(MyModelFormat::MyNode* node)
    : node(node)
{

}

MyModelFormat::MyNodeAnimation::~MyNodeAnimation()
{

}

void MyModelFormat::MyNodeAnimation::addTransformation(glm::vec3 scale,
                                                       glm::vec3 translate,
                                                       glm::vec3 rotate,
                                                       float deadline)
{
    transformations.emplace_back(scale, translate, rotate, deadline);
}
