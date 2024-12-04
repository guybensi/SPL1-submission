#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation &other);// Copy Constructor
        Simulation& operator=(const Simulation &other);// Copy Assignment Operator
        Simulation(Simulation &&other) = default; // Move Constructor
        Simulation& operator=(Simulation &&other) = default; // Move Assignment Operator
        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isFacilityExists(const string &facilityName);//our method
        bool isSettlementExists(const string &settlementName);
        Settlement *getSettlement(const string &settlementName);
        vector<BaseAction*>& getlog();//our method
        Plan &getPlan(const int planID);
        int getplanCounter();
        void step();
        void close();
        void open();
        SelectionPolicy* createSelectionPolicy(const string& selectionPolicyType);
        ~Simulation();// Destructor
        

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};