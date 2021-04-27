#include "NumberWithUnits.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <math.h>



using namespace std;
namespace ariel{

    //declarations
    std::multimap<std::string, NumberWithUnits::unit> NumberWithUnits::map_units;
    istream& check_char(istream& input, char expectedChar);

    
    NumberWithUnits::NumberWithUnits(double value_in , std::string unit_in){
        check_legal_unit(unit_in);
        this->value_in = value_in;
        this->unit_in = unit_in;
    };

    
    //Reading units from file and puting them into two maps to save the relation
    //between the unints and the covertion rates.
    void NumberWithUnits::read_units(std::ifstream& file){
        double first_val;
        double second_val;
        string first_unit_name;
        string second_unit_name;
        string eq;
        

        while(file>>first_val>>first_unit_name>>eq>>second_val>>second_unit_name){
            //Putting units into map with covertions rates
            NumberWithUnits::unit databigger(second_unit_name,second_val);
            NumberWithUnits::unit datasmaller(first_unit_name,1/second_val);

            map_units.insert(make_pair(first_unit_name,databigger));
            map_units.insert(make_pair(second_unit_name,datasmaller));
        }
        file.close();//closing file
        //Finding all connections between units of the same group and adding them to map
        vector<NumberWithUnits::unit> cont;
        vector<string>names;
        for(auto& it:map_units){
            auto range = map_units.equal_range(it.second.first);
            for(auto itr = range.first ; itr != range.second;itr++){
                 if(it.first!=itr->second.first && it.first!=it.second.first && it.second.first!= itr->second.first){
                    NumberWithUnits::unit databigger(itr->second.first, it.second.second*itr->second.second);
                    cont.push_back(databigger);
                    names.push_back(it.first);
                    
                 }
            }  
        }
        for(unsigned long i = 0 ;i<cont.size();i++){
            map_units.insert(make_pair(names.at(i),cont.at(i)));
        }

    
        //Finding deeper connections between nodes
        cont.clear();
        names.clear();
        int first = 0,second =0;
        for(auto& it:map_units){
            auto range = map_units.equal_range(it.second.first);
            for(auto itr = range.first ; itr != range.second;itr++){
                auto child = map_units.equal_range(itr->second.first);
                if(first>NumberWithUnits::RUN_LIMIT1){continue;}
                for(auto child_it = child.first;child_it!=child.second;child_it++){
                    if(second>NumberWithUnits::RUN_LIMIT2){continue;}
                    if(it.first!=child_it->second.first){
                        if(cont.size()<NumberWithUnits::MAP_LIMIT && map_units.size()<1000){
                            NumberWithUnits::unit databigger(child_it->second.first, it.second.second*itr->second.second*child_it->second.second);
                            cont.push_back(databigger);
                            names.push_back(it.first);
                        }
                    }
                    second++;
                }
                first++;
            }  
        }//inserting units into map.
        for(unsigned long i = 0 ;i<cont.size();i++){
            map_units.insert(make_pair(names.at(i),cont.at(i)));
        }
        cont.clear();
        names.clear();
    };

    //Checking if unit is a legal unit
    //if not will throw exception
    void NumberWithUnits::check_legal_unit(string unit){
        if(map_units.find(unit)!=map_units.end()){
            return;
        }
        throw invalid_argument( unit+" -is a wrong unit type" );
    }

    //find the converstion rate of units.
    pair<double , bool> NumberWithUnits::find_convertion(const NumberWithUnits& a ,const NumberWithUnits& b){
        string unit_runner = a.unit_in;
        double conv = a.CONV_RATE;

        if(a.unit_in == b.unit_in){ // same units
            pair<double , bool> data(conv,true);
            return data;
        }
        auto range = map_units.equal_range(a.unit_in);
        for(auto it = range.first;it!=range.second;it++){
            if(it->second.first==b.unit_in){
                pair<double, bool> data(it->second.second ,true);
                return data;
            }
        }
        
        throw invalid_argument("Units are not of the same type.");
    }

    const NumberWithUnits operator+ (const NumberWithUnits& a, const NumberWithUnits& b){
        pair<double , bool> data = NumberWithUnits::find_convertion(a,b);
        if(data.second == true ){                // a > b
            NumberWithUnits ans(a.value_in+b.value_in/data.first,a.unit_in);
            return ans;
            
        }else{                                  // b > a
            NumberWithUnits ans(a.value_in+b.value_in*data.first,a.unit_in);
            return ans;
        }
        return a;
    };

    NumberWithUnits& NumberWithUnits::operator+= ( const NumberWithUnits& b){
        NumberWithUnits a(this->value_in,this->unit_in);
        NumberWithUnits ans = a+b;
        this->unit_in = ans.unit_in;
        this->value_in = ans.value_in;
        return *this;
    };
    NumberWithUnits operator+(const NumberWithUnits & b){
        return b;
    };
    NumberWithUnits& NumberWithUnits::operator++(){
        this->value_in++;
        return *this;
    }; 
    
    const NumberWithUnits NumberWithUnits::operator++(int){
        NumberWithUnits copy(this->value_in,this->unit_in);
        ++*this;
        return copy;
    };

    const NumberWithUnits operator- (const NumberWithUnits& a, const NumberWithUnits& b){
        pair<double , bool> data = NumberWithUnits::find_convertion(a,b);
        if(data.second == true){                // a > b
            NumberWithUnits ans(a.value_in-b.value_in/data.first,a.unit_in);
            return ans;
            
        }else{                                  // b > a
            NumberWithUnits ans(a.value_in-b.value_in*data.first,a.unit_in);
            return ans;
        }
        return a;
    };
    NumberWithUnits& NumberWithUnits::operator-= ( const NumberWithUnits& b){
        NumberWithUnits a(this->value_in,this->unit_in);
        NumberWithUnits ans = a-b;
        this->unit_in = ans.unit_in;
        this->value_in = ans.value_in;
        return *this;
        }
    NumberWithUnits operator-(const NumberWithUnits & b){
        return NumberWithUnits (-b.value_in,b.unit_in);
    }
    NumberWithUnits& NumberWithUnits::operator--(){
        this->value_in--;
        return *this;
    };
    const NumberWithUnits NumberWithUnits::operator--(int){
        NumberWithUnits copy(this->value_in,this->unit_in);
        --*this;
        return copy;
    };


    bool operator==(const NumberWithUnits& a, const NumberWithUnits& b){
        if(a.unit_in==b.unit_in){
            if(a.value_in==b.value_in){
                return true;
            }
        }

        NumberWithUnits group = a+b; // if not same group will throw exeption.
        group.value_in = 0;
        group = group+a;
        double dif = group.value_in;
        group.value_in = 0;
        group = group+b;
        double dif2 = group.value_in;

        if(dif-dif2<0.00001 &&dif-dif2>-0.00001){
            return true;
        }
        else{
            return false;
        }
        return false;
    };
    bool operator!=(const NumberWithUnits& a, const NumberWithUnits& b){
        if(a==b){
            return false;
        }
        return true;
    };
    bool operator<=(const NumberWithUnits& a, const NumberWithUnits& b){
        NumberWithUnits group = a+b; // if not same group will throw exeption.
        group.value_in = 0;
        group = group+a;
        double dif = group.value_in;
        group.value_in = 0;
        group = group+b;
        double dif2 = group.value_in;
        
        if(dif<=dif2){
            return true;
        }
        else{
            return false;
        }
    };
    bool operator>=(const NumberWithUnits& a, const NumberWithUnits& b){
        if(a==b){
            return true;
        }
        else if(a>b){
            return true;
        }
        return false;
    };
    bool operator<(const NumberWithUnits& a, const NumberWithUnits& b){
        NumberWithUnits group = a+b; // if not same group will throw exeption.
        group.value_in = 0;
        group = group+a;
        double dif = group.value_in;
        group.value_in = 0;
        group = group+b;
        double dif2 = group.value_in;


        //rounding up decimal to 3 places.
        double epsi = dif - dif2;
        epsi = round(1000 * epsi) / 1000;
        if(epsi==-0){
            epsi=0;
            return false;
        }

        if(dif<dif2){
            return true;
        }
        else{
            return false;
        }
    };
    bool operator>(const NumberWithUnits& a, const NumberWithUnits& b){
        NumberWithUnits group = a+b; // if not same group will throw exeption.
        group.value_in = 0;
        group = group+a;
        double dif = group.value_in;
        group.value_in = 0;
        group = group+b;
        double dif2 = group.value_in;
        
        if(dif>dif2){
            return true;
        }
        return false;
    };


    const NumberWithUnits operator* (const NumberWithUnits& a, const double& b){
        NumberWithUnits ans = a;
        ans.value_in = ans.value_in*b;
        return ans;
    };
    const NumberWithUnits operator* (const double& a, const NumberWithUnits& b){
        return b*a;
    };

    std::ostream& operator<< (std::ostream& os, const NumberWithUnits& a){
        os<<a.value_in<<"["<<a.unit_in<<"]";
        return os;
    };
    std::istream& operator>> (std::istream& is, NumberWithUnits& a){ // Based on erels function from operator lecture.
        double value = 0;
        string unit;


        if((!(is>>value)) || ((!check_char(is,'[')))){

            if(is.badbit!=0){
                throw invalid_argument("Wrong format , correct format is: double[unit]");
            }
        }
        char currentchar = ' ';
        while(currentchar!=']'){
            is>>currentchar;
            if(currentchar!=']'){
                unit += currentchar;
            }
        }
        NumberWithUnits ans (value,unit);
        a = ans;
        return is;
    };

    istream& check_char(istream& input, char expectedChar) {         // Based on erels function from operator lecture.
        char actualChar = ' ';
        input >> actualChar;
       
        if (!input) {
            return input;
        }
        if (actualChar!=expectedChar) {
        input.setstate(ios::failbit);
    }
    return input;
    }
  
};


