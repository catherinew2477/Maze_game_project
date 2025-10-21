//
//  Map.cpp
//  project3
//
//  Created by Catherine Wang on 5/13/24.
//


#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <utility>
#include <cstdlib>
using namespace std;

#include "utilities.h"
#include "Map.h"
#include "Actor.h"

Map::Map() {
    generateMap();
}

// functions below help add actors and objects

void Map::reprintMapObjects(Game* g, Map* m, Player* p) { // to prevent objects from disappearing after player goes over them lol
    // iterate through weapons/scrolls and print symbol for each item
    for (list<Weapon*>::iterator w = weaponsOnMap.begin(); w!=weaponsOnMap.end(); w++) {
        int r = (*w)->posRow();
        int c = (*w)->posCol();
        maze[r][c] = ')';
    }
    for (list<Scroll*>::iterator s = scrollsOnMap.begin(); s!=scrollsOnMap.end(); s++) {
        int r = (*s)->posRow();
        int c = (*s)->posCol();
        maze[r][c] = '?';
    }
    // reprint stairs only for levels 1-4
    if (p->getLevel() < 3) {
        int r = s->posRow();
        int c = s->posCol();
        maze[r][c] = '>';
    }
    if (p->getLevel() == 4) {
        int r = (g->getGoldenIdol())->posRow();
        int c = (g->getGoldenIdol())->posCol();
        maze[r][c] = '&';
    }
}

void Map::randomPos(int& posRow, int& posCol) {
    while (!checkSpace(posRow,posCol)) {
        posRow = randInt(1,16);
        posCol = randInt(1,68);
    }
    ;
}


void Map::placePlayer(int posRow, int posCol) {
    maze[posRow][posCol] = '@';
}

void Map::placeMonsters() {
    int numOfLoops = randInt(3,11);
    for (int i = 0; i < numOfLoops; i++) {
        int r(0), c(0);
        while (!checkSpace(r,c)) {
            r = randInt(1,16);
            c = randInt(1,68);
        }
        int whichMonster = randInt(1,4);
        switch (whichMonster) {
            case 1:
                maze[r][c] = 'B';
                actors.push_back(new Bogeyman(r, c));
                break;
            case 2:
                actors.push_back(new Snakewoman(r, c));
                maze[r][c] = 'S';
                break;
            case 3:
                actors.push_back(new Dragon(r, c));
                maze[r][c] = 'D';
                break;
            case 4:
                actors.push_back(new Goblin(r, c));
                maze[r][c] = 'G';
                break;
        }
    }
    ;
}

void Map::placeStairs() { // randomly place the stairs in a valid position
    int r = 0;
    int c = 0;
    while(!checkSpace(r,c)) {
        r = randInt(1,16);
        c = randInt(1,68);
    }
    maze[r][c] = '>';
    s = getStairs();
    s = new Stairs(r, c);
}

void Map::placeObjects() {
    int numOfLoops = randInt(2,3); // number of scrolls or weapons to show up on map
    for (int i = 0; i < numOfLoops; i++) {
        int r(0), c(0);
        while (!checkSpace(r,c)) {
            r = randInt(1,16);
            c = randInt(1,68);
        }
        int whichObject = randInt(1,10); // randomly pick what kind of scroll or weapon, each object is associated with a number
        switch (whichObject) {
            case 1:
                maze[r][c] = '?';
                scrollsOnMap.push_back(new Scroll(r, c, "scroll of teleportation"));
                break;
            case 2:
                maze[r][c] = '?';
                scrollsOnMap.push_back(new Scroll(r, c, "scroll of improve armor"));
                break;
            case 3:
                maze[r][c] = '?';
                scrollsOnMap.push_back(new Scroll(r, c, "scroll of strength"));
                break;
            case 4:
                maze[r][c] = '?';
                scrollsOnMap.push_back(new Scroll(r, c, "scroll of enhance health"));
                break;
            case 5:
                maze[r][c] = '?';
                scrollsOnMap.push_back(new Scroll(r, c, "scroll of enhance dexterity"));
                break;
            // weapons
            case 6:
                maze[r][c] = ')';
                weaponsOnMap.push_back(new ShortSword(r, c));
                break;
            case 7:
                maze[r][c] = ')';
                weaponsOnMap.push_back(new Mace(r, c));
                break;
            case 8:
                maze[r][c] = ')';
                weaponsOnMap.push_back(new LongSword(r, c));
                break;
            case 9:
                maze[r][c] = ')';
                weaponsOnMap.push_back(new MFSleep(r, c));
                break;
            case 10:
                maze[r][c] = ')';
                weaponsOnMap.push_back(new MagicAx(r, c));
                break;
        }
    }
    ;
}


// functions below make the actual map
bool Map::validRectangle(int& sr, int& sc, int& roomWidth, int& roomHeight) {
    for (int i = sr; i < MAX_HEIGHT-1 && i < sr + roomHeight; i++) {
        for (int j = sc; j < MAX_WIDTH-1 && j < sc + roomWidth; j++) {
            if (maze[i][j] == ' ') { // if a space is already there
                return false;
            }
        }
    }
    return true;
}

void Map::generateMap() {
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            maze[i][j] = '#';
        }
    }
    struct Room {
        Room(int r, int c, int roomWidth, int roomHeight) : m_r(r), m_c(c), m_roomWidth(roomWidth), m_roomHeight(roomHeight) {}
        int m_r, m_c, m_roomWidth, m_roomHeight; // m_r and m_c are coordinates for upper left corner
    };
    int numOfRooms = randInt(3,5); // randomly decide number of rooms to build
    vector<Room*> rooms;
    for (int i = 0; i < numOfRooms; i++) { // for each randomly generated room
        int roomWidth = randInt(5,20);
        int roomHeight = randInt(3,10);
        int c = randInt(10,50);
        int r = randInt(6,10);
        
        while (!validRectangle(r, c, roomWidth, roomHeight)) { // keep on randomly generated dimensions and starting coordinate until valid
            roomWidth = randInt(5,20);
            roomHeight = randInt(3,10);
            c = randInt(10,50);
            r = randInt(6,10);
        } // once we break out of loop, we made a valid rectangle yay
        
        // Fill the valid room with spaces
        for (int j = r; j < MAX_HEIGHT-1 && j < r + roomHeight; j++) {
            for (int k = c; k < MAX_WIDTH-1 && k < c + roomWidth; k++) {
                maze[j][k] = ' ';
            }
        }
        
        if (i != 0) { // if not the first room...
            // connect current room to the previous room with hallway by accessing our vector of rooms
            Room* prevRoom = rooms[i-1];
            makeCorridor(prevRoom->m_r + prevRoom->m_roomHeight/2, prevRoom->m_c + prevRoom->m_roomWidth/2, r + roomHeight/2, c + roomWidth/2);
        }
        rooms.push_back(new Room(r, c, roomWidth, roomHeight)); // add new room to vector
    }
    for (int i = 0; i < rooms.size(); i++) {
        delete rooms[i];
    }
}

void Map::makeCorridor(int cr_prev, int cc_prev, int cr_curr, int cc_curr) {
    if (cr_prev != cr_curr) {
        int x = min(cr_prev,cr_curr);
        while (x < max(cr_prev, cr_curr)) {
            maze[x][cc_curr] = ' ';
            x++;
        }
    }
    if (cc_prev != cc_curr) {
        int x = min(cc_prev,cc_curr);
        while (x < max(cc_prev, cc_curr)) {
            maze[cr_curr][x] = ' ';
            x++;
        }
    }
}


void Map::displayMap() {
    // use loop to cout everything we've made to set up the Map
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

// the functions below help with attack function

// three getter functions below to get the monster/object we want to perform actions with
Actor* Map::getMonster(list<Actor*>& actors, int r, int c) { // returns the monster that is being attacked
    for (list<Actor*>::iterator i = actors.begin(); i != actors.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return *i;
        }
    }
    return nullptr; // this path should never happen if I properly stored all the monsters on the map into the list
}

list<Actor*>::iterator Map::getMonsterToKill(list<Actor*>& actors, int r, int c) {
    for (list<Actor*>::iterator i = actors.begin(); i != actors.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return i;
        }
    }
    return actors.end(); // this path should never happen
}

Weapon* Map::getWeapon(list<Weapon*>& weapons, int r, int c) {
    for (list<Weapon*>::iterator i = weapons.begin(); i != weapons.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return *i;
        }
    }
    return nullptr; // if the object that the player wants to pick up is not a Weapon
}

list<Weapon*>::iterator Map::getWeaponToPickUp(list<Weapon*>& weapons, int r, int c) {
    for (list<Weapon*>::iterator i = weapons.begin(); i != weapons.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return i;
        }
    }
    return weapons.end(); // path will never be accessed
}

Scroll* Map::getScroll(list<Scroll*>& scrolls, int r, int c) {
    for (list<Scroll*>::iterator i = scrolls.begin(); i != scrolls.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return *i;
        }
    }
    return nullptr; // if the object that the player wants to pick up is not a Scroll
}

list<Scroll*>::iterator Map::getScrollToPickUp(list<Scroll*>& scrolls, int r, int c) {
    for (list<Scroll*>::iterator i = scrolls.begin(); i != scrolls.end(); i++) {
        if ((*i) -> posRow() == r && (*i) -> posCol() == c) {
            return i;
        }
    }
    return scrolls.end(); // path will never be accessed, just put it there so that all paths return something
}


