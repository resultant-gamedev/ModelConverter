#ifndef MYNODE_H
#define MYNODE_H

#include <vector>
#include <string>
#include <cassert>

#include "extern/glm/glm.hpp"

struct Bone
{
    Bone(int v, float b)
        : vertexIndex(v)
        , boneWeight(b){}

    int vertexIndex;
    float boneWeight;
};

class MyNodeAnimation;
class MyAnimation;

/**
 * @brief The MyNode class
 * Saves node structure with bone data and
 * corresponding animation
 */
class MyNode
{
    friend class MyAnimation;

public:
    MyNode(std::string name,
           MyNode* parent = 0);
    ~MyNode();

    void addChild(MyNode* child);
    void addBoneDep(int vertexIndex, float boneWeight);

    Bone& getBoneDep(unsigned int index);
    unsigned int getBoneDepCount();

    MyNode *getParent();
    MyNode *getChild(unsigned int i);
    unsigned int getChildCount();
    std::string& getName();

    void setBindPose(glm::mat4 m);

private:    
    void addNodeAnimation(MyNodeAnimation* nodeAnim);

    MyNode* mParent;
    std::vector<MyNode*> mChildren;
    std::vector<Bone> boneData;
    std::vector<MyNodeAnimation*> nodeAnimations;

    std::string name;

    glm::mat4 bindPose;
    glm::mat4 invBindPose;
};

#endif // MYNODE_H
