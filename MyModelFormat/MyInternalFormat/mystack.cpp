#include "mystack.h"

MyModelFormat::MyStack::MyStack(std::string name,
                                MyModelFormat::MyStack* parent)
    : name(name)
    , parent(parent)
{

}

MyModelFormat::MyStack::~MyStack()
{

}

MyModelFormat::MyStack *MyModelFormat::MyStack::getParent()
{
    return parent;
}

std::vector<MyModelFormat::MyStack *> &MyModelFormat::MyStack::getChildren()
{
    return children;
}

std::string MyModelFormat::MyStack::getName()
{
    return name;
}

void MyModelFormat::MyStack::addChild(MyModelFormat::MyStack *child)
{
    children.emplace_back(child);
}

void MyModelFormat::MyStack::addContent(std::string cont)
{
    content += cont;
}

std::string &MyModelFormat::MyStack::getContent()
{
    return content;
}
