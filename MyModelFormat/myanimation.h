#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <vector>

class MyNode;
class MyNodeAnimation;

/**
 * @brief The MyAnimation class
 * Saves all animations of each bone
 */
class MyAnimation
{
public:
    MyAnimation();
    ~MyAnimation();

    void addNodeAnimation(MyNode* node);
    MyNodeAnimation* getLastNodeAnim();

private:
    std::vector<MyNodeAnimation*> nodeAnimations;
};

#endif // MYANIMATION_H
