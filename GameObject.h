//
//  Weapon.hpp
//  project3
//
//  Created by Catherine Wang on 5/14/24.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <string>
using namespace std;

class Object {
public:
    Object(int posRow, int posCol) : m_posRow(posRow), m_posCol(posCol) {}
    int posRow() const;
    int posCol() const;
    virtual ~Object() {}
private:
    int m_posRow, m_posCol;
};

class Stairs : public Object {
public:
    Stairs(int posRow, int posCol) : Object(posRow, posCol) {}
};

class Weapon : public Object {
public:
    Weapon(int posRow, int posCol, string action, int dex, int damage, string name) : Object(posRow, posCol), m_action(action), m_dex(dex), m_damage(damage), m_name(name) {}
    string getActionString() {return m_action; }
    int getDex() {return m_dex; }
    int getDamage() {return m_damage; }
    virtual string getWeaponName(Weapon* w);
private:
    int m_dex, m_damage;
    string m_action;
    string m_name;
};

class Mace : public Weapon {
public:
    Mace(int posRow, int posCol) : Weapon(posRow, posCol, "swings mace at", 0, 2, "mace") {}
};

class ShortSword : public Weapon {
public:
    ShortSword(int posRow, int posCol) : Weapon(posRow, posCol, "slashes short sword", 0, 2, "short sword") {}
};

class LongSword : public Weapon {
public:
    LongSword(int posRow, int posCol) : Weapon(posRow, posCol, "swings long sword", 2, 4, "long sword") {}
    
};

class MagicAx : public Weapon {
public:
    MagicAx(int posRow, int posCol) : Weapon(posRow, posCol, "chops magic ax", 5, 5, "magic ax") {}
};

class MFSleep : public Weapon {
public:
    MFSleep(int posRow, int posCol) : Weapon(posRow, posCol, "strikes magic fangs of sleep", 3, 2, "magic fangs of sleep") {}
};

class Scroll : public Object {
public:
    Scroll(int posRow, int posCol, string type) : Object(posRow, posCol), m_type(type){}
    string getType() {return m_type;}
private:
    string m_type;
};

#endif /* Weapon_hpp */
