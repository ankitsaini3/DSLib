#include <iostream>
#include "A.hpp"

// default constructor
A::A()
{
    std::cout << "default ctor for A called " << this << "\n";
    i = new int;
    f = new float;
    *i = 10;
    *f = 3.14;
}

// parametrised constructor
A::A(int in, float fl)
{
    std::cout << "Para ctor for A is called " << this << "\n";
    i = new int;
    f = new float;

    *i = in;
    *f = fl;
}

// copy constructor
A::A(const A &other)
{
    std::cout << "copy constructor of A called " << this << "\n";
    i = new int;
    f = new float;

    *i = *(other.i);
    *f = *(other.f);
}

// move constructor
A::A(A &&other)
{
    std::cout << "move ctor for A called " << this << "\n";
    i = other.i;
    f = other.f;

    other.i = nullptr;
    other.f = nullptr;
}

// copy assignment
A &A::operator=(const A &other)
{
    std::cout << "copy assignment of A called " << this << "\n";
    if (this != &other)
    {
        delete f;
        delete i;

        i = new int(*(other.i));
        f = new float(*(other.f));
    }

    return *this;
}

// move assignment
A &A::operator=(A &&other) noexcept
{
    std::cout << "move assignment of A called " << this << "\n";
    if (this != &other)
    {
        delete f;
        delete i;

        i = other.i;
        f = other.f;

        other.i = nullptr;
        other.f = nullptr;
    }

    return *this;
}

// destructor
A::~A()
{
    std::cout << "dtor for A called " << this << "\n";
    delete f;
    delete i;
}


void A::display()
{
    std::cout << "i = " << *i << " f = " << *f << std::endl;
}