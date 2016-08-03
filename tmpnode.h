#ifndef TMPNODE_H
#define TMPNODE_H

#include "MyModelFormat/mymodel.h"

class TmpNode
{
    friend class MyParser;

public:
    TmpNode(std::string name,
           MyNode* parent = 0);
    ~TmpNode();

    void setParent(TmpNode* parent);
    void addChild(TmpNode* child);
    void addBoneDep(int vertexIndex, float boneWeight);

    std::vector<MyModelFormat::Bone>& getBoneDeps();

    TmpNode *getParent();
    std::vector<TmpNode*>& getChildren();
    std::string& getName();

    void setBindPose(glm::mat4 m);
    glm::mat4 getBindPose();
    glm::mat4 getInvBindPose();

private:
    void addNodeAnimation(MyNodeAnimation* nodeAnim);

    TmpNode* mParent;
    std::vector<TmpNode*> mChildren;
    std::vector<Bone> boneData;
    std::vector<MyNodeAnimation*> nodeAnimations;

    std::string name;

    glm::mat4 bindPose;
    glm::mat4 invBindPose;
};

#endif // TMPNODE_H
