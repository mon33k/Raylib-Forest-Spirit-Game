#pragma once
#include <string>
#include <vector>
#include "Item.h" // add Item struct
#include "raylib.h"

using namespace std;

struct Player {
    string name;
    int health = 100;
    Vector2 position = {100, 300};
    vector<Item> inventory;
    bool hasAllElements[5] = {false, false, false, false, false};

    void addItem(Item item) {
        inventory.push_back(item);
        hasAllElements[item.type] = true;
    }
};
