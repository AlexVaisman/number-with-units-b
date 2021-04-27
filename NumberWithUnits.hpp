#pragma once
#include <string>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>



namespace ariel{
    class NumberWithUnits{
        private:
        //Variables
        double value_in ;
        std::string unit_in;
        double CONV_RATE = 1;
        static const int RUN_LIMIT1 = 100;
        static const int RUN_LIMIT2 = 200;
        static const int MAP_LIMIT = 1000;
        //Maps
        typedef std::pair<std::string, double> unit;
        static std::multimap<std::string, unit> map_units;

        //Functions
        static void check_legal_unit(std::string unit);
        static std::pair<double , bool> find_convertion(const NumberWithUnits& a ,const NumberWithUnits& b);
        static std::pair<double , bool> check_sons(std::string father, std::string target);


        public:
        NumberWithUnits(double value_in , std::string unit_in);
        static void read_units(std::ifstream&  file);


        //Operators
        //Plus (+) operators
        friend const NumberWithUnits operator+ (const NumberWithUnits& a, const NumberWithUnits& b);
        NumberWithUnits& operator+= ( const NumberWithUnits& b);
        friend NumberWithUnits operator+(const NumberWithUnits & b);
        NumberWithUnits& operator++(); //prefix
        const NumberWithUnits operator++(int); //postfix

        //Minus (-)operators
        friend const NumberWithUnits operator- (const NumberWithUnits& a, const NumberWithUnits& b);
        NumberWithUnits& operator-= ( const NumberWithUnits& b);
        friend NumberWithUnits operator-(const NumberWithUnits & b);
        NumberWithUnits& operator--(); //prefix
        const NumberWithUnits operator--(int); //postfixvvv

        //Comparison operators
        friend bool operator==(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator!=(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator<=(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator>=(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator<(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator>(const NumberWithUnits& a, const NumberWithUnits& b);
        
        //Multiplication 
        friend const NumberWithUnits operator* (const NumberWithUnits& a, const double& b);
        friend const NumberWithUnits operator* (const double& a, const NumberWithUnits& b);

        //Input/Output
        friend std::ostream& operator<< (std::ostream& os, const NumberWithUnits& a);
        friend std::istream& operator>> (std::istream& is, NumberWithUnits& a);

    };

}

