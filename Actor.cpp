//
//  player.cpp
//  project3
//
//  Created by Catherine Wang on 5/12/24.
//

#include "Map.h"
#include "utilities.h"
#include <string>
#include <cmath>
using namespace std;

bool Goblin::reachesPlayer(int g_r, int g_c, int p_r, int p_c, vector<pair<int, int> >& visited, vector<pair<int, int> >& bestPath, int maxSteps, Map* m) {
    if (visited.size() >= maxSteps) { // if the current path being traversed (stored in visited vector) takes more than the maximum number of steps, give up and check another path
        return false;
    }
    
    if (abs(g_r - p_r) + abs(g_c - p_c) == 1) { // if the goblin is already next to player
        visited.push_back(make_pair(g_r, g_c)); // mark the goblin's current location as visited already
        if (bestPath.empty() || visited.size() < bestPath.size()) { // check that the best path is still being determined
            bestPath = visited;
        }
        return true;
    }

    
    for (vector<pair<int, int> >::iterator i = visited.begin(); i!=visited.end(); i++) { // loop through the visited vector to check if the goblin's current position has already been visited... if yes, give up on the path
        if (i->first == g_r && i->second == g_c) { // if place has already been visited
            return false;
        }
    }
    visited.push_back(make_pair(g_r, g_c)); // add coordinate to our vector that keeps track of the new coordinate that has been visited
    
    bool pathFound = false;
    // check right
    if (m->checkSpace(g_r, g_c+1) && p_c > g_c) { // if the player is on the right of the goblin and the goblin is able to move to the right one step
        if (reachesPlayer(g_r, g_c+1, p_r, p_c, visited, bestPath, maxSteps, m)) {
            pathFound = true;
            if (bestPath.empty() || visited.size() < bestPath.size()) {
                bestPath = visited; // if this newly traversed path is better than the best path so far (meaning that it takes less steps to get to the player), replace best path with the current path
                
            }
        }
    }
    // check up
    if (m->checkSpace(g_r-1, g_c) && p_r < g_r) { // if the player is above the goblin and the goblin is able to move to one step up
        if (reachesPlayer(g_r-1, g_c, p_r, p_c, visited, bestPath, maxSteps, m)) {
            pathFound = true;
            if (bestPath.empty() || visited.size() < bestPath.size()) {
                bestPath = visited;
            }
        }
    }
    // check left
    if (m->checkSpace(g_r, g_c-1) && p_c < g_c) { // if the player is on the left of the goblin and the goblin is able to move one step to the left
        if (reachesPlayer(g_r, g_c-1, p_r, p_c, visited, bestPath, maxSteps, m)) {
            pathFound = true;
            if (bestPath.empty() || visited.size() < bestPath.size()) {
                bestPath = visited;
            }
        }
    }
   // check down
    if (m->checkSpace(g_r+1, g_c) && p_r > g_r) { // if the player is below the goblin and the goblin is able to move one step below
        if (reachesPlayer(g_r+1, g_c, p_r, p_c, visited, bestPath, maxSteps, m)) {
            pathFound = true;
            if (bestPath.empty() || visited.size() < bestPath.size()) {
                bestPath = visited;
            }
        }
    }
    
    visited.pop_back(); // if the goblin is unable to move in any of the four directions, remove the current position of the goblin to go back to the last valid visited position (stored in visited vector)
    return pathFound;
}

void Goblin::move(int r, int c, Map* m, int maxSteps) { // r and c are player's position and maxSteps is the goblin's smell distance
    vector<pair<int, int> > visited;
    vector<pair<int, int> > bestPath;
    
    if (reachesPlayer(posRow(), posCol(), r, c, visited, bestPath, maxSteps, m)) { // if the goblin is able to reach the player from its current position
        if (bestPath.size() > 1) { // if the best path has already been created
            int r = bestPath[1].first; // get the next row that the goblin should move to according to the best path
            int c = bestPath[1].second; // get the next column that the goblin should move to according to the best path
            m->placeChar('G', r, c); // place the G character to make it appear that the goblin moved to the new position
            int current_r = posRow();
            int current_c = posCol();
            m->replaceWithSpace(current_r, current_c); // replace the G's original position with a space
            setposRow(r);
            setposCol(c);
        }
    }
    m->placeChar('G', posRow(), posCol()); // if the goblin can't smell or the reach the player from its current position, stay put
}


void Bogeyman::move(int r, int c, Map* m) { // pass in player's current positiom
    int b_r = posRow();
    int b_c = posCol();
    if (abs((b_c-c)) + abs((b_r-r)) <=5) {
        if (r > b_r) { // move down if player's current r is bigger
            if (m->checkSpace(b_r+1, b_c)) {
                m->replaceWithSpace(b_r, b_c); // replace previous position with a space
                setposRow(b_r+1);
                m->placeChar('B', posRow(), b_c);
                return;
            }
        }
        else if (c > b_c) { // move right if player's current r is bigger
            if (m->checkSpace(b_r, b_c+1)) {
                m->replaceWithSpace(b_r, b_c); // replace previous position with a space
                setposCol(b_c+1);
                m->placeChar('B', b_r, posCol());
                return;
            }
        }
        else if (b_r > r) { // move up if monster is on a row that is lower than player's
            if (m->checkSpace(b_r-1, b_c)) {
                m->replaceWithSpace(b_r, b_c); // replace previous position with a space
                setposRow(b_r-1);
                m->placeChar('B', posRow(), b_c);
                return;
            }
        }
        else if (b_c > c) { // move left if monster is at column to the right of the player
            if (m->checkSpace(b_r, b_c-1)) {
                m->replaceWithSpace(b_r, b_c); // replace previous position with a space
                setposCol(b_c-1);
                m->placeChar('B', b_r, posCol());
                return;
            }
        }
    }
}

void Snakewoman::move(int r, int c, Map* m) {
    int s_r = posRow();
    int s_c = posCol();
    if (abs((s_c-c)) + abs((s_r-r)) <=5) {
        if (r > s_r) { // move down if player's current r is bigger
            if (m->checkSpace(s_r+1, s_c)) {
                m->replaceWithSpace(s_r, s_c); // replace previous position with a space
                setposRow(s_r+1);
                m->placeChar('S', posRow(), s_c);
                return;
            }
        }
        else if (c > s_c) { // move right if player's current r is bigger
            if (m->checkSpace(s_r, s_c+1)) {
                m->replaceWithSpace(s_r, s_c); // replace previous position with a space
                setposCol(s_c+1);
                m->placeChar('S', s_r, posCol());
                return;
            }
        }
        else if (s_r > r) { // move up if monster is on a row that is lower than player's
            if (m->checkSpace(s_r-1, s_c)) {
                m->replaceWithSpace(s_r, s_c); // replace previous position with a space
                setposRow(s_r-1);
                m->placeChar('S', posRow(), s_c);
                return;
            }
        }
        else if (s_c > c) { // move left if monster is at column to the right of the player
            if (m->checkSpace(s_r, s_c-1)) {
                m->replaceWithSpace(s_r, s_c); // replace previous position with a space
                setposCol(s_c-1);
                m->placeChar('B', s_r, posCol());
                return;
            }
        }
    }
}

void Player::readScroll(Map* m, Object* s, string& sentence) {
    if (s == nullptr) {
        return;
    }
    Scroll* ptr = dynamic_cast<Scroll*>(s);
    if (ptr->getType() == "scroll of teleportation") {
        // The player is randomly moved to another place in the level that is not occupied by a wall or a monster.
        int old_r = posRow();
        int old_c = posCol();
        m->replaceWithSpace(old_r, old_c);
        int new_r = 0;
        int new_c = 0;
        m->randomPos(new_r, new_c);
        setposRow(new_r);
        setposCol(new_c);
        m->placePlayer(new_r, new_c);
        return;
    }
    if (ptr->getType() == "scroll of improve armor") {
        //The player's armor points are increased by a random integer from 1 to 3.
        setArmor();
        sentence = "You read the scroll called scroll of improve armor\nYour armor glows blue.";
        return;
    }
    if (ptr->getType() == "scroll of strength") {
        //The player's strength points are increased by a random integer from 1 to 3.
        setStrength();
        sentence = "You read the scroll called scroll of strength\nYour muscles bulge.";
        return;
    }
    if (ptr->getType() == "scroll of enhance health") {
        //The player's maximum hit point value is increased by a random integer from 3 to 8. This scroll does not affect the player's current number of hit points.
        setMaxHP();
        sentence = "You read the scroll called scroll of enhance health\nYou feel your heart beating stronger.";
        return;
    }
    else { // if the scroll of enhance dexterity is read...
        //The player's dexterity is increased by 1.
        setDex();
        sentence = "You read the scroll called scroll of enhance dexterity\nYou feel like less of a klutz.";
        return;
    }
}

void Player::displayInventory() {
    cout << "Inventory:" << endl;
    char letter = 97;
    for (vector<Object*>::iterator i = inventory.begin(); i != inventory.end(); i++, letter++) {
        if (identifyScrollNameFromObject(*i) == "") {
            cout << " " << letter << ". " << identifyWeaponNameFromObject(*i) << endl;
            getObjectsAtLetters().push_back(identifyWeaponNameFromObject(*i));
        }
        else {
            cout << " " << letter << ". " << identifyScrollNameFromObject(*i) << endl;
            getObjectsAtLetters().push_back(identifyWeaponNameFromObject(*i));
        }
        inventoryLetters.push_back(letter);
    }
}

void Player::movePlayer(Map* m, int r, int c, char dir, string& p_sentence, string& m_sentence) {
    r = posRow();
    c = posCol();
    switch (dir) {
        case ARROW_UP:
            if (m->checkSpace(r-1, c)) {
                m->replaceWithSpace(r, c); // replace previous position with a space
                r--;
                setposRow(r);
                break;
            }
            if (m->hasMonster(r-1, c)) { // if the player moves toward a monster, trigger attack
                Weapon* w = getCurrWeapon();
                list<Actor*> actors = m -> getActors();
                Actor* monster = m -> getMonster(actors, r-1, c);
                monster -> attackPlayer(this, identifyAndUseMonster(monster), m_sentence, p_sentence);
                if (getSleepTime() > 0) {
                    break;
                }
                attackMonster(m, w, monster, p_sentence, m_sentence);
            }
            break;
        case ARROW_DOWN:
            if (m->checkSpace(r+1, c)) {
                m->replaceWithSpace(r, c);
                r++;
                setposRow(r);
                break;
            }
            if (m->hasMonster(r+1, c)) {
                Weapon* w = getCurrWeapon();
                list<Actor*> actors = m -> getActors();
                Actor* monster = m -> getMonster(actors, r+1, c);
                monster -> attackPlayer(this, identifyAndUseMonster(monster), m_sentence, p_sentence);
                if (getSleepTime() > 0) {
                    break;
                }
                attackMonster(m, w, monster, p_sentence, m_sentence);
            }
            break;
        case ARROW_LEFT:
            if (m->checkSpace(r, c-1)) {
                m->replaceWithSpace(r, c);
                c--;
                setposCol(c);
                break;
            }
            if (m->hasMonster(r, c-1)) {
                Weapon* w = getCurrWeapon();
                list<Actor*> actors = m -> getActors();
                Actor* monster = m -> getMonster(actors, r, c-1);
                string sentence = "";
                monster -> attackPlayer(this, identifyAndUseMonster(monster), m_sentence, p_sentence);
                if (getSleepTime() > 0) {
                    break;
                }
                attackMonster(m, w, monster, p_sentence, m_sentence);
            }
            break;
        case ARROW_RIGHT:
            if (m->checkSpace(r, c+1)) {
                m->replaceWithSpace(r, c);
                c++;
                setposCol(c);
                break;
            }
            if (m->hasMonster(r, c+1)) {
                Weapon* w = getCurrWeapon();
                list<Actor*> actors = m -> getActors();
                Actor* monster = m -> getMonster(actors, r, c+1);
                string sentence = "";
                monster -> attackPlayer(this, identifyAndUseMonster(monster), m_sentence, p_sentence);
                if (getSleepTime() > 0) {
                    break;
                }
                attackMonster(m, w, monster, p_sentence, m_sentence);
            }
            break;
    }
    
    return;
}

string Actor::identifyMonsterName(Actor* monster) { // this function is used to determine which monster in the list of actors (that are currently on the map) the player is attacking so when I cout the attack sentence, it gets the right monster's name
    if (Bogeyman* bogeymanPtr = dynamic_cast<Bogeyman*>(monster)) {
        return bogeymanPtr -> getName();
    }
    if (Dragon* dragonPtr = dynamic_cast<Dragon*>(monster)) {
        return dragonPtr -> getName();
    }
    if (Goblin* goblinPtr = dynamic_cast<Goblin*>(monster)) {
        return goblinPtr -> getName();
    }
    else {
        Snakewoman* snakewomanPtr = dynamic_cast<Snakewoman*>(monster);
        return snakewomanPtr -> getName();
    }
}


Weapon* Player::identifyAndUseMonster(Actor* monster) { // get the monster's weapon to help monsters attack player with their associated weapon
    if (Bogeyman* bogeymanPtr = dynamic_cast<Bogeyman*>(monster)) {
        return bogeymanPtr->getWeapon();
    }
    if (Dragon* dragonPtr = dynamic_cast<Dragon*>(monster)) {
        return dragonPtr->getWeapon();
    }
    if (Goblin* goblinPtr = dynamic_cast<Goblin*>(monster)) {
        return goblinPtr->getWeapon();
    }
    else {
        Snakewoman* snakewomanPtr = dynamic_cast<Snakewoman*>(monster);
        return snakewomanPtr->getWeapon();
    }
}

string Player::identifyWeaponNameFromObject(Object* objectPtr) { // use dynamic casting to determine what kind of weapon the player picked up
    if (ShortSword* ptr = dynamic_cast<ShortSword*>(objectPtr)) {
        return "short sword";
    }
    if (LongSword* ptr = dynamic_cast<LongSword*>(objectPtr)) {
        return "long sword";
    }
    if (MagicAx* ptr = dynamic_cast<MagicAx*>(objectPtr)) {
        return "magic ax";
    }
    if (MFSleep* ptr = dynamic_cast<MFSleep*>(objectPtr)) {
        return "magic fangs of sleep";
    }
    if (Mace* ptr = dynamic_cast<Mace*>(objectPtr)) {
        return "mace";
    }
    return "";
}

string Player::identifyScrollNameFromObject(Object* objectPtr) { // use dynamic casting to determine what kind of scroll the player picked up
    if (Scroll* ptr = dynamic_cast<Scroll*>(objectPtr)) {
        return ptr->getType();
    }
    return "";
}

void Actor::attackPlayer(Actor* player, Weapon* w, string& monSentence, string& p_sentence) { // monster attacks player first if player is next to it
    
    int weaponDamageAmount = w -> getDamage();
    int mon_strength = getStrength();
    int damagePoints = randInt(0, mon_strength + weaponDamageAmount - 1);
    
    int mon_dex = getDex(); // bad access issue
    int weaponDexBonus = w -> getDex();
    int mon_points = mon_dex + weaponDexBonus;
    
    int player_points = player->getDex() + player->getArmor();
    
    if (randInt(1, mon_points) >= randInt(1, player_points)) { // if dexterity bonus allows monster to hit player, deduct player HP
        if (this->getName() == "Snakewoman") {
            displayMonAction(w, "and hits, putting the Player to sleep", monSentence);
            player->addSleepTime();
            int playerHP = player->getHit() - damagePoints;
            if (playerHP < 0) {
                player -> setHP(0);
            }
            if (player -> getHit() <= 0) { // if HP runs out, kill player ;-;
                displayMonAction(w, "dealing a final blow.", monSentence);
                p_sentence = "";
                return;
            }
            else {
                player -> setHP(playerHP);
                return;
            }
            return;
        }
        displayMonAction(w, "and hits.", monSentence);
        int playerHP = player->getHit() - damagePoints;
        if (playerHP < 0) {
            player -> setHP(0);
        }
        if (player -> getHit() <= 0) { // if HP runs out, kill player ;-;
            displayMonAction(w, "dealing a final blow.", monSentence);
            p_sentence = "";
            return;
        }
        else {
            player -> setHP(playerHP);
            return;
        }
    }
 displayMonAction(w, "and misses.", monSentence);
}

void Player::attackMonster(Map* m, Weapon* w, Actor* monster, string& sentence, string& mon_sentence) {
    int weaponDamageAmount = w -> getDamage();
    int attackerStrength = getStrength();
    int damagePoints = randInt(0, attackerStrength + weaponDamageAmount - 1);
    
    int attackerDex = getDex(); // bad access issue
    int weaponDexBonus = w -> getDex();
    int attackerPoints = attackerDex + weaponDexBonus;
    
    int monsterPoints = monster -> getDex() + monster -> getArmor();
    
    if (randInt(1, attackerPoints) >= randInt(1, monsterPoints)) { // if dexterity bonus allows attacker to hit defender, deduct defender HP
        int monsterHP = monster -> getHit() - damagePoints;
        monster -> setHP(monsterHP);
        // if player hits the monster with magic fangs of sleep, add monster sleep time
        if (getCurrWeapon()->getWeaponName(getCurrWeapon()) == "magic fangs of sleep" && monsterHP>=0) {
            displayAction(getCurrWeapon(), monster, "and hits, putting the" + monster->getName(), sentence);
            monster -> addSleepTime();
            return;
        }
        if (monster -> getHit() <= 0) { // if HP runs out, KILL MONSTER MWAHAHAHA
            displayAction(w, monster, " dealing a final blow.", sentence);
            int r = monster -> posRow();
            int c = monster -> posCol();
            m -> replaceWithSpace(r, c);
            list<Actor*>::iterator i = m -> getMonsterToKill(m -> getActors(), r, c);
            (m -> getActors()).erase(i);
            delete monster;
            mon_sentence = "";
            return;
        }
        displayAction(w, monster, " and hits.", sentence);
        return;
    }
    displayAction(w, monster, " and misses.", sentence);
}

bool Player::pickUpObject(Map* m, string& sentence) {
    // if player's current position matches with an object's position (iterate through weapon and object lists that both keep track of objects on map), add to weapon/scroll inventory and return true, else do nothing and return false
    int r = posRow();
    int c = posCol();
  
    // add case that player picks up idol
    // add case that player picks up... empty space
    if (m -> getWeapon(m -> getWeaponsOnMap(), r, c) == nullptr && m-> getScroll(m -> getScrollsOnMap(), r, c) == nullptr) { // if player picks up nothing
        return false;
    }
    if (m -> getWeapon(m -> getWeaponsOnMap(), r, c) == nullptr){ // if the object that the player picked up isn't a weapon
        Scroll* addScroll = m -> getScroll(m -> getScrollsOnMap(), r, c); // find the scroll that matches the player's current position (because player is standing on it lol)
        for (int i = 0; i<inventory.size(); i++) {
            if (inventory[i] == nullptr) {
                inventory[i] = addScroll;
                list<Scroll*>::iterator s = m -> getScrollToPickUp(m -> getScrollsOnMap(), r, c);
                (m->getScrollsOnMap()).erase(s); // delete scroll from scrolls list because it's gone from map
                delete *s;
                sentence = "You pick up a scroll called " + addScroll -> getType();
                return true;
            }
        }
    }
    else { // if the object that the player picked up is a weapon
        Weapon* addWeapon = m -> getWeapon(m -> getWeaponsOnMap(), r, c); // find the weapon that matches the player's current position
        for (int i = 0; i<inventory.size(); i++) {
            if (inventory[i] == nullptr) {
                inventory[i] = addWeapon;
                list<Weapon*>::iterator w = m->getWeaponToPickUp(m -> getWeaponsOnMap(), r, c);
                (m->getWeaponsOnMap()).erase(w); // delete weapon from weapons list because it's gone from map
                delete *w;
                sentence = "You pick up " + addWeapon -> getWeaponName(addWeapon);
                return true;
            }
        }
    }
    return false;
}

void Actor::displayMonAction(Weapon* w, string missOrHit, string& sentence) {
    if (identifyMonsterName(this) == "Snakewoman") {
        sentence = "the Snakewoman strikes magic fangs at the Player " + missOrHit;
        return;
    }
    sentence = identifyMonsterName(this) + " " + w->getActionString() + " at the Player " + missOrHit;
    return;
}

void Player::displayAction(Weapon* w, Actor* monster, string missOrHit, string& sentence) {
    sentence = "Player " + getCurrWeapon() -> getActionString() + " at the " + monster->getName() + missOrHit;
}

