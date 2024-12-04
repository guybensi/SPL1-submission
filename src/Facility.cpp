
#include <string>
#include <vector>
#include "Facility.h"
#include <iostream>
using std::string;
using std::vector;
using namespace std;

//Constructor
FacilityType ::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
 name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}
//Copy Constructor
FacilityType ::FacilityType(const FacilityType&other):FacilityType(other.name, other.category, other.price, other.lifeQuality_score, other.economy_score, other.environment_score){}//copy constractor
//Copy Assignment operator
FacilityType& FacilityType::operator=(const FacilityType &other){return *this;}

const string & FacilityType ::getName() const {return name;}

int FacilityType ::getCost() const {return price;}

int FacilityType ::getLifeQualityScore() const {return lifeQuality_score;}

int FacilityType ::getEnvironmentScore() const {return environment_score;}

int FacilityType ::getEconomyScore() const {return economy_score;}

FacilityCategory FacilityType ::getCategory() const {return category;}

FacilityType ::~FacilityType(){}//destcractor


//--------------------------------------------------------------------------------------

//Constructor
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), 
      settlementName(settlementName), 
      status(FacilityStatus::UNDER_CONSTRUCTIONS), 
      timeLeft(price) {}

//Second Constructor
Facility ::Facility(const FacilityType &type, const string &settlementName):
FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}
//Copy Constructor
Facility ::Facility(const Facility &other): Facility(other.name, other.getSettlementName(), other.category, other.price, other.lifeQuality_score, other.economy_score, other.environment_score){}//copy constractor
//Copy Assignment operator
Facility& Facility ::operator=(const Facility &other){return *this;}//copy assignment operator

const string & Facility ::getSettlementName() const {return settlementName;}

const int Facility ::getTimeLeft() const {return timeLeft;}

FacilityStatus Facility ::step(){
    if (timeLeft > 0) {
        timeLeft--;
    }
    if (timeLeft == 0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return getStatus();
}

void  Facility ::setStatus(FacilityStatus status){
    this->status = status;
}

const FacilityStatus& Facility ::getStatus() const {return status;}

const string Facility ::statusToString() const {
    string stirngStatus;
    switch (this->status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            stirngStatus = "UNDER_CONSTRUCTIONS";
            break;
        case FacilityStatus::OPERATIONAL:
            stirngStatus = "OPERATIONAL";
            break;
    }
    return stirngStatus;
}

const string Facility ::toString() const{
    string CurrStatus;
    switch (this->status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            CurrStatus = "UNDER_CONSTRUCTIONS";
            break;
        case FacilityStatus::OPERATIONAL:
            CurrStatus = "OPERATIONAL";
            break;
    }
    string CurrCategory;
    switch (this->category) {
        case FacilityCategory::LIFE_QUALITY:
            CurrCategory = "LIFE_QUALITY";
            break;
        case FacilityCategory::ECONOMY:
            CurrCategory = "ECONOMY";
            break;
        case FacilityCategory::ENVIRONMENT:
            CurrCategory = "ENVIRONMENT";
            break;
    }
     return "Name: " + this->name + " | " + 
       "Settlement name: " + this->settlementName + " | " + 
       "Category: " + CurrCategory + " | " + 
       "Status: " + CurrStatus + " | " +
       "Price: " + std::to_string(this->price) + " | " +
       "Life Quality Score: " + std::to_string(this->lifeQuality_score) + " | " +
       "Economy Score: " + std::to_string(this->economy_score) + " | " +
       "Environment Score: " + std::to_string(this->environment_score) + " | " +
       "Time Left: " + std::to_string(this->timeLeft);
}

Facility::~Facility(){};//destcractor

