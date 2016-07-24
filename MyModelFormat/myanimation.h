#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <vector>
#include <string>

class MyNode;
class MyNodeAnimation;

/**
 * @brief The MyAnimation class
 * Saves all animations of each bone
 */
class MyAnimation
{
public:
    MyAnimation(std::string name, float duration);
    ~MyAnimation();

    std::string& getName();
    float getDuration();

    void addNodeAnimation(MyNode* node);
    MyNodeAnimation* getLastNodeAnim();

private:
    std::vector<MyNodeAnimation*> nodeAnimations;
    std::string name;
    float duration;
};

#endif // MYANIMATION_H
