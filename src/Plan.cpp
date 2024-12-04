
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
#include <iostream>
using namespace std;
using std::vector;

//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE) , facilities(), underConstruction(),facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){}

// Copy Constructor
Plan::Plan(const Plan &other):Plan(other.plan_id, other.settlement, other.selectionPolicy->clone(), other.facilityOptions){
    status = other.status;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();
    for (Facility* facility : other.underConstruction) {
        addFacility(new Facility (*facility));
    }
    for (Facility* facility : other.facilities) {
        addFacility(new Facility (*facility));
    }  
}

//Copy Constructor for copy simulation
Plan::Plan(const Settlement &settlement, const Plan &other):Plan(other.plan_id, settlement, other.selectionPolicy->clone(), other.facilityOptions){
    status = other.status;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();
}

// Copy Assignment Operator
Plan& Plan::operator=(const Plan &other) {
    if (this != &other) {
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        selectionPolicy = (other.selectionPolicy != nullptr) ? other.selectionPolicy->clone() : nullptr;
        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();
        for (Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        }
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();
        for (Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }
    }
    return *this;
}

// move constractor
Plan::Plan(Plan &&other)
: Plan(other.plan_id, other.settlement, other.selectionPolicy, other.facilityOptions)
{
    status = other.status;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();

    // Deep copy of the Facility lists (shallow copy of the pointers will lead to issues)
    // Make sure to copy the actual Facility objects and not just the pointers
    facilities.clear();
    for (Facility* facility : other.facilities) {
        addFacility(facility);
    }

    underConstruction.clear();
    for (Facility* facility : other.underConstruction) {
        addFacility(facility);
    }

    // Now reset other object's data after move
    other.facilities.clear(); 
    other.underConstruction.clear();
    other.selectionPolicy = nullptr; 
}

// Move Assignment Operator
Plan& Plan::operator=(Plan &&other) {
    if (this != &other) {
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        selectionPolicy = other.selectionPolicy;
        other.selectionPolicy = nullptr;

        facilities.clear();
        for (Facility* facility : other.facilities) {
            addFacility(facility);
        }

        underConstruction.clear();
        for (Facility* facility : other.underConstruction) {
            addFacility(facility);
        }

        other.facilities.clear();
        other.underConstruction.clear();
    }
    return *this;
}

const int Plan::getlifeQualityScore() const {return life_quality_score;}

const int Plan::getEconomyScore() const {return economy_score;}

const int Plan::getEnvironmentScore() const {return environment_score;}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if (this->selectionPolicy != selectionPolicy) {
        delete this->selectionPolicy;//delete the curr selection policy
        this->selectionPolicy = selectionPolicy;
    }
}

bool Plan::changePolicy(const string newPolicy){//our method
    if (newPolicy == "eco") {
        setSelectionPolicy(new EconomySelection());
        return true; 
    } else if (newPolicy == "env") {
        setSelectionPolicy(new SustainabilitySelection());
        return true;
    } else if (newPolicy == "nve") {
        setSelectionPolicy(new NaiveSelection());
        return true;
        //for balace we need to sent the curr scores and the scores in under constraction
    } else if (newPolicy == "bal") {
        int newEcoScore = getEconomyScore();
        int newLifeScore = getlifeQualityScore();
        int newEnvDcore = getEnvironmentScore();
        for (Facility* FcilityUnderConstruction : underConstruction){
            newEcoScore += FcilityUnderConstruction->getEconomyScore();
            newLifeScore += FcilityUnderConstruction->getLifeQualityScore();
            newEnvDcore += FcilityUnderConstruction->getEnvironmentScore();
        }
        setSelectionPolicy (new BalancedSelection (newLifeScore, newEcoScore, newEnvDcore));
        return true;    
    }else {
        return false;
    }
}

string Plan::getSelectionPolicy() const {//our method
    if (dynamic_cast<NaiveSelection*>(selectionPolicy)) {
        return "nve";
    } else if (dynamic_cast<BalancedSelection*>(selectionPolicy)) {
        return "bal";
    } else if (dynamic_cast<EconomySelection*>(selectionPolicy)) {
        return "eco";
    } else if (dynamic_cast<SustainabilitySelection*>(selectionPolicy)) {
        return "env";
    }else {return "";}
}

void Plan::step(){
    //as long as i can build more
    while (status == PlanStatus::AVALIABLE){
        Facility *selectedFacility = new Facility(this->selectionPolicy->selectFacility(facilityOptions),this->settlement.getName());
        addFacility(selectedFacility);
        if (underConstruction.size() == static_cast<size_t>(this->settlement.getLimit())) {
            this->status = PlanStatus::BUSY; 
        }
    }
    // doing a step for all the facilities
    for (size_t i = 0; i < underConstruction.size();){
        Facility* currFacility = underConstruction[i];
        FacilityStatus currStat = currFacility->step();
        if (currStat == FacilityStatus::OPERATIONAL){
            addFacility(currFacility);
            scoreUpdate(currFacility);
            underConstruction.erase(underConstruction.begin() + i);
        }
        else{i++;}

    }
    //maybe the plan can be avalible
    if (underConstruction.size() < static_cast<size_t>(this->settlement.getLimit())){
        this->status = PlanStatus::AVALIABLE;
    }
}

void Plan::printStatus(){
    if (status == PlanStatus::AVALIABLE) {cout << "plan id: " << plan_id << "settlement: " << settlement.getName() <<  "Status: Available" << endl;}
    else {cout << "plan id: " << plan_id << "settlement: " << settlement.getName() << "Status: Busy" << endl;}
 }

const vector<Facility*>& Plan::getFacilities() const{return facilities;}

void Plan::addFacility(Facility* facility){
    if(facility->getStatus() == FacilityStatus::OPERATIONAL){
            facilities.push_back(facility);
    }
    else{
        underConstruction.push_back(facility);
    }  
}

const string Plan::toString() const {
    string statusString;
    string result = "PlanID: " + std::to_string(plan_id) + "\n";
    result = result + "SettlementName: " + settlement.getName() + "\n"; 
    switch (status) {
        case PlanStatus::AVALIABLE:
            statusString =  "AVALIABLE";
            break;
        case PlanStatus::BUSY:
            statusString = "BUSY";
            break;
    }
    if (status == PlanStatus::AVALIABLE) {
        result = result +  "Status: Available" + "\n";
    } else {
        result = result + "Status: Busy" + "\n";
    }
    result += "SelectionPolicy: " + selectionPolicy->getName() + "\n"; 
    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score) + "\n";
    result += "Under Construction:";
    result +=  "\n";
    for (Facility* facility : underConstruction) {
        result += "Facility Name: " + facility->getName() + "\n";
        result += "FacilityN Status: " + facility->statusToString() + "\n";
    }
    result += "\nFacilities:";
    result += "\n";
    for (Facility* facility : facilities) {
        result += "Facility Name: " + facility->getName() + "\n";
        result += "FacilityN Status: " + facility->statusToString() + "\n";
    }
    return result;
}


void Plan::scoreUpdate(Facility* facility) {// our method
    this->life_quality_score += facility->getLifeQualityScore();
    this->economy_score += facility->getEconomyScore();  
    this->environment_score += facility->getEnvironmentScore();  
}

const int Plan::getId(){return plan_id;}// our method

Settlement Plan::getSettlement(){return settlement;}//our method

// Destructor
Plan::~Plan() {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    for (Facility* facility : facilities) {
        delete facility;
    }
}


