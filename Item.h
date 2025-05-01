#pragma once
#include <string>
using namespace std;

struct Item {
    string name;
    int type;
    bool hasElement;

    Item(string itemName, int itemType, bool elementStatus) {
        name = itemName;
        type = itemType;
        hasElement = elementStatus;
    }
};
