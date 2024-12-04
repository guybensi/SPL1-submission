#include <vector>
#include "Facility.h"
#include "../include/SelectionPolicy.h"
#include <algorithm>
#include <initializer_list>
#include <climits>
#include <stdexcept>
#include <iostream>
using namespace std;
using std::vector;


//Constructor
NaiveSelection::NaiveSelection():lastSelectedIndex(-1){} 

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
 if (facilitiesOptions.empty()) {
        throw std::out_of_range("Vector is empty");
    }
    lastSelectedIndex++;
    return facilitiesOptions[(lastSelectedIndex % facilitiesOptions.size())];
}

const string NaiveSelection ::toString() const{ 
    return "The last selected index was: " + std :: to_string(lastSelectedIndex);
}

 NaiveSelection* NaiveSelection ::clone() const{
    return new NaiveSelection(*this); 
}
string NaiveSelection::getName() const{return "nve";}//our method



//--------------------------------------------------------------------------------------

//Constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available to select ");} 
    int diff = INT_MAX;
    const FacilityType* ans = nullptr;
    //we want to choose the facility wich for whom the distance will be minimal
    for (const FacilityType& currfacility : facilitiesOptions){
        int distancecheck = distance (currfacility);
        if ( distancecheck < diff){
            ans = &currfacility;
            diff = distancecheck;
        } 
    }
    //we need to update the scores for everyfacility in UnderConstraction
    LifeQualityScore += ans->getLifeQualityScore();
    EconomyScore += ans->getEconomyScore();
    EnvironmentScore += ans->getEnvironmentScore();
    return *ans;
}

int BalancedSelection ::distance (const FacilityType &CurrFacility){
    int NewLife = this->LifeQualityScore + CurrFacility.getLifeQualityScore();
    int NewEco = this->EconomyScore + CurrFacility.getEconomyScore();
    int NewEnv = this->EnvironmentScore + CurrFacility.getEnvironmentScore();
    return std::max({NewLife, NewEco, NewEnv}) - std::min({NewLife, NewEco, NewEnv});
}

const string BalancedSelection::toString() const {
    return "Balanced selection with scores: " +
           std::to_string(LifeQualityScore) + ", " +
           std::to_string(EconomyScore) + ", " +
           std::to_string(EnvironmentScore);
}

    
BalancedSelection * BalancedSelection :: clone() const{
    return new BalancedSelection(*this);
}
string BalancedSelection::getName() const{return "bal";}//our method

//--------------------------------------------------------------------------------------

//Constructor
EconomySelection::EconomySelection():lastSelectedIndex(-1){}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    for (std::size_t i = 1;i<=facilitiesOptions.size(); i++){
        int releventIndex = (lastSelectedIndex + i) % facilitiesOptions.size();
        if (facilitiesOptions[releventIndex].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = releventIndex;
            return facilitiesOptions[releventIndex];
        }
    }
     throw std::runtime_error("No eco facility found"); 
}

const string EconomySelection::toString() const {
    return "The last selected index was: " + std::to_string(lastSelectedIndex);
}


EconomySelection * EconomySelection::clone() const{
    return new EconomySelection(*this); 
}
string EconomySelection::getName() const{return "eco";}//our method



//--------------------------------------------------------------------------------------

//Constructor
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1){}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
        for (std::size_t i = 1;i<=facilitiesOptions.size(); i++){
        int releventIndex = (lastSelectedIndex + i) % facilitiesOptions.size();
        if (facilitiesOptions[releventIndex].getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = releventIndex;
            return facilitiesOptions[releventIndex];
        }
    }
     throw std::runtime_error("No environment facility found");
}

const string SustainabilitySelection::toString() const{
    return "The last selected index was: " + std :: to_string(lastSelectedIndex);
}

SustainabilitySelection *SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
}
string SustainabilitySelection::getName() const{return "env";}//our method


