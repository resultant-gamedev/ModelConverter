#ifndef MYMODEL_H
#define MYMODEL_H

#include "MyInternalFormat/mynode.h"
#include "MyInternalFormat/mynodeanimation.h"
#include "MyInternalFormat/myanimation.h"

namespace MyModelFormat
{

class MyModel
{
public:
    MyModel();
    ~MyModel();

    std::vector<glm::vec4>& getPositions();
    std::vector<glm::vec3>& getNormals();
    std::vector<glm::vec2>& getUVs();
    std::vector<uint32_t>& getIndices();
    std::vector<MyModelFormat::MyNode>& getBones();
    std::vector<MyModelFormat::MyAnimation*>& getAnimations();

    MyNode* findBone(std::string name);

private:
    std::vector<glm::vec4> vPosition;
    std::vector<uint32_t> vIndex;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::vec2> vUV;

    std::vector<MyModelFormat::MyNode> vBone;
    std::vector<MyModelFormat::MyAnimation*> vAnimation;
};

}

#endif // MYMODEL_H
