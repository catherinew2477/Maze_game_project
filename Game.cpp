//
//  Game.cpp
//  project3
//
//  Created by Catherine Wang on 5/10/24.
//


#include "utilities.h"
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Map.h"
using namespace std;


Game::Game(int goblinSmellDistance) : m_gsd(goblinSmellDistance) {
    m = new Map();
    int player_posRow = 0;
    int player_posCol = 0;
    m->randomPos(player_posRow, player_posCol); // generate random but valid position for Player
    m->placePlayer(player_posRow, player_posCol);
    p = new Player(player_posRow, player_posCol);
    m->placeMonsters(); // randomly places monsters onto map
    m->placeObjects(); // randomly places scrolls onto map
    m->placeStairs();
}

void Game::monstersMayMove(Player* p, Map* m) { // calls each monster's version of move for every monster currently on the map
    list<Actor*> actors = m->getActors();
    for (list<Actor*>::iterator i = actors.begin(); i!=actors.end(); i++) {
        if ((*i)->identifyMonsterName(*i) == "Bogeyman") {
            Bogeyman* ptr = dynamic_cast<Bogeyman*>(*i);
            int r = p->posRow();
            int c = p->posCol();
            ptr->move(r, c, m);
        }
        if ((*i) -> identifyMonsterName(*i) == "Snakewoman") {
            Snakewoman* ptr = dynamic_cast<Snakewoman*>(*i);
            int r = p->posRow();
            int c = p->posCol();
            ptr->move(r, c, m);
        }
        if ((*i) -> identifyMonsterName(*i) == "Goblin") {
            Goblin* ptr =  dynamic_cast<Goblin*>(*i);
            int r = p->posRow();
            int c = p->posCol();
            ptr->move(r, c, m, m_gsd);
        }
    }
    ;
}

void Game::displayStats() {
    cout << "Level: " << p->getLevel() << ", " << "Hit points: " << p->getHit() << ", " << "Armor: " << p->getArmor() << ", " << "Strength: " << p->getStrength() << ", " << "Dexterity: " << p->getDex() << endl; 
}

void Game::play()
{
    m->displayMap();
    displayStats();
    char command = getCharacter();
    while (command != 'q') {
        if (p->getHit() <= 0) {
            break;
        }
        if (trueWithProbability(0.1) && p->getHit()<p->getMaxHP()) { // the player may occasionally regain a hit point
            p->setHP(p->getHit() + 1);
        }
        
        switch(command) {
            case 'w': { // wield a weapon
                clearScreen();
                p->displayInventory();
                char chooseWeapon = getCharacter();
                int loopNum = 0;
                string wieldSentence = "";
                for (vector<char>::iterator i = (p->getInventoryLetters()).begin(); i!=(p->getInventoryLetters()).end() && wieldSentence==""; i++, loopNum++) { // iterate through the letters in the inventory that label each weapon in the inventory to find which letter matches the letter inputted by the user
                    cout << p->getInventory()[loopNum] << endl;
                    if (chooseWeapon == *i) { // if the matching weapon is found
                        p->setWeapon(p->getInventory()[loopNum]); // set the player's current weapon to the new weapon by converting Object* to Weapon*
                        p->wieldWeapon(p->getCurrWeapon());
                        m->displayMap();
                        displayStats();
                        cout << endl;
                        wieldSentence = "You are wielding " + (p->getCurrWeapon())->getWeaponName(p->getCurrWeapon());
                    }
                }
                if (wieldSentence == "") {break; }
                m->displayMap();
                displayStats();
                cout << endl;
                cout << wieldSentence << endl;
                break;
            }
            case 'r': { // read a scroll
                clearScreen();
                p->displayInventory();
                char chooseScroll = getCharacter();
                int loopNum = 0;
                string readScrollSentence = "";
                for (vector<char>::iterator i = (p->getInventoryLetters()).begin(); i!=(p->getInventoryLetters()).end() && readScrollSentence==""; i++, loopNum++) {
                    if (chooseScroll == *i) { // when the matching scroll is found, let the player read it
                        p->readScroll(m, (p->getInventory())[loopNum], readScrollSentence);
                        m->displayMap();
                        displayStats();
                        cout << endl;
                        cout << readScrollSentence << endl;
                    }
                }
                if (readScrollSentence == "") {break; }
                m->displayMap();
                displayStats();
                cout << endl;
                cout << readScrollSentence << endl;
                break;
            }
            case '>': // implement levels!!!
                if ((m->getStairs())->posRow() == p -> posRow() && (m->getStairs()) -> posCol() == p -> posCol()) {
                    Map* temp = m;
                    Stairs* deleteStairs = temp -> getStairs();
                    delete deleteStairs;
                    delete temp;
                    // delete map and generate new map
                    p->addLevel();
                    
                    // display new map
                    m = new Map();
                    int player_posRow = 0;
                    int player_posCol = 0;
                    m->randomPos(player_posRow, player_posCol);
                    m->placePlayer(player_posRow, player_posCol);
                    p->setposCol(player_posCol);
                   p->setposRow(player_posRow);
                    if (p->getLevel() == 4) {
                        int idol_r = 0;
                        int idol_c = 0;
                        m->makeIdol(idol_r, idol_c); // makes idol
                        m -> randomPos(idol_r, idol_c); // makes random but valid position for idol
                        m -> placeIdol(idol_r, idol_c);
                        goldenIdol = new Object (idol_r, idol_c);
                        m->placeMonsters(); // randomly places monsters onto map
                        m->placeObjects(); // randomly places scrolls onto map
                        m->displayMap();
                        displayStats();
                        break;
                    } // if player is not at level 5 yet, still place stairs
                    // generate random but valid position for Player
                    m->placeMonsters(); // randomly places monsters onto map
                    m->placeObjects(); // randomly places scrolls onto map
                    m->placeStairs();
                    m->displayMap();
                    displayStats();
                    break;
                }
                break;
            case 'i': {
                clearScreen();
                p -> displayInventory();
                char command = getCharacter();
                switch(command) { // to exit out of inventory and go back to original map, press any key
                    default:
                        m->placePlayer(p->posRow(), p->posCol());
                        m->displayMap();
                        displayStats();
                        break;
                }
            }
                break;
            case 'c': // cheat option
                p->setHP(50);
                m->displayMap();
                displayStats();
                break;
            case ARROW_LEFT:
            case ARROW_RIGHT:
            case ARROW_UP:
            case ARROW_DOWN: {
                string p_sentence = "";
                string m_sentence = "";
                
            
                p->movePlayer(m, p->posRow(), p->posCol(), command, p_sentence, m_sentence);
                
                m->reprintMapObjects(this, m, p);
                m->placePlayer(p->posRow(), p->posCol());
                monstersMayMove(p, m);
               
               
                m->displayMap();
                displayStats();
                cout << endl;
                cout << endl;
                cout << p_sentence << endl;
                cout << m_sentence << endl;
                break;
            }
            case 'g': // pick up object on map
                string pickUp_sentence = "";
                if (p->pickUpObject(m, pickUp_sentence)) {
                    cout << pickUp_sentence << endl;
                    break;
                }
                if (p->getLevel() == 4) { 
                    if (m->reachIdol(p, goldenIdol)) {
                        cout << "You pick up the golden idol" << endl;
                        cout << "Congratulations, you won!" << endl;
                        return;
                    }
                }
                if (p->getInventory().size() >25) {
                    cout << "Your knapsack is full; you can't pick that up." << endl;
                    break;
                }
                else { // if player picked up nothing, move on
                    break;
                }
        }
        if (p->getHit() <= 0) {
            cout << "Press q to exit game." << endl;
        }
        command = getCharacter();
        clearScreen();
    }
    
    ;
}
