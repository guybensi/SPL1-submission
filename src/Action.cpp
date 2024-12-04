#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"
using namespace std;
#include <iostream>
enum class SettlementType;
enum class FacilityCategory;
extern Simulation* backup;
//--------------------------------------------------------------------------------------
//Constructor
BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{return status;}

string BaseAction::StatusToString () const{//our method
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
    }
    return "";
}

void BaseAction::complete(){status = ActionStatus::COMPLETED;}

void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg; 
    cout << "ERROR: " << getErrorMsg() <<endl;
}
const string& BaseAction::getErrorMsg() const{return errorMsg;}


//--------------------------------------------------------------------------------------
//Constructor
SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation){
    if (numOfSteps <= 0) {
            error("Invalid number of steps: " + std::to_string(numOfSteps));
            return;
        }
    for (int i = 1; i <= numOfSteps; i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const{
    return "SimulateStep: " + std::to_string(numOfSteps) + " steps";
}

SimulateStep* SimulateStep::clone() const{return new SimulateStep(*this);}

string SimulateStep::description()const {//our method
    return "simulateStep " + std::to_string(numOfSteps) + " " + StatusToString();
}



//--------------------------------------------------------------------------------------
//Constructor
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName), selectionPolicy(selectionPolicy){}

void AddPlan::act(Simulation &simulation){
    Settlement* settlement = simulation.getSettlement(settlementName);     
    if (settlement == nullptr) {//settlement not found           
        error("Settlement " + settlementName + " not found.");
        return;
        }
    SelectionPolicy* TheSelectionPolicy = simulation.createSelectionPolicy(selectionPolicy);
    if (TheSelectionPolicy == nullptr) {//selection policy not found 
        error("Invalid selection policy: " + selectionPolicy);
        return;
    }
    simulation.addPlan(settlement, TheSelectionPolicy);
    complete();
}

const string AddPlan::toString() const{
            return "AddPlan: Settlement Name = " + settlementName + ", Selection Policy = " + selectionPolicy;
        }

AddPlan* AddPlan::clone() const{return new AddPlan(*this);}

string AddPlan::description()const {//our method
    return "Plan " + settlementName + " " + selectionPolicy + " " + StatusToString();
}



//--------------------------------------------------------------------------------------
//Constructor
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName), settlementType(settlementType){} 

void AddSettlement::act(Simulation &simulation){
    Settlement* newSettlement = new Settlement(settlementName, SettlementType (settlementType));
    if(!simulation.addSettlement(newSettlement)){
        error("Settlement: " + settlementName +"already exists");
        return;
    }  
    complete();
}

AddSettlement* AddSettlement::clone() const{return new AddSettlement(*this);}

string AddSettlement::SettlementTypeToString() const {//our method
    switch (settlementType) {
        case SettlementType::VILLAGE:
            return "0";
        case SettlementType::CITY:
            return "1";
        case SettlementType::METROPOLIS:
            return "2";
    }
    return "";
}

const string AddSettlement::toString() const{
    return "AddSettlement: Settlement Name = " + settlementName + ", Settlement Type = " + SettlementTypeToString();
}

string AddSettlement::description()const {//our method
    return "settlement " + settlementName + " " + SettlementTypeToString() + " " + StatusToString();
}


//--------------------------------------------------------------------------------------
//Constructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
        facilityName(facilityName), facilityCategory(facilityCategory), price(price),lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation){
    FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(!simulation.addFacility(newFacility)){
        error("Facility: " + facilityName + "already exists");
        return;
    }
    complete(); 
}

AddFacility* AddFacility::clone() const{return new AddFacility(*this);}

const string AddFacility::toString() const{
    return "AddFacility: Facility Name = " + facilityName + 
           ", Category = " + std::to_string(static_cast<int>(facilityCategory)) + 
           ", Price = " + std::to_string(price) + 
           ", LifeQualityImpact = " + std::to_string(lifeQualityScore) + 
           ", EconomyImpact = " + std::to_string(economyScore) + 
           ", EnvironmentImpact = " + std::to_string(environmentScore);
}

string AddFacility::description()const {//our method
    return "facility " + std::to_string(static_cast<int>(facilityCategory)) + " " + std::to_string(price) +" " + std::to_string(lifeQualityScore) +  " " +  std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + StatusToString();
}

//--------------------------------------------------------------------------------------
//Constructor
PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

void PrintPlanStatus::act(Simulation &simulation){
    if (planId < 0 || planId >= simulation.getplanCounter()) {//the id is not legal
        error("Plan: " + std::to_string(planId) + " "+ "doesn't exist");
        return; 
    }
    Plan& plan = simulation.getPlan(planId);  
    cout << plan.toString() << endl;
    complete();
}

PrintPlanStatus* PrintPlanStatus::clone() const{return new PrintPlanStatus(*this);}

const string PrintPlanStatus::toString() const{
    return "PrintPlanStatus: PlanID = " + std::to_string(planId);
}

string PrintPlanStatus::description()const {//our method
    return "planStatus " + std::to_string(planId) + " " + StatusToString();
}



//--------------------------------------------------------------------------------------
//Constructor

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId), newPolicy(newPolicy),previousPolicy(""){}

void ChangePlanPolicy::act(Simulation &simulation){
    if (planId < 0 || planId >= simulation.getplanCounter()) {//the id is not legal
        error("Plan: " + std::to_string(planId) + "doesn't exist");
        return; 
    }
    Plan& plan = simulation.getPlan(planId);
    string currPolicy = plan.getSelectionPolicy();
    if (newPolicy == currPolicy) {
        error("Curr policy and new polic are the same");
        return;
    }
    if (!plan.changePolicy(newPolicy)){
        error("Invalid selection policy");
        return; 
    }
    previousPolicy = currPolicy;
    complete();
}


ChangePlanPolicy* ChangePlanPolicy::clone() const{return new ChangePlanPolicy(*this);}

const string ChangePlanPolicy::toString() const{
    return "ChangePlanPolicy: PlanID = " + std::to_string(planId) + 
           ", Previous Policy = " + previousPolicy + 
           ", New Policy = " + newPolicy;
}

string ChangePlanPolicy::description()const {//our method
    return "changePolicy:: " + std::to_string(planId) + " " + newPolicy + " " + StatusToString();
}

//--------------------------------------------------------------------------------------

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation){
    for (BaseAction* curr : simulation.getlog()){
        cout << curr->description() << endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const{return new PrintActionsLog(*this);}

const string PrintActionsLog::toString() const{return description();}   

string PrintActionsLog::description()const {//our method
    return "log " + StatusToString();
}


//--------------------------------------------------------------------------------------

Close::Close(){}

void Close::act(Simulation &simulation){
    simulation.close();
}   

Close* Close::clone() const{return new Close(*this);}

const string Close::toString() const{return description();} 

string Close::description()const {//our method
    return "close " + StatusToString();
}

//--------------------------------------------------------------------------------------
BackupSimulation::BackupSimulation(){}

void BackupSimulation::act(Simulation &simulation) {
    // if the backup has a simulation we need to delete it first
    if (backup != nullptr) {
        *backup = simulation; 
    }
    else {backup = new Simulation(simulation);} 
    complete();
}

BackupSimulation* BackupSimulation::clone() const {return new BackupSimulation(*this);}

const string BackupSimulation::toString() const{return description();}

string BackupSimulation::description()const {//our method
    return "backup " + StatusToString();
}

//--------------------------------------------------------------------------------------
RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation){
    if (backup == nullptr) {
        error("No backup available");
    } else {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation* RestoreSimulation::clone() const{return new RestoreSimulation(*this);}

const string RestoreSimulation::toString() const{return description();}

string RestoreSimulation::description()const {//our method
    return "restore " + StatusToString();
}


