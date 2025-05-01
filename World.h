#pragma once
#include <string>
#include "raylib.h"
using namespace std;

enum Elements {
    WATER,
    AIR,
    FIRE,
    EARTH,
    METAL
};

enum Zones { FOREST, MOUNTAIN, RIVER };


const string ELEMENT_NAMES[] = { "WATER", "AIR", "FIRE", "EARTH", "METAL" };

struct Spirit {
    string name;
    Vector2 position;
    Elements element;
    bool talkedTo = false;
};