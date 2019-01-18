#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <iostream>
#include <set>
#include <list>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
///átírandó
#define R 1
#define G 1
#define B 1
#define Rot 0


class Database;
class Block_prototype;
class Block;
class Side;
class Color_value;
class Pos;
class Connection;

///vertical = 0, horizontal = 1, ...
enum Path_type {vertical, horizontal, bot_to_right, bot_to_left, top_to_left, top_to_right};
/// black = 0, red = 1, green = 2, yellow = 3, blue = 4, purple = 5, turquoise = 6, white = 7 ///
enum Color {black, red, green, yellow, blue, purple, turquoise, white};


class Connection
{
public:
    Block* first;
    int first_s_id;
    Block* second;
    int second_s_id;

    Connection(Block* f, int a, Block* s, int b) : first(f), first_s_id(a), second(s), second_s_id(b) {} ;
    ~Connection(){};

};
class Pos
{
public:
    int x,y;
    Block* b;
    Pos(){};
    Pos(int _x, int _y):x(_x),y(_y){};
};
bool operator< (Pos a, Pos b)
{
    if(a.x < b.x) return true;
    if(a.x == b.x && a.y < b.y) return true;
    return false;
}
///#######################################################################################///
///####################################--COLOR_VALUE--####################################///
///#######################################################################################///
class Color_value
{
public:
    int indx;
    int value;
    Color col;
    Color_value(Color c, int v, int i) : indx(i), value(v), col(c) {};
    ~Color_value();
};
///########################################################################################///
///######################################--DATABASE--######################################///
///########################################################################################///
class Circle
{
public:
    set<Pos> ups;
    list<Connection> connect[3];

    Circle(set<Pos> &u, list<Connection> (&c)[3]);
};
class Database
{
public:
    set<Color_value*> color_values;

    bool rotateable;

    int db;

    bool is_four_turn[3];

    int straight_count[3];

    int turn_count[3];

    bool is_small_circle[3];

    set<Block_prototype*> a[8];
    set<Block_prototype*> all;
    set<Block*> all_blocks;
    set<Pos> ups;
    list<Connection> connect[3];
    vector<Circle> circles;

    Database();
    ~Database();

};
///#######################################################################################///
///##################################--BLOCK_PROTOTYPE--##################################///
///#######################################################################################///
class Block_prototype
{
public:
    string id;

    Side* bot_color;
    Side* left_color;
    Side* top_color;
    Side* right_color;
    Side* sides[4];

    int rotation = 0;

    Path_type path_types[3];

   void rotate_one()
    {
        Side* tmp = sides[0];
        sides[0] = sides[3];
        sides[3] = sides[2];
        sides[2] = sides[1];
        sides[1] = tmp;

        switch(path_types[0])
        {
            case 0: path_types[0] = horizontal; break;
            case 1: path_types[0] = vertical; break;
            case 2: path_types[0] = bot_to_left; break;
            case 3: path_types[0] = top_to_left; break;
            case 4: path_types[0] = top_to_right; break;
            case 5: path_types[0] = bot_to_right; break;
        }
        switch(path_types[1])
        {
            case 0: path_types[1] = horizontal; break;
            case 1: path_types[1] = vertical; break;
            case 2: path_types[1] = bot_to_left; break;
            case 3: path_types[1] = top_to_left; break;
            case 4: path_types[1] = top_to_right; break;
            case 5: path_types[1] = bot_to_right; break;
        }
        switch(path_types[2])
        {
            case 0: path_types[2] = horizontal; break;
            case 1: path_types[2] = vertical; break;
            case 2: path_types[2] = bot_to_left; break;
            case 3: path_types[2] = top_to_left; break;
            case 4: path_types[2] = top_to_right; break;
            case 5: path_types[2] = bot_to_right; break;
        }
        rotation = ((rotation + 1)%4);
    }
    void reset_rotation()
    {
        int rot_to_null = 4-rotation;
        for(int i = 0; i < rot_to_null; i++)
        {
            this->rotate_one();
        }
    }

    list<Block*> elements;

    Block_prototype(ifstream&);
    ~Block_prototype();
};
///#######################################################################################///
///#######################################--BLOCK--#######################################///
///#######################################################################################///
class Block
{
public:
    string id;

    Pos position;

    Side* bot_color;
    Side* left_color;
    Side* top_color;
    Side* right_color;
    Side* sides[4];

    int rotation;

    Path_type path_types[3];

    void rotate_one()
    {
        Side* tmp = sides[0];
        sides[0] = sides[3];
        sides[3] = sides[2];
        sides[2] = sides[1];
        sides[1] = tmp;

        switch(path_types[0])
        {
            case 0: path_types[0] = horizontal; break;
            case 1: path_types[0] = vertical; break;
            case 2: path_types[0] = bot_to_left; break;
            case 3: path_types[0] = top_to_left; break;
            case 4: path_types[0] = top_to_right; break;
            case 5: path_types[0] = bot_to_right; break;
        }
        switch(path_types[1])
        {
            case 0: path_types[1] = horizontal; break;
            case 1: path_types[1] = vertical; break;
            case 2: path_types[1] = bot_to_left; break;
            case 3: path_types[1] = top_to_left; break;
            case 4: path_types[1] = top_to_right; break;
            case 5: path_types[1] = bot_to_right; break;
        }
        switch(path_types[2])
        {
            case 0: path_types[2] = horizontal; break;
            case 1: path_types[2] = vertical; break;
            case 2: path_types[2] = bot_to_left; break;
            case 3: path_types[2] = top_to_left; break;
            case 4: path_types[2] = top_to_right; break;
            case 5: path_types[2] = bot_to_right; break;
        }
        rotation = ((rotation + 1)%4);
    }
    void reset_rotation()
    {
        int rot_to_null = 4-rotation;
        for(int i = 0; i < rot_to_null; i++)
        {
            this->rotate_one();
        }
    }

    Block(string, Side*, Side*, Side*, Side*, int, int, int);
    Block(Block*);
    ~Block();

    void set_pos(int x, int y){ position.x = x; position.y = y;};
    void set_pos(Pos p){position = p;};
};
///########################################################################################///
///########################################--SIDE--########################################///
///########################################################################################///
class Side
{
public:
    bool colors[3];
    Color RGB;

    Side();
    Side(Side*);
    ~Side();
};
///#######################################################################################///
///#####################################--OPERATORS--#####################################///
///#######################################################################################///
inline bool operator==(Pos a, Pos b)
{
    return a.x == b.x && a.y == b.y;
}
inline bool operator==(Side& a, Side& b)
{
    //cout << a.RGB << " faaaaasz";
    return a.RGB == b.RGB;
}
ostream &operator<<(ostream &os, Block* b)
{
    os << b->id << " " << b->position.x << " " << b->position.y << " " << b->rotation;
    return os;
}
istream &operator>>(istream &is, Side* s)
{
    int rgb;
    is >> rgb;
    s->RGB = (Color)rgb;
    is >> s->colors[0] >> s->colors[1] >> s->colors[2];
    return is;
}
ostream &operator<<(ostream &os, Block_prototype* b)
{
    os << b->id;
    return os;
}
bool operator<(Color_value& a, Color_value& b)
{
    if(a.value > b.value)
        return true;
    else if(a.value == b.value && a.col < b.col)
        return true;
    return false;
}
ostream &operator<<(ostream &os, Connection a)
{
    os <<"############################# -> CON: " << a.first->id << " " << a.first_s_id << " " << a.second->id << " " << a.second_s_id << endl;
    return os;
}
ostream &operator<<(ostream &os, Pos a)
{
    os << "x: " << a.x << " y: " << a.y << endl;
    return os;
}
///########################################################################################///
///######################################--DATABASE--######################################///
///########################################################################################///
Database::Database()
{
    rotateable = Rot;
}
Database::~Database()
{
    for(size_t i = 0; i < 8; i++)
        for(Block_prototype* bp : a[i])
            delete bp;
}
///#######################################################################################///
///##################################--BLOCK_PROTOTYPE--##################################///
///#######################################################################################///
Block_prototype::Block_prototype(ifstream& f)
{
    bot_color = new Side();
    left_color = new Side();
    top_color = new Side();
    right_color = new Side();
    sides[0] = bot_color;
    sides[1] = left_color;
    sides[2] = top_color;
    sides[3] = right_color;
    int red_p;
    int green_p;
    int blue_p;
    int db;
    f >> id >> bot_color >> left_color >> top_color >> right_color >> red_p >> green_p >> blue_p >> db >> ws;
    path_types[0] = (Path_type)red_p;
    path_types[1] = (Path_type)green_p;
    path_types[2] = (Path_type)blue_p;
    for(int i = 0; i < db; i++)
    {
        Block* b = new Block(id, bot_color, left_color, top_color, right_color, red_p, green_p, blue_p);
        elements.push_back(b);
    }
   /// cout << "######" << this << "#######\n";


}
Block_prototype::~Block_prototype()
{
    for(int i = 0; i < 4; i++)
        sides[i] = nullptr;
    delete bot_color;
    delete left_color;
    delete top_color;
    delete right_color;
    for(Block* b : elements)
        delete b;
}
///#######################################################################################///
///#######################################--BLOCK--#######################################///
///#######################################################################################///
Block::Block(string s, Side* bot_s, Side* left_s, Side* top_s, Side* right_s, int red_p, int green_p, int blue_p)
{
    id = s;
    bot_color = new Side(bot_s);
    left_color = new Side(left_s);
    top_color = new Side(top_s);
    right_color = new Side(right_s);
    path_types[0] = (Path_type)red_p;
    path_types[1] = (Path_type)green_p;
    path_types[2] = (Path_type)blue_p;
    rotation = 0;
    sides[0] = bot_color;
    sides[1] = left_color;
    sides[2] = top_color;
    sides[3] = right_color;
 ///   cout << "######" << this << "######" << endl;
}
Block::Block(Block* old)
{
    cout << "\n Block copy constructor BEGIN \n \n";
    id = old->id;
    cout << "\n Block copy constructor END \n \n";
    bot_color = new Side(old->bot_color);
    left_color = new Side(old->left_color);
    top_color = new Side(old->top_color);
    right_color = new Side(old->right_color);
    for(int i=0; i<(sizeof (old->path_types)); i++)
    {
        path_types[i] = old->path_types[i];
    }
    sides[0] = bot_color;
    sides[1] = left_color;
    sides[2] = top_color;
    sides[3] = right_color;

}
Block::~Block()
{
    for(int i = 0; i < 4; i++)
        sides[i] = nullptr;
    delete bot_color;
    delete left_color;
    delete top_color;
    delete right_color;
}
///########################################################################################///
///########################################--SIDE--########################################///
///########################################################################################///
Side::Side()
{

}
Side::Side(Side* s)
{
    RGB = s->RGB;
    colors[0] = s->colors[0];
    colors[1] = s->colors[1];
    colors[2] = s->colors[2];
}
Side::~Side()
{

}
///########################################################################################///
///########################################--READ--########################################///
///########################################################################################///
void read(Database* data)
{
    ifstream conf("../input_gen/config.txt");
    if(!conf.good())
    {
        cerr << "szar a file!!! :((";
        exit(1);
    }
    ///config beolvasása:
    {
        bool b;
        conf >> b;
        if(!b)
        {
            cerr << "nincs megoldas\n";
            exit(0);
        }
        conf >> data->turn_count[0] >> data->turn_count[1] >> data->turn_count[2]
             >> data->is_four_turn[0] >> data->is_four_turn[1] >> data->is_four_turn[2]
             >> data->straight_count[0] >> data->straight_count[1] >> data->straight_count[2]
             >> data->is_small_circle[0] >> data->is_small_circle[1] >> data->is_small_circle[2];
        if(data->rotateable)
        {
            if(data->turn_count[0] >= 4)
            {
                Color_value* c = new Color_value(red, R, 0);
                data->color_values.insert(c);
            }
            if(data->turn_count[1] >= 4)
            {
                Color_value* c = new Color_value(green, G, 1);
                data->color_values.insert(c);
            }
            if(data->turn_count[2] >= 4)
            {
                Color_value* c = new Color_value(blue, B, 2);
                data->color_values.insert(c);
            }
        }
        else
        {
            if(data->is_four_turn[0])
            {
                Color_value* c = new Color_value(red, R, 0);
                data->color_values.insert(c);
            }
            if(data->is_four_turn[1])
            {
                Color_value* c = new Color_value(green, G, 1);
                data->color_values.insert(c);
            }
            if(data->is_four_turn[2])
            {
                Color_value* c = new Color_value(blue, B, 2);
                data->color_values.insert(c);
            }
        }
    }

    /// Color_values sorting BEGIN

    /// Color_values sorting END

    ifstream f("../input_gen/1o2i.txt");
    if(!f.good())
    {
        cerr << "szar a file!!! :((";
        exit(1);
    }
    while(f.good())
    {
        Block_prototype* tmp = new Block_prototype(f);
        data->a[tmp->bot_color->RGB].insert(tmp);
        data->a[tmp->left_color->RGB].insert(tmp);
        data->a[tmp->top_color->RGB].insert(tmp);
        data->a[tmp->right_color->RGB].insert(tmp);
        data->all.insert(tmp);
    }
    for(Block_prototype*  bp : data->all)
    {
    	for(Block* b : bp->elements)
    	{
    		data->all_blocks.insert(b);
    	}
    }
    cout << "osszes elem szama: " << data->all_blocks.size() <<endl;
}

Circle::Circle(set<Pos> &u, list<Connection> (&c)[3])
{
    cout << "new circle found, size: " << u.size() << endl;
    cout << "copying positions:" << endl;
    for(Pos p : u)
    {
        Pos temp = p;
        Block* new_block;
        new_block = new Block(p.b);                     ///EZ A SOR NEM FUT LE, CSAK AZ ELSÕ CSEMPÉNÉL
        cout << "tile copied: " << p.b << endl;
        temp.b = new_block;
        temp.b->set_pos(p.b->position);
        for(int rot=0; rot < p.b->rotation; rot++)
        {
            temp.b->rotate_one();
        }
        ups.insert(temp);
    }
    cout << "copying connections:" << endl;
    for(int i = 0; i < 3; i++)
    {
        for(Connection& con : c[i])
        {
            connect[i].push_back(con);
        }
    }
}

#endif // BLOCK_HPP_INCLUDED
