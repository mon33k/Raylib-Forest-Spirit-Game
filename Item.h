#pragma once
#include <string>
using namespace std;


const string ITEM_TYPES[] = { "WATER", "AIR", "FIRE", "EARTH", "METAL" };
const string ITEM_NAMES[] = {"Water Orb", "Flute", "Torch", "Stone Axe", "Sword"};

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
