#include <boost/version.hpp>
#include <boost/config.hpp>
#include <iostream>
using namespace std;

int main()
{

    cout<< BOOST_VERSION<<endl; //版本号
    cout<< BOOST_LIB_VERSION<<endl;

    cout<< BOOST_PLATFORM<<endl; //操作系统
    cout<< BOOST_COMPILER<<endl;
    cout<< BOOST_STDLIB<<endl;

}

