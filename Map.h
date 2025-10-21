//
//  Maze.cpp
//  project3
//
//  Created by Catherine Wang on 5/12/24.
//

#ifndef maze_h
#define maze_h

#include <iostream>
#include <list>
#include <cstdlib>
#include "Actor.h"
#include "Game.h"

const int MAX_HEIGHT = 17;
const int MAX_WIDTH = 69; 

class Map
{
  public:
    Map(); // makes the map by initializing maze
    ~Map() { // Map destructor that deletes all pointers to monsters and points to objects that are still on the map
        for (list<Actor*>::iterator i = actors.begin(); i!=actors.end(); i++) {
            delete *i;
        }
        actors.clear();
        for (list<Weapon*>::iterator i = weaponsOnMap.begin(); i!=weaponsOnMap.end(); i++) {
            delete *i;
        }
        weaponsOnMap.clear();
        for (list<Scroll*>::iterator i = scrollsOnMap.begin(); i!=scrollsOnMap.end(); i++) {
            delete *i;
        }
        scrollsOnMap.clear();
    }
    void  displayMap(); // couts the map
    void generateMap(); // includes my algorithm for generating rooms
    void makeCorridor(int cr_prev, int cc_prev, int cr_curr, int cc_curr);
    bool validRectangle(int& sr, int& sc, int& roomWidth, int& roomHeight); // checks if the randomly generated room and its dimensions are valid
    
    void reprintMapObjects(Game* g, Map* m, Player* p); // to make sure objects don't disappear after player moves over them
    
    // functions for checking if a spot has some kind of characteristic
    bool hasStairs(int r, int c) {return maze[r][c] == '>';}
    bool reachIdol(Player* p, Object* i) {
        int p_r = p->posRow();
        int p_c = p->posCol();
        int i_r = i->posRow();
        int i_c = i->posCol();
        return (p_r == i_r && p_c == i_c);
    }
    void replaceWithSpace(int& r, int& c) { //  when player moves to new spot, monsters are killed, objects are picked up
        if (maze[r][c] == '>') {
            maze[r][c] = '>';
        }
        maze[r][c] = ' ';
    }
    bool checkSpace(int r, int c) { // makes sure that player is blocked by walls and monsters
        if (maze[r][c] == ')' || maze[r][c] == '?' || maze[r][c] == '>' || maze[r][c] == '&') {
            return true;
        }
        if (maze[r][c] == '@') {
            return false;
        }
        return maze[r][c] == ' '; }
    
    bool hasMonster(int r, int c) { // checks if there is a monster at the position the player wants to move to, triggers attack
        return (maze[r][c] == 'S' || maze[r][c] == 'D' || maze[r][c] == 'B' || maze[r][c] == 'G');
    }
    
    // functions in charge of placing actors and objects randomly
    void randomPos(int& posRow, int& posCol); // generate random and valid position when placing actors and objects onto map
    void placePlayer(int posRow, int posCol);
    void placeMonsters();
    void placeObjects();
    void placeStairs();
    void placeIdol(int r, int c) {maze[r][c] = '&';}
    void placeChar(char symbol, int r, int c) {
        maze[r][c] = symbol;
    }
    
    // getters
    Stairs* getStairs() {return s;}
    Object* makeIdol(int& r, int& c) {
        goldenIdol = new Object(r, c);
        return goldenIdol ; }
    list<Actor*>& getActors()  {return actors; }
    list<Weapon*>& getWeaponsOnMap() {return weaponsOnMap; }
    list<Scroll*>& getScrollsOnMap()  {return scrollsOnMap; }
    Actor* getMonster(list<Actor*>& actors, int r, int c); // takes in the location of the unidentified monster and uses loop to compare inputted position with each position of each actor in the list
    Weapon* getWeapon(list<Weapon*>& weapons, int r, int c); // same as above but for weapons and scrolls specifically
    Scroll* getScroll(list<Scroll*>& scrolls, int r, int c);
    
    list<Actor*>::iterator getMonsterToKill(list<Actor*>& actors, int r, int c); // return iterator of monster that needs to be removed
    list<Weapon*>::iterator getWeaponToPickUp(list<Weapon*>& weapons, int r, int c); // return iterator of weapon to be picked up, remove from list
    list<Scroll*>::iterator getScrollToPickUp(list<Scroll*>& scrolls, int r, int c); // return iterator of scroll to be picked up, remove from list
    
  private:
    char maze[MAX_HEIGHT][MAX_WIDTH]; // character array that sets up the maze
    list<Actor*> actors; // AKA monsters
    list<Weapon*> weaponsOnMap;
    list<Scroll*> scrollsOnMap;
    Stairs* s;
    Object* goldenIdol;
};


#endif
