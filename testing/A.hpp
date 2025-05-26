#pragma once

class A
{
public:
    int *i;
    float *f;

    // default constructor
    A();

    // parametrised constructor
    A(int in, float fl);

    // copy constructor
    A(const A &other);

    // move constructor
    A(A &&other);

    // copy assignment
    A &operator=(const A &other);

    // move assignment
    A &operator=(A &&other) noexcept;

    // destructor
    ~A();

    void display();
};