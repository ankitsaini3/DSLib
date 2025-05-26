#include"../include/ds/vector.hpp"
#include<iostream>
#include"A.hpp"
#include<vector>


int main(int argc, char const *argv[])
{
    // A ob0;
    // A ob1(4,5.6);
    // A obx(2,5.6);


    // ds::vector<A> v0;
    // ds::vector<A> v1(3);
    // ds::vector<A> v2(3, ob1);
    // ds::vector<A> v3(v2);
    // ds::vector<A> vx(v2);
    // ds::vector<A> v4(std::move(vx));
    // ds::vector<A> v5{ob0, ob1};

    // ds::vector<A> v6(5, A(5,6.7));
    // ds::vector<A> v7(2, A(5,4.5));
    // ds::vector<A> v8(3, A(1,2.0));
    // ds::vector<A> v9(3, A(1,2.3));

    // v8 = v6;
    // v9 = v7;


    // std::cout << v0.capacity() << std::endl;
    // std::cout << v1.capacity() << std::endl;
    // std::cout << v2.capacity() << std::endl;
    // std::cout << v3.capacity() << std::endl;
    // std::cout << v4.capacity() << std::endl;
    // std::cout << v5.capacity() << std::endl;
    // std::cout << v6.capacity() << std::endl;
    // std::cout << v7.capacity() << std::endl;
    // std::cout << v8.capacity() << std::endl;
    // std::cout << v9.capacity() << std::endl;
    // std::cout << vx.capacity() << std::endl;

    // ds::vector<A> v(5, A(4,5.6));

    // for(size_t i=0; i<v.size(); i++)
    // {
    //     v.at(i).display();
    // }


    // ds::vector<int> v{23,12,4,5,6};

    // int a = 0;

    // try
    // {
    //     a = v.at(5);
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

    // ds::vector<A> v0{A(3,4), A(2,5.6), A(1,8.9)};
    
    // for(auto& it: v0)
    // {
    //     it.display();
    // }

    ds::vector<A> v0{A(1,4.5), A(4,6), A(4,6.7)};
    A ob(5,6);

    std::cout << v0.size() << v0.capacity() << std::endl;
    // v0.clear();
    // v0.push_back(A(8,4));
    // v0.push_back(ob);
    // v0.pop_back();
    // v0[v0.size()-1].display();
    // std::cout << v0.size() << v0.capacity() << std::endl;

    


    return 0;
}
