#ifndef MYNODE_H
#define MYNODE_H

#include <vector>
#include <string>
#include <cassert>

struct Bone
{
    Bone(int v, float b)
        : vertexIndex(v)
        , boneWeight(b){}

    int vertexIndex;
    float boneWeight;
};

class MyNode
{
public:
    MyNode(std::string name,
           MyNode* parent = 0);
    ~MyNode();

    void addChild(MyNode* child);
    void addBoneDep(Bone bone);

    MyNode *getParent();
    MyNode *getChild(unsigned int i);
    unsigned int getChildCount();
    std::string& getName();

    Bone& getBoneDep(unsigned int index);
    unsigned int getBoneDepCount();

private:
    MyNode* mParent;
    std::vector<MyNode*> mChildren;
    std::vector<Bone> boneData;

    std::string name;
};

#endif // MYNODE_H
