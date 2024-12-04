#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <stdexcept>
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &other);//Copy Constructor
        Plan(const Settlement &settlement, const Plan &other);//Copy Constructor for copy simulation
        Plan& operator=(const Plan &other);//Copy Assignment Operator      
        Plan(Plan &&other);// Move Constructor
        Plan& operator=(Plan &&other);// Move Assignment Operator
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        bool changePolicy(const string newPolicy);//our method
        string getSelectionPolicy() const;//our method
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        void scoreUpdate(Facility* facility);//our method
        const int getId();// our method
        Settlement getSettlement();//our method
        ~Plan();//Destructor

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};