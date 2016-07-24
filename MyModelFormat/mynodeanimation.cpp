#include "mynodeanimation.h"
#include "myanimation.h"

MyNodeAnimation::MyNodeAnimation()
{

}

MyNodeAnimation::~MyNodeAnimation()
{

}

void MyNodeAnimation::connectNode(MyNode *node)
{
    this->node = node;
}

void MyNodeAnimation::addTranslations(glm::vec3 vector, float deadline)
{
    translations.emplace_back(vector, deadline);
}

void MyNodeAnimation::addRotation(glm::vec3 vector, float deadline)
{
    rotations.emplace_back(vector, deadline);
}

void MyNodeAnimation::addScaling(glm::vec3 vector, float deadline)
{
    scalings.emplace_back(vector, deadline);
}
