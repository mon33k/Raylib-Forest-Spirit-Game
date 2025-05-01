#include "raylib.h"
#include "raymath.h"
#include "Player.h"
#include "Item.h"
#include "World.h"
#include "GameState.h"

#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

// compile in windows cmd 
//g++ main.cpp -o main.exe -IC:/raylib/w64devkit/include -LC:/raylib/w64devkit/lib -lraylib -lopengl32 -lgdi32 -lwinmm

// Global variables
GameState gameState = TITLE;
const int screenWidth = 1600;
const int screenHeight = 600;
// Background layers
Texture2D background;
Texture2D midground2;
Texture2D midground1;
Texture2D foreground;
// Bg Scroll Positions
float scrollingBack = 0.0f;
float scrollingMid = 0.0f;
float scrollingFore = 0.0f;
// Setting up scene, ground level, encountering spirits after walking a dist
const float groundLevel = 400.0f; // ground for player and sprites not to float into the sky
float distanceWalked = 0.0f;
bool spiritReady = false;

// Player player;
Player player = {"Traveler", 100, {100, 300}};
string playerName;
bool playerLoaded = true;
float playerSpeed = 3.0f;
float blinkTimer = 0.0f;
bool showUnderscore = true;
bool nameEntered = false;
vector<Spirit> spirits;
Spirit* activeSpirit = nullptr; // Pointer to the current active spirit
bool isTalking = false;
string currentSpiritMessage = "";


Player CreateNewPlayer(string playerName) {
    Player p;
    p.name = playerName;
    p.health = 100;
    p.position = {100, 300}; // starting position
    // You could also clear inventory if needed
    return p;
}

Spirit CreateSpirit(string name, Vector2 position, Elements element) {
    Spirit s;
    s.name = name;
    s.position = position;
    s.element = element;
    s.talkedTo = false;
    return s;
}

void InitGame() {
    InitWindow(screenWidth, screenHeight, "Spirit Forest - Beginner Game");
    SetTargetFPS(60);
    // main background color #1c466d
    // 28 70 109
    background = LoadTexture("assets/background.png"); // Or wherever your background.png is
    midground2 = LoadTexture("assets/midground2.png");
    midground1 = LoadTexture("assets/midground1.png");
    foreground = LoadTexture("assets/foreground.png");

    spirits.push_back({"Water Spirit", {600, groundLevel}, WATER});
    spirits.push_back({"Air Spirit", {400, groundLevel}, AIR});
    spirits.push_back({"Fire Spirit", {200, groundLevel}, FIRE});
    spirits.push_back({"Earth Spirit", {700, groundLevel}, EARTH});
    spirits.push_back({"Metal Spirit", {500, groundLevel}, METAL});    

}

void UpdateTitleScreen() {
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && playerName.length() < 12) {
            playerName += (char)key;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) {
        playerName.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER) && !playerName.empty()) {
        player = CreateNewPlayer(playerName);
        gameState = PLAYING;
    }
    // Blinking underscore timer
    blinkTimer += GetFrameTime();
    if (blinkTimer >= 0.5f) {
        showUnderscore = !showUnderscore;
        blinkTimer = 0.0f;
    }
}

// Add file to save game data for player names and player progress
// void LoadGame(){
    // check playerLoaded bool 
// }

void DrawTitleScreen() {
    DrawText("Enter your name:", 100, 100, 30, WHITE);

    string nameToDisplay = playerName;
    if (showUnderscore) {
        nameToDisplay += "_";
    }

    DrawText(nameToDisplay.c_str(), 100, 150, 30, YELLOW);
}

void UpdateGameplay() {
    // Move player
    if (IsKeyDown(KEY_RIGHT)) {
        player.position.x += playerSpeed;
        scrollingBack -= 0.1f;
        scrollingMid  -= 0.5f;
        scrollingFore -= 1.0f;
    
        distanceWalked += playerSpeed; // add distance moved
    }
    
    if (IsKeyDown(KEY_LEFT)) {
        player.position.x -= playerSpeed;
        scrollingBack += 0.1f;
        scrollingMid  += 0.5f;
        scrollingFore += 1.0f;
    
        distanceWalked += playerSpeed; // still add distance moved
    }
    
    
    if (IsKeyDown(KEY_UP))    player.position.y -= playerSpeed;
    if (IsKeyDown(KEY_DOWN))  player.position.y += playerSpeed;

    // Keep player within the bounds of the screen window
    // Clamp player position to screen/world limits
    // Wrap horizontally
    if (player.position.x > background.width) player.position.x = 0;
    if (player.position.x < 0) player.position.x = background.width;

    // Keep vertical position fixed at groundLevel
    player.position.y = groundLevel;


    // Wrap the scrolling for background
    if (scrollingBack <= -background.width*2) scrollingBack = 0;
    if (scrollingMid <= -midground2.width*2) scrollingMid = 0;
    if (scrollingMid <= -midground1.width*2) scrollingMid = 0;
    if (scrollingFore <= -foreground.width*2) scrollingFore = 0;

    // Check spirit distances
    if (!spiritReady && distanceWalked > 2000.0f) {
        spiritReady = true;
        int randomIndex = GetRandomValue(0, spirits.size() - 1);
        activeSpirit = &spirits[randomIndex];
    
        // Randomize spirit X position to right of screen
        activeSpirit->position.x = GetRandomValue(800, background.width - 100);
        activeSpirit->position.y = groundLevel; // always keep on ground
    }
    

    
    
    // Handle talking and picking up items
}


void DrawGameplay() {

    // BeginDrawing();
    Color forestNight = {28, 70, 109, 255}; // your nice #1c466d dark blue
    ClearBackground(forestNight); 
    // Draw background
    DrawTexture(background, (int)scrollingBack, 0, WHITE);
    DrawTexture(background, (int)scrollingBack + background.width, 0, WHITE);

    DrawTexture(midground2, (int)scrollingMid, 0, WHITE);
    DrawTexture(midground2, (int)scrollingMid + midground2.width, 0, WHITE);
    
    DrawTexture(midground1, (int)scrollingMid, 0, WHITE);
    DrawTexture(midground1, (int)scrollingMid + midground1.width, 0, WHITE);

    // Draw player
    DrawCircleV(player.position, 20, BLUE);

      // After drawing backgrounds
    if (spiritReady && activeSpirit != nullptr && !isTalking) {
        DrawCircleV(activeSpirit->position, 20, YELLOW);
        string spiritName = (*activeSpirit).name;
        DrawText(spiritName.c_str(), activeSpirit->position.x - 10, activeSpirit->position.y - 40, 20, WHITE);
    }
    DrawTexture(foreground, (int)scrollingFore, 0, WHITE);
    DrawTexture(foreground, (int)scrollingFore + foreground.width, 0, WHITE);


    // Draw dialog, etc.
    // EndDrawing();
}



int main() {
    InitGame();

    // Setup player, spirits, etc.    
    while (!WindowShouldClose()) {
        switch (gameState) {
            case TITLE:
                if (!playerLoaded)
                    UpdateTitleScreen();
                else
                    gameState = PLAYING; // Skip title screen
                break;
            case PLAYING:
                UpdateGameplay();
                break;
            case VICTORY:
                // Optional: update victory input (press R to restart?)
                break;
            case GAMEOVER:
                // Optional: update gameover input
                break;
        }
    
        BeginDrawing();
        ClearBackground(GREEN);
    
        switch (gameState) {
            case TITLE:
                DrawTitleScreen();
                break;
            case PLAYING:
                DrawGameplay();
                break;
            case VICTORY:
                //DrawVictoryScreen();  // you can make this later
                break;
            case GAMEOVER:
                //DrawGameOverScreen(); // you can make this later
                break;
        }
    
        EndDrawing();
    }
    

    CloseWindow();
    return 0;
}


// int main() {
//     InitGame();

//     // 🌱 Player setup (use your Player struct now!)
//     Player player;
//     player.name = "Traveler";  // Default name for now
//     player.health = 100;
//     player.position = { 100, 300 };

//     float playerSpeed = 3.0f;

//     // 🌱 Spirit setup
//     Vector2 spiritPos = { 600, 300 };
//     float interactDistance = 40.0f;
//     bool isNearSpirit = false;
//     bool talkedToSpirit = false;

//     while (!WindowShouldClose()) {
//         // 🌱 Movement using player struct
//         if (IsKeyDown(KEY_RIGHT)) player.position.x += playerSpeed;
//         if (IsKeyDown(KEY_LEFT))  player.position.x -= playerSpeed;
//         if (IsKeyDown(KEY_UP))    player.position.y -= playerSpeed;
//         if (IsKeyDown(KEY_DOWN))  player.position.y += playerSpeed;

//         // Check distance to spirit
//         float dist = sqrt(pow(player.position.x - spiritPos.x, 2) + pow(player.position.y - spiritPos.y, 2));
//         isNearSpirit = (dist <= interactDistance);

//         // Interaction
//         if (isNearSpirit && IsKeyPressed(KEY_E)) {
//             talkedToSpirit = true;

//             // 🌱 Spirit gives a WATER Orb when talked to
//             Item waterOrb("Water Orb", WATER, true);
//             player.addItem(waterOrb);
//         }

//         // Draw everything
//         BeginDrawing();
//         ClearBackground(GREEN); // Forest background

//         // Draw player
//         DrawCircleV(player.position, 20, BLUE);

//         // Draw spirit
//         DrawCircleV(spiritPos, 20, YELLOW);

//         // Show prompt if near
//         if (isNearSpirit && !talkedToSpirit) {
//             DrawText("Press E to talk", screenWidth/2 - 100, 50, 20, WHITE);
//         }

//         // After talking
//         if (talkedToSpirit) {
//             DrawText("Spirit: Welcome, traveler...", screenWidth/2 - 150, 100, 20, WHITE);

//             // Show inventory unlocked
//             for (size_t i = 0; i < player.inventory.size(); i++) {
//                 string itemText = "Acquired: " + player.inventory[i].name;
//                 DrawText(itemText.c_str(), 10, 400 + i * 20, 20, SKYBLUE);
//             }
//         }

//         EndDrawing();
//     }

//     CloseWindow();
//     return 0;
// }
