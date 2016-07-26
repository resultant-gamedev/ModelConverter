#include "mynodeanimation.h"
#include "myanimation.h"

MyNodeAnimation::MyNodeAnimation(MyNode* node)
    : node(node)
{

}

MyNodeAnimation::~MyNodeAnimation()
{

}

void MyNodeAnimation::addTransformation(glm::vec3 scale,
                                        glm::vec3 translate,
                                        glm::vec3 rotate,
                                        float deadline)
{
    transformations.emplace_back(scale, translate, rotate, deadline);
}
