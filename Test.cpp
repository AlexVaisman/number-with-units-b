#include "doctest.h"
#include "NumberWithUnits.hpp"
#include <string>
#include <algorithm>  
#include <iostream> 
#include <sstream> 

using namespace ariel;


TEST_CASE("Checking operators plus minus and multiplication"){
    NumberWithUnits a(2,"cm");
    std::stringstream res;
    res<<a;
    CHECK(("2[cm]")==(res.str()));
    res<<-a;
    CHECK(("-2[cm]")==(res.str()));
    res<<+a;
    CHECK(("-2[cm]")==(res.str()));
    res<<5*a;
    CHECK(("10[cm]")==(res.str()));
    res<<a++;
    CHECK(("2[cm]")==(res.str()));
    res<<a;
    CHECK(("3[cm]")==(res.str()));
    res<<++a;
    CHECK(("4[cm]")==(res.str()));
    res<<a--;
    CHECK(("4[cm]")==(res.str()));
    res<<a;
    CHECK(("3[cm]")==(res.str()));
    res<<--a;
    CHECK(("2[cm]")==(res.str()));
    res<<a;
    CHECK(("2[cm]")==(res.str()));

}

TEST_CASE("Checking comparison operators"){
    std::stringstream res;
    NumberWithUnits a(2,"cm");
    NumberWithUnits b(3,"cm");
    NumberWithUnits c(2,"cm");
    CHECK((a==b)==(false));
    CHECK((a!=b)==(true));

    CHECK((a==c)==(true));
    CHECK((a!=c)==(false));

    CHECK((a<=c)==(true));
    CHECK((a<c)==(false));
    CHECK((a>=c)==(true));
    CHECK((a>c)==(false));

}


TEST_CASE("Checking plus minus between units"){
    std::stringstream res;
    NumberWithUnits a(2,"cm");
    NumberWithUnits b(3,"cm");
    NumberWithUnits c(2,"m");
    NumberWithUnits d(2,"km");
    
    res<<a+b;
    CHECK(("5[cm]")==(res.str()));
    res<<a-b;
    CHECK(("-1[cm]")==(res.str()));
    res<<c+a;
    CHECK(("2.02[m]")==(res.str()));
    res<<c-a;
    CHECK(("1.98[m]")==(res.str()));
    res<<a+c;
    CHECK(("202[cm]")==(res.str()));
    res<<a-c;
    CHECK(("198[cm]")==(res.str()));
    res<<d+c;
    CHECK(("2.002[km]")==(res.str()));
    res<<d-c;
    CHECK(("1.998[km]")==(res.str()));
}


TEST_CASE("Checking if throws wrong unmatching unit types"){
    std::stringstream res;
    NumberWithUnits a(2,"cm");
    NumberWithUnits b(3,"sec");
    NumberWithUnits c(2,"ton");
    NumberWithUnits d(2,"km");
    
    
    CHECK_THROWS(res<<a+b;);
    CHECK_THROWS(res<<a-b;);
    CHECK_THROWS(res<<c+a;);
    CHECK_THROWS(res<<c-a;);
    CHECK_THROWS(res<<a+c;);
    CHECK_THROWS(res<<a-c;);
    CHECK_THROWS(res<<d+c;);
    CHECK_THROWS(res<<d-c;);
}