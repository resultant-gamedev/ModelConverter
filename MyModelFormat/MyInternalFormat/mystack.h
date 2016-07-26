#ifndef MYSTACK_H
#define MYSTACK_H

#include <string>
#include <vector>

namespace MyModelFormat
{

class MyStack
{
public:
    MyStack(std::string name, MyStack* parent = NULL);
    ~MyStack();

    MyStack*                getParent();
    std::vector<MyStack*>&  getChildren();
    std::string             getName();

    void addChild(MyStack* child);
    void addContent(std::string cont);
    std::string& getContent();

private:
    std::string content;

    std::string name;

    MyStack* parent;
    std::vector<MyStack*> children;
};

}

#endif // MYSTACK_H
