#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <vector>
#include <string>

class MyNode;
class MyNodeAnimation;
class MyExporter;
class MyImporter;

/**
 * @brief The MyAnimation class
 * Saves all animations of each bone
 */
class MyAnimation
{
    friend class MyExporter;
    friend class MyImporter;

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
