#ifndef MYMODEL_H
#define MYMODEL_H

#include "MyInternalFormat/mynode.h"
#include "MyInternalFormat/mynodeanimation.h"
#include "MyInternalFormat/myanimation.h"

class MyModel
{
public:
    MyModel();
    ~MyModel();

    std::vector<glm::vec4>& getPositions();
    std::vector<glm::vec3>& getNormals();
    std::vector<glm::vec2>& getUVs();
    std::vector<uint32_t>& getIndices();
    std::vector<MyNode>& getBones();
    std::vector<MyAnimation>& getAnimations();

    MyNode* findBone(std::string name);

private:
    std::vector<glm::vec4> vPosition;
    std::vector<uint32_t> vIndex;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::vec2> vUV;

    std::vector<MyNode> vBone;
    std::vector<MyAnimation> vAnimation;
};

#endif // MYMODEL_H
