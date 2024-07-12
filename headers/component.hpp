#pragma once

class Component {
public:
    Component() = default;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void graphics() = 0;
};