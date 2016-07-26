#include "mystack.h"

MyStack::MyStack(std::string name, MyStack* parent)
    : name(name)
    , parent(parent)
{

}

MyStack::~MyStack()
{

}

MyStack *MyStack::getParent()
{
    return parent;
}

std::vector<MyStack *> &MyStack::getChildren()
{
    return children;
}

std::string MyStack::getName()
{
    return name;
}

void MyStack::addChild(MyStack *child)
{
    children.emplace_back(child);
}

void MyStack::addContent(std::string cont)
{
    content += cont;
}

std::string &MyStack::getContent()
{
    return content;
}
