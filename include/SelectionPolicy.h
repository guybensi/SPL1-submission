#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual string getName() const = 0;
        virtual ~SelectionPolicy()= default; //Destructor
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        NaiveSelection(const NaiveSelection &other) = default;// Copy-Constrector
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
        string getName() const override;//our method
        ~NaiveSelection() override = default;//Destructor
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection(const BalancedSelection &other) = default;// Copy-Constrector:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        int distance(const FacilityType &CurrFacility);
        const string toString() const override;
        BalancedSelection *clone() const override;
        string getName() const override;//our method
        ~BalancedSelection() override = default;//Destructor
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        EconomySelection(const EconomySelection &other) = default; // Copy-Constrector
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;
        string getName() const override;//our method
        ~EconomySelection() override = default;//Destructor
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        SustainabilitySelection(const SustainabilitySelection &other) = default;// Copy-Constrector:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        string getName() const override;//our method
        ~SustainabilitySelection() override = default;//Destructor
    private:
        int lastSelectedIndex;
};