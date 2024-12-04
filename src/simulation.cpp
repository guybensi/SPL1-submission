
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "Action.h"
#include <bits/algorithmfwd.h>
#include "Auxiliary.h"
#include <iostream>
using namespace std;
using std::string;
using std::vector;
#include <fstream>

//Constructor
Simulation::Simulation(const string &configFilePath):isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions(){
    // reading from the file
    std::ifstream file(configFilePath);
    if (!file.is_open()) {throw std::runtime_error("Failed to open config file.");}
    string line;
    while (std::getline(file, line)) {
        // if there is # ignore this line
        if (line.empty() || line[0] == '#'){continue;}
        // using the given class
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);
        if (arguments[0] == "settlement"){//creating a new settlement
            if (arguments.size() != 3){throw std::runtime_error("Invalid settlement format in config file");}
            string name = arguments[1];
            SettlementType type = static_cast<SettlementType>(std::stoi(arguments[2]));
            if(!addSettlement(new Settlement(name, type))){throw std::runtime_error("Invalid settlement");}
            
        }
        else if (arguments[0] == "facility"){//creating a new facility
            if (arguments.size() != 7){throw std::runtime_error("Invalid facility format in config file");}
            string name = arguments[1];
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(arguments[2]));
            int price = std::stoi(arguments[3]);
            int lifeQualityScore = std::stoi(arguments[4]);
            int ecoScore = std::stoi(arguments[5]);
            int envScore = std::stoi(arguments[6]);
            if(!addFacility(FacilityType(name, category, price, lifeQualityScore, ecoScore, envScore))){throw std::runtime_error("Invalid facility");}      
        }
        else if (arguments[0] == "plan"){//creating a new plan
            if (arguments.size() != 3){throw std::runtime_error("Invalid plan format in config file");}
            string settlementName = arguments[1];   
            Settlement *settlement = getSettlement(settlementName);
            string policyType = arguments[2];  
            SelectionPolicy *selectionPolicy = createSelectionPolicy(policyType);
            addPlan(settlement, selectionPolicy);
        }
    }
    file.close();
}

// Copy Constructor
Simulation::Simulation(const Simulation &other):isRunning(other.isRunning), planCounter(other.planCounter),actionsLog(), plans(),settlements(), facilitiesOptions(other.facilitiesOptions){ 
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (Settlement* curr : other.settlements) {
        settlements.push_back(new Settlement(*curr));
    }
    for(Plan plan : other.plans){
        string settlementName = plan.getSettlement().getName();
        Settlement *newSettelemnet = getSettlement(settlementName); 
        Plan newPlan = Plan(*newSettelemnet,plan);
        plans.push_back(newPlan);
    }
    planCounter = other.planCounter;
    isRunning = other.isRunning; 
    facilitiesOptions = other.facilitiesOptions;  
}

// Copy Assignment Operator
Simulation& Simulation::operator=(const Simulation &other){ 
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;
         for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear(); 
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        } 
         for (Settlement* currSet : settlements) {
            delete currSet;
        }
        settlements.clear(); 
        for (Settlement* currSet : other.settlements) {
            settlements.push_back(new Settlement(*currSet));
        } 
        plans.clear();
        for(Plan plan : other.plans){
            string settlementName = plan.getSettlement().getName();
            Settlement *newSettelemnet = getSettlement(settlementName); 
            Plan newPlan = Plan(*newSettelemnet,plan);
            plans.push_back(newPlan);
        }     
    } 
    return *this;
}
//--------------------------------------------------------------------------------------
void Simulation::start() {
    open();
    cout << "The simulation has atarted" << endl;
    string line;
    while (isRunning) {
        //first command
        cout << "Enter command: ";
        std::getline(cin, line);
        
        // using the given class
        vector<string> arguments = Auxiliary::parseArguments(line);
        if (arguments.empty()) continue;
        //Simulate Step
        if (arguments[0] == "step"){
            if (arguments.size() != 2){
                cout << "Invalid action" << endl;
                continue;
            }
            int numOfSteps = std::stoi(arguments[1]);
            SimulateStep* newAction = new SimulateStep(numOfSteps);
            newAction->act(*this);
            addAction(newAction);
        }
        //Add plan
        else if (arguments[0] == "plan"){
            if (arguments.size() != 3){
                cout << "Invalid action" << endl;
                continue;
            }
            AddPlan* newAction = new AddPlan(arguments[1], arguments[2]);
            newAction->act(*this);
            addAction(newAction);
        }
        //Add settlement
        else if (arguments[0] == "settlement"){
            if (arguments.size() != 3){
                cout << "Invalid action" << endl;
                continue;
            }
            const int type = std::stoi(arguments[2]);
            SettlementType currType;
            switch (type){
                case 0:
                    currType = SettlementType::VILLAGE;
                    break;
                case 1:
                    currType = SettlementType::CITY;
                    break;
                case 2:
                    currType = SettlementType::METROPOLIS;
                    break;
            }
            AddSettlement* newAction = new AddSettlement(arguments[1], currType);
            newAction->act(*this);
            addAction(newAction);
        }
        
        //Add facility
        else if (arguments[0] == "facility") {
            if (arguments.size() != 7) {
                cout << "Invalid action" << endl;
                continue;
            }
            string name = arguments[1];
            const int givenCategory = std::stoi(arguments[2]);
            FacilityCategory category;
            switch (givenCategory){
                case 0:
                    category = FacilityCategory::LIFE_QUALITY;
                    break;
                case 1:
                    category = FacilityCategory::ECONOMY;
                    break;
                case 2:
                    category = FacilityCategory::ENVIRONMENT;
                    break;
            }
            int price = std::stoi(arguments[3]);
            int lifeQualityScore = std::stoi(arguments[4]);
            int ecoScore = std::stoi(arguments[5]);
            int envScore = std::stoi(arguments[6]);
            AddFacility* newAction = new AddFacility(name, category, price, lifeQualityScore, ecoScore, envScore);
            newAction->act(*this);
            addAction(newAction);
        }
        //print plan status
        else if (arguments[0] == "planStatus"){
            if (arguments.size() != 2){
                cout << "Invalid action" << endl;
                continue;
            }
            int id = std::stoi(arguments[1]);
            PrintPlanStatus* newAction = new PrintPlanStatus(id);
            newAction->act(*this);
            addAction(newAction);
        }
        //Change Plan Policy
        else if (arguments[0] == "changePolicy"){
            if (arguments.size() != 3){
                cout << "Invalid action" << endl;
                continue;
            }
            int id = std::stoi(arguments[1]);
            ChangePlanPolicy* newAction = new ChangePlanPolicy(id, arguments[2]);
            newAction->act(*this);
            addAction(newAction);
        }
        //Print Actions Log
        else if (arguments[0] == "log"){
            if (arguments.size() != 1){
                cout << "Invalid action" << endl;
                continue;
            }
            PrintActionsLog* newAction = new PrintActionsLog();
            newAction->act(*this);
            addAction(newAction);
        }
        //Close
        else if (arguments[0] == "close"){
            if (arguments.size() != 1){
                cout << "Invalid action" << endl;
                continue;
            }
            Close* newAction = new Close();
            newAction->act(*this);
            addAction(newAction);
        }
        //Backup Simulation
        else if (arguments[0] == "backup"){
            if (arguments.size() != 1){
                cout << "Invalid action" << endl;
                continue;
            }
            BackupSimulation* newAction = new BackupSimulation();
            newAction->act(*this);
            addAction(newAction);
        }
        //RestoreSimulation
        else if (arguments[0] == "restore"){
            if (arguments.size() != 1){
                cout << "Invalid action" << endl;
                continue;
            }
            RestoreSimulation* newAction = new RestoreSimulation();
            newAction->act(*this);
            addAction(newAction);
        }
        //unknown command
        else {
            cout << "Unknown command." << endl;
        }
    }
}

 //--------------------------------------------------------------------------------------
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter, *settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}
//--------------------------------------------------------------------------------------
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}
//--------------------------------------------------------------------------------------
bool Simulation::addSettlement(Settlement *settlement){
    if (isSettlementExists(settlement->getName())){return false;}
    settlements.push_back(settlement);
    return true;
}
//--------------------------------------------------------------------------------------
bool Simulation::addFacility(FacilityType facility){
    if (isFacilityExists(facility.getName())){return false;}
    facilitiesOptions.push_back(facility);
    return true;
}
//--------------------------------------------------------------------------------------
bool Simulation::isFacilityExists(const string &facilityName){
    for (const FacilityType curr : facilitiesOptions){
        if (curr.getName() == facilityName){return true;}
    }
    return false; 
}
//--------------------------------------------------------------------------------------
bool Simulation::isSettlementExists(const string &settlementName){
    for (const Settlement* curr : settlements){
        if (curr->getName() == settlementName){return true;}
    }
    return false; 
}
//--------------------------------------------------------------------------------------
Settlement* Simulation::getSettlement(const string &settlementName){
    for (Settlement* curr : settlements){
        if (curr->getName() == settlementName){return curr;}
    }
    return nullptr;
}
//--------------------------------------------------------------------------------------
vector<BaseAction*>& Simulation::getlog(){return actionsLog;}//our method
//--------------------------------------------------------------------------------------
Plan& Simulation::getPlan(const int planID){
    return plans[planID];
}
//--------------------------------------------------------------------------------------
int Simulation::getplanCounter(){return planCounter;}//our method
//--------------------------------------------------------------------------------------
void Simulation::step(){
    for (Plan &p : plans){
        p.step();
    }
}
//--------------------------------------------------------------------------------------
void Simulation::close(){
    for (const Plan& curr : plans){
        cout << curr.toString() << endl;
        cout << "\n" << endl;
    }
    isRunning = false;
}
//--------------------------------------------------------------------------------------
void Simulation::open(){isRunning = true;}

//--------------------------------------------------------------------------------------
SelectionPolicy* Simulation::createSelectionPolicy(const string& selectionPolicyType){//our method
    SelectionPolicy* policy = nullptr;
    if (selectionPolicyType == "eco") {
        policy = new EconomySelection();
    } else if (selectionPolicyType == "bal") {
        policy = new BalancedSelection(0, 0, 0);
    } else if (selectionPolicyType == "env") {
        policy = new SustainabilitySelection();
    } else if (selectionPolicyType == "nve") {
        policy = new NaiveSelection();
    } else {
        return nullptr;
    }
    return policy;
}

//--------------------------------------------------------------------------------------
// Destructor
Simulation:: ~Simulation(){
    for (BaseAction* act : actionsLog){
        delete act;
    }
    actionsLog.clear();
    for (Settlement *sett : settlements){
        delete sett;
    }
    settlements.clear();
}
