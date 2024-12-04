#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        Settlement(const Settlement& other) = default;//Copy Constructor
        const string &getName() const;
        SettlementType getType() const;
        const string settlementTypeToString() const;//our method
        const string toString() const;
        int getLimit() const;//our method

        private:
            const string name;
            SettlementType type;
            int limit;
};