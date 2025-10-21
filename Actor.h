//
//  player.hpp
//  project3
//
//  Created by Catherine Wang on 5/12/24.
//

#ifndef actor_hpp
#define actor_hpp

#include <string>
#include <cstdlib>
#include <vector>
#include "GameObject.h"
class Map;
#include "utilities.h"


class Actor {
public:
    Actor(int posRow, int posCol, int hit, int armor, int strength, int dex, int sleepTime, string name) : m_posRow(posRow), m_posCol(posCol), m_hit(hit), m_armor(armor), m_strength(strength), m_dex(dex), m_sleepTime(sleepTime), m_name(name) {}
    
    virtual ~Actor() {}
    // setters
    void setposRow(int r) {m_posRow = r;}
    void setposCol(int c) {m_posCol = c;}
    void setHP(int HP) {m_hit = HP; }
    void addSleepTime() {m_sleepTime = randInt(0,9);}
    int getSleepTime() {return m_sleepTime; }
    void setArmor() {m_armor += randInt(1,3); }
    void setStrength() {m_strength += randInt(1,3); }
    void setDex() {m_dex+=1; }
    
    
    // getters
    int posRow() {return m_posRow; }
    int posCol() {return m_posCol; }
    int getHit() const {return m_hit; }
    int getArmor() const {return m_armor; }
    int getStrength() const {return m_strength; }
    int getDex() const {return m_dex; }
    int getSleepTime() const {return m_sleepTime; }
    virtual string getName() const {return "";}
    
    string identifyMonsterName(Actor* monster);
    
    void displayMonAction(Weapon* w, string missOrHit, string& sentence);

    void attackPlayer(Actor* player, Weapon* w, string& sentence, string& p_sentence);
    
private:
    int m_posRow, m_posCol, m_hit, m_armor, m_dex, m_strength, m_sleepTime;
    string m_name;
};

class Player : public Actor {
public:
    Player(int posRow, int posCol) : Actor(posRow, posCol, 20, 2, 2, 2, 0, "Player"), inventory(25,nullptr) {
        m_level = 0;
        ShortSword* s = new ShortSword(posRow, posCol);
        inventory[0] = s;
        currWeapon = s;
        m_maxHP = 20;
    }
    virtual ~Player() {
        for (vector<Object*>::iterator i = inventory.begin(); i!=inventory.end(); i++) {
            delete *i;
        }
        inventory.clear();
    }
    
    void movePlayer(Map* m, int r, int c, char dir, string& p_sentence, string& m_sentence);
    void readScroll(Map* m, Object* s, string& sentence);
    
    void addLevel() {m_level++; }
    int getLevel() {return m_level;}
    
    
    void attackMonster(Map* m, Weapon* w, Actor* monster, string& sentence, string& mon_sentence);
    void displayAction(Weapon* w, Actor* monster, string missOrHit, string& sentence);
   
    
    bool pickUpObject(Map* m, string& sentence);
    void displayPickUp(Object* o, string& sentence);
    
   
    
    void displayInventory(); // iterates over player's inventory and prints out everything
    void wieldWeapon(Weapon* w) {currWeapon = w; } 
    void readScroll(Scroll* s); // when press r, shows inventory to select scroll to read
    Weapon* getCurrWeapon() {return currWeapon; }
   
    // setters
    void setWeapon(Object* w) {
        currWeapon = dynamic_cast<Weapon*>(w);
    }
    int getMaxHP() {return m_maxHP; }
    void setMaxHP() {m_maxHP += randInt(3,8); }
    
    
    // dynamic casting headache
    Weapon* identifyAndUseMonster(Actor* actorPtr);
    string identifyWeaponNameFromObject(Object* objectPtr);
    string identifyScrollNameFromObject(Object* objectPtr);
    
    // getters for vectors that store player's inventory objects
    vector<char>& getInventoryLetters() {return inventoryLetters; }
    vector<Object*>& getInventory() {return inventory; }
    vector<string>& getObjectsAtLetters() {return objectsAtLetters; }
    
private:
    vector<Object*> inventory;
    Weapon* currWeapon;
    int m_level;
    vector<char> inventoryLetters;
    vector<string> objectsAtLetters;
    int m_maxHP;
};

class Bogeyman : public Actor {
public:
    Bogeyman(int posRow, int posCol) : Actor(posRow, posCol, randInt(5,10), 2, randInt(2,3), randInt(2,3), 0, "Bogeyman") {
        b_weapon = new ShortSword(posRow, posCol);
    }
    virtual ~Bogeyman() {delete b_weapon; }
    void move(int r, int c, Map* m); // towards player if it smells the player

    string getName() const {return "Bogeyman"; }
    Weapon* getWeapon() {return b_weapon; }
private:
    ShortSword* b_weapon;
};

class Snakewoman : public Actor {
public:
    Snakewoman(int posRow, int posCol) : Actor(posRow, posCol, randInt(3,6), 3, 2, 3, 0, "Snakewoman") {
        s_weapon = new MFSleep(posRow, posCol);
    }
    virtual ~Snakewoman() {delete s_weapon; }
    void move(int r, int c, Map* m);

    Weapon* getWeapon() {return s_weapon; }
    string getName() const {return "Snakewoman"; }
private:
    MFSleep* s_weapon;
};

class Dragon : public Actor {
public:
    Dragon(int posRow, int posCol) : Actor(posRow, posCol, randInt(20,25), 4, 4, 4, 0, "Dragon") {
        d_weapon = new LongSword(posRow, posCol);
    }
    virtual ~Dragon() {delete d_weapon; }

    string getName() const {return "Dragon"; }
    Weapon* getWeapon() {return d_weapon; }
private:
    LongSword* d_weapon;
};

class Goblin : public Actor {
public:
    Goblin(int posRow, int posCol) : Actor(posRow, posCol, randInt(15,20), 1, 3, 1, 0, "Goblin") {
        g_weapon = new ShortSword(posRow, posCol);
    }
    virtual ~Goblin() {delete g_weapon; }
    
     //recursive goblin movement
    void move(int r, int c, Map* m, int maxSteps); // this function moves the goblin based on the shortest path to the player that is determined by the reachesPlayer function
        bool reachesPlayer(int g_r, int g_c, int p_r, int p_c, vector<pair<int, int> >& visited, vector<pair<int, int> >& bestPath, int maxSteps, Map* m);

    void attack();
    Weapon* getWeapon() {return g_weapon; }
    string getName() const {return "Goblin"; }
    


private:
    ShortSword* g_weapon;
};


#endif /* player_hpp */
