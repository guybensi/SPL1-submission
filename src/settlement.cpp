
#include <string>
#include <vector>
#include "Settlement.h"
using std::string;
using std::vector;

//Constructor
Settlement ::Settlement(const string &name, SettlementType type): name(name), type(type),limit(){
    switch (type) {
        case SettlementType::VILLAGE:
            limit = 1;
            break;
        case SettlementType::CITY:
            limit = 2;
            break;
        case SettlementType::METROPOLIS:
            limit = 3;
            break;
    }
}

const string& Settlement ::getName() const{return name;}

SettlementType Settlement ::getType() const{return type;}


const string Settlement::settlementTypeToString() const{
    switch (type) {
        case SettlementType::VILLAGE:
            return "Village";
        case SettlementType::CITY:
            return "City";
        case SettlementType::METROPOLIS:
            return "Metropolis";
        default:
            return "Unknown";
    }
}

const string Settlement ::toString() const{
    return "Name: " + name + ", Type: " + settlementTypeToString();
}

int Settlement::getLimit()const {return limit;}
