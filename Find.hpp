#ifndef FIND_HPP_INCLUDED
#define FIND_HPP_INCLUDED

#include "Block.hpp"
#include <vector>
#include <queue>
#include <cmath>
#include <sstream>

using namespace std;

#define debug  //cout << "debug" << endl;

void rekurziv_force(Block* act, int act_in, Pos last_pos, Database* data, Color_value* cv);
void max_circle(vector<Circle> all_circles);

class Option
{
public:
    Block_prototype* bp;
    int rot;
    Option(Block_prototype* blockp, int r):bp(blockp), rot(r){};

};

void bubbleSort(vector<pair<double,Option>> &a)         ///SZERINTEM SZAR, A MASIK ERTELMES, EZT IS AT KENE IRNI, LEHET MEGJAVUL A REKURZIV
{
    bool swapp = true;
    while(swapp)
    {
        swapp = false;
        for (size_t i = 0; i < a.size()-1; i++)
        {
            if (a[i].first < a[i+1].first )
            {
                pair<double, Option> temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
//                a[i].first += a[i+1].first;
//                a[i+1].first = a[i].first - a[i+1].first;
//                a[i].first -= a[i+1].first;
                swapp = true;
            }
        }
    }
}

void bubbleSort(vector<pair<double,Block*>> &a)         ///JO SORT
{
    bool swapp = true;
    while(swapp)
    {
        swapp = false;
        for (size_t i = 0; i < a.size()-1; i++)
        {
            if (a[i].first > a[i+1].first )
            {
                pair<double, Block*> temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
                swapp = true;
            }
        }
    }
}

Pos count_pos(Pos p, int a)
{
    switch(a)
    {
        case 0: p.y++; break;
        case 1: p.x--; break;
        case 2: p.y--; break;
        case 3: p.x++; break;
    }
    return p;
}

double weight(Pos act, Pos last, Path_type x, int j, int distance)
{
	///TODO
    //cout << "x:" << x << endl;
    //cout << "j:" << j << endl;
	static int dist = distance;
    switch(x)
    {
        case 0: j = (j + 2) % 4; break;
        case 1: j = (j + 2) % 4; break;
        case 2: if(j == 0) j = (j+3)%4; else j = (j+1)%4; break;
        case 3: if(j == 1) j = (j+3)%4; else j = (j+1)%4; break;
        case 4: if(j == 2) j = (j+3)%4; else j = (j+1)%4; break;
        case 5: if(j == 3) j = (j+3)%4; else j = (j+1)%4; break;
    }
    //cout << "j:" << j << endl;
    Pos future = count_pos(act, j);
    double ret = 1.0*(sqrt((future.x-last.x)*(future.x-last.x) + (future.y-last.y)*(future.y-last.y)));
    //cout << "ret:" << ret << " dist: " << distance << endl;
    if(ret <= dist)
    {
        return ret;
    }
    return 0-ret;

}

void connect_block(Pos& act, set<Pos> &ups, list<Connection> (&con)[3])
{
    for(int side_id = 0; side_id < 4; side_id++)
    {
        set<Pos>::iterator it = ups.find(count_pos(act, side_id));
        if(it != ups.end())
        {
            Connection c(act.b,side_id,(*it).b,((side_id+2)%4));
            for(size_t i = 0; i < sizeof act.b->sides[side_id]->colors; i++)
            {
                if(act.b->sides[side_id]->colors[i] == 1)
                {
                    con[i].push_back(c);
                    //cout << "i:" << i << ", " << c;
                }
            }
        }
    }
//    data->connect[cv->indx].push_back(con);
//    //cout << con;
}

///XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
bool chmod_check(Block_prototype* patient, Pos& patient_pos, Database* data)
{
    bool ok = true;
    for(int it = 0; it < 4 && ok; it++)
    {
        Pos pos_to_check = count_pos(patient_pos,it);
        set<Pos>::iterator it_p = data->ups.find(pos_to_check);
        if(it_p != data->ups.end())
        {
            //ok = ok && (patient->sides[it]->RGB == (*it_p).b->sides[(it+2)%4]->RGB);
        	ok = (ok && (patient->sides[it]->RGB == (*it_p).b->sides[(it+2)%4]->RGB));
        //cout << "---------- chmod kül: " << (patient->sides[it]->RGB - (*it_p).b->sides[(it+2)%4]->RGB) <<endl;
        }
    }
    //cout << "------------------------------------------ chmod jó-e: "<< ok << endl;
    return ok;
}
void put_down(Block* to_put, Pos& p, Database* data)
{
    p.b = to_put;
    to_put->set_pos(p);
    data->ups.insert(p);
    connect_block(p, data->ups, data->connect);
    data->all_blocks.erase(data->all_blocks.find(to_put));

    static int counter = 0;
    stringstream s;
    s << "2o3i_" << counter << ".txt";
    counter++;
    ofstream of(s.str());
    for(set<Pos>::iterator it = data->ups.begin(); it != data->ups.end(); it++)
    {
        set<Pos>::iterator szar = data->ups.end();
        szar--;
        if(it != szar)
            of << (*it).b << endl;
        else
            of << (*it).b;
    }
}
list<Connection>::iterator find_con(Block* b, list<Connection> &ls)
{
    list<Connection>::iterator it= ls.begin();
    while(it != ls.end())
    {
        if((*it).first == b || (*it).second == b)
            return it;
        it++;
    }
    return it;
}
void pick_up(Block* to_pick, Pos& p, Database* data)
{
 ///   to_pick->rotation=0;
    data->ups.erase(data->ups.find(p));
    for(int i = 0; i < 3; i++)
    {
        list<Connection>::iterator it = find_con(to_pick,data->connect[i]);
        while( it != data->connect[i].end())
        {
            debug;
            data->connect[i].erase(it);
            it = find_con(to_pick,data->connect[i]);
        }
    }
    p.b = nullptr;
}
void can_be_placed(Block_prototype* patient, Pos& patient_pos, Pos& last_pos, vector<pair<double,Option>>& options,
                    bool rotateable, Database* data,  Color_value* cv, int patient_in)
{
    if(rotateable)
    {
        for(size_t rot = 0; rot < 4; rot++)
        {
            if(chmod_check(patient,patient_pos, data))
            {
                Option op(patient,rot);
                int size_all = (data->all_blocks.size()/2);
                if(patient->elements.size() == 0)
                	return;
//                //cout << "ez a cv indx: " << cv->indx << endl;
//                //cout << "ez a path_type: " << patient->path_types[cv->indx] << endl;
                pair<double, Option> pr = make_pair(weight(patient_pos, last_pos, patient->path_types[cv->indx], patient_in, size_all), op);
                if(pr.first >= 0.0)
                {
                	options.push_back(pr);
                }
//                //cout << "ez az op merete: " << options.size() << endl;
            }
            patient->rotate_one();
        }
        patient->reset_rotation();
    }
    else
    {
        ///queue feltölt
        {
            if(chmod_check(patient,patient_pos, data))
            {
            debug;
                Option op(patient,0);
                int size_all = (data->all_blocks.size()/2);
                if(patient->elements.size() == 0)
                	return;
                pair<double, Option> pr = make_pair(weight(patient_pos, last_pos, patient->path_types[cv->indx], patient_in, size_all),op);
                if(pr.first >= 0.0)
                {
                	options.push_back(pr);
                }
                /////cout << "ez az op merete: " << options.size() << endl;
            }
        }
    }
}

void rekurziv_force(Block* act, int act_in, Pos last_pos, Database* data, Color_value* cv)
{
    size_t act_out = 0;
    bool ex = false;
    while(!ex)
    {
//        //cout << act << " -> ";
//        //cout << "ITT" << endl;
//        //cout << "act_in: " << act_in << " ";
//        //cout <<"act_out: " << act_out << endl;
//        //cout << ((act->sides[act_out]->colors[cv->indx] == 1) && (act_out != act_in)) << endl;
        if((act->sides[act_out]->colors[cv->indx] == 1) && (act_out != act_in)) {ex = true;}
        else act_out++;
    }
//    //cout << "act_in:" << act_in << " act_out:" << act_out << "\n";
    vector<pair<double,Option>> options;
    Pos future_pos = count_pos(act->position, act_out);
    int future_in = ((act_out+2)%4);
    for(Block_prototype* bp : data->a[act->sides[act_out]->RGB])
    {
        can_be_placed(bp, future_pos, last_pos, options, data->rotateable, data, cv, future_in);
    }
    cout << "############################### " << options.size() << endl;
    ///TODO
    if(options.size() > 1)
    {
    	bubbleSort(options);
    }
	while(!options.empty())
	{
		Option current = options[0].second;
		options.erase(options.begin());
		if(current.bp->elements.size() == 0)
		{
			continue;
		}
		Block* recent = *current.bp->elements.begin();
		current.bp->elements.pop_front();
		//cout << "######################################rot: "  << current->rot << endl;
//		recent->rotation = current.rot;
        for(int i = 0; i < current.rot; i++)
        {
            recent->rotate_one();
        }
		///lerak
		put_down(recent,future_pos,data);
		///if
			///kör vagy rek
			cout << "pos: " << future_pos << "pos_l: " << last_pos;
		if(future_pos == last_pos)
		{
		    cout << "\n############# Future == Last type BEGIN\n";

		    for(set<Pos>::iterator it = data->ups.begin(); it != data->ups.end(); it++)
            {
			set<Pos>::iterator szar = data->ups.end();
			szar--;
			cout << "kor eleme copy elott: " << (*it).b << endl;
            }

            cout << "recent block: " << recent << endl;

			Circle c(data->ups, data->connect);
			data->circles.push_back(c);
		    ///
		    for(set<Pos>::iterator it = c.ups.begin(); it != c.ups.end(); it++)
            {
			set<Pos>::iterator szar = c.ups.end();
			szar--;
			cout << "kor eleme copy utan: " << (*it).b << endl;
            }
		    ///
			static int count_laps = 1;
			cout << "circle counter: " << count_laps << endl;
			count_laps++;
			//max_circle(data->circles);
			//exit(0);
		    cout << "\n############# Future == Last type END\n";
        }
		else
		{
			rekurziv_force(recent, future_in, last_pos, data, cv);
		}
		///felvesz
		pick_up(recent, future_pos, data);
		recent->reset_rotation();
		current.bp->elements.push_back(recent);
		data->all_blocks.insert(recent);
	}
    ///TODO
}

void rek_fv_prev(Database* data, Color_value* cv)
{

    Block* first = *((*data->all.begin())->elements.begin());
    Pos first_pos(0,0);
    put_down(first, first_pos, data);
    (*data->all.begin())->elements.pop_front();
    size_t id_to_last = 0;
    do
    {
        if(first->sides[id_to_last]->colors[cv->indx]) break;
    }while(id_to_last++);
    Pos last = count_pos(first->position, id_to_last);
    //cout << first << endl;
    //cout << "last: " << last.x  << " " << last.y << endl;

    ///rekurzió hívás
    rekurziv_force(first, id_to_last, last, data, cv);
    //max_circle(data->circles);
}

///XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void brute_force(Database* data, Color_value* cv)
{
    ///min keresés (db) + adathalmaz felép

    Block_prototype* min_p = *data->all.begin();
    for(Block_prototype* p : data->all)
    {
        if(min_p->elements.size() > p->elements.size())
            min_p = p;
    }
    vector<Block*> v;
    int minimum = min_p->elements.size();
    for(Block_prototype* p : data->all)
    {
        for(int i = 0; i < minimum; i++)
        {
            v.push_back(*p->elements.begin());
            p->elements.pop_front();
        }
    }


    int distance = v.size()/2;
    Block* first = v[0];
    v.erase(v.begin());
    first->set_pos(0,0);
    first->position.b = first;
    data->ups.insert(first->position);
    size_t i = 0;
    do
    {
        if(first->sides[i]->colors[cv->indx]) break;
    }while(i++);
//                //cout << first->position.x << " " << first->position.y << "  " << i << endl;
    Pos last = count_pos(first->position, i);
    //cout << last.x  << " " << last.y << endl;


    Block* tmp = first;
    ///
    while(data->ups.find(last) == data->ups.end())
    {
        if(v.size() == 1)
        {
            size_t j = 0;
            bool ex = false;
            while(!ex)
            {
//                            //cout << tmp << " -> ";
//                            //cout << "ITT" << endl;
//                            //cout << "i: " << i << " ";
//                            //cout <<"j: " << j << endl;
//                            //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
                if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
                else j++;
            }
            Pos act = count_pos(tmp->position, j);
            Block* temp = v[0];
            //cout << temp << endl;
            int step_in = ((j+2)%4);
            bool ok = true;
            for(int it = 0; it < 4 && ok; it++)
            {
                Pos posit = count_pos(act, (step_in + it + 1)%4);
                set<Pos>::iterator it_p = data->ups.find(posit);
                if(it_p != data->ups.end())
                {
                    //cout << "####--" << (*it_p).b << " --###-- " << (temp->sides[((step_in + it + 1)%4)]->RGB) << " " << (((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB) << endl;
                    ok = ok && (temp->sides[((step_in + it + 1)%4)]->RGB == ((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB);
                }
            }
            if(ok)
            {
                temp->position = act;
                act.b = temp;
                data->ups.insert(act);

                connect_block(act,data->ups,data->connect);

//                            Connection con(tmp,j,temp,((j+2)%4));
//                            data->connect[cv->indx].push_back(con);
//                            //cout << con;

                v.erase(v.begin());

                ///set-bõl a szomszédok kellenek
                ///Connection con_2();
            }
        }
        else
        {
//                    //cout << (data->ups.find(last) == data->ups.end()) << endl;
            size_t j = 0;
            bool ex = false;
            while(!ex)
            {
                //cout << tmp << " -> ";
                //cout << "ITT" << endl;
                //cout << "i: " << i << " ";
                //cout <<"j: " << j << endl;
                //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
                if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
                else j++;
            }
            //cout << "i:" << i << " j:" << j << "\n";
            vector<pair<double, Block*>> options;
            Pos act = count_pos(tmp->position, j);
            debug;
            for(Block* b : v)
            {
                //cout << b << " -> ";
                //cout << b->sides[3]->RGB << endl;
                //cout << (data->ups.find(act) == data->ups.end()) << " " << (tmp->sides[j] == b->sides[((j+2)%4)]) << endl;
                if(data->ups.find(act) == data->ups.end() && tmp->sides[j]->RGB == b->sides[((j+2)%4)]->RGB)
                {
                    int step_in = ((j+2)%4);
                    bool ok = true;
                    for(int it = 0; it < 3 && ok; it++)
                    {
                        Pos posit = count_pos(act, (step_in + it + 1)%4);
                        set<Pos>::iterator it_p = data->ups.find(posit);
                        if(it_p != data->ups.end())
                        {
                            ok = ok && (b->sides[((step_in + it + 1)%4)] == (*it_p).b->sides[(((step_in + it + 1)%4)+2)%4]);
                        }
                    }
                    if(ok)
                    {
                        //cout << b << "j:" << j << endl;
                        options.push_back(make_pair(weight(act,last,b->path_types[cv->indx],((j+2)%4), distance), b));
                    }
                }
            }
            Block* temp = options[0].second;
//                    //cout << "options size: " << options.size() << endl;
            temp->position = act;
            act.b = temp;
            data->ups.insert(act);
//                    //cout << data->ups.size() << endl;


            connect_block(act, data->ups, data->connect);

//                        Connection con(tmp,j,temp,((j+2)%4));
//                        data->connect[cv->indx].push_back(con);
//                        //cout << con;



            int iter = 0;
            for(Block* findable : v)
            {
                if(findable == temp)
                    break;
                iter++;
            }
            v.erase(v.begin()+iter);
            i = ((j+2)%4);
            tmp = temp;
//                    //cout << (*data->ups.find(last)).y << endl;
        }
    }

    /////////////
}

void alap_force(Database* data, Color_value* cv)
{
    cout << "eppen nezett szin: " << cv->col << endl;

    ///min keresés (db) + adathalmaz felép

    Block_prototype* base_p = *data->all.begin();

    vector<Block*> v;

    {
        for(int i = 0; i < 4; i++)
        {
            Block* temp = *base_p->elements.begin();
            base_p->elements.pop_front();
            for(int r=0; r<i; r++)
            {
                temp->rotate_one();
            }
            v.push_back(temp);
        }
    }
    cout << "v.size: " << v.size() << endl;

    int distance = v.size()/2;
    Block* first = v[0];
    v.erase(v.begin());
    first->set_pos(0,0);
    first->position.b = first;


    data->ups.insert(first->position);
    size_t i = 0;
    do
    {

        if(first->sides[i]->colors[cv->indx] == 1) break;
    }while(i++);

    cout << "first block: " << first << " side_id_future_in: " << i << endl;

    Pos last = count_pos(first->position, i);

    cout << "future last block position: " << last << endl;

    Block* tmp = first;

    cout << "tmp block: " << tmp << endl;
    ///
    while(data->ups.find(last) == data->ups.end())
    {
        if(v.size() == 1)
        {
            cout << "#### v.size szerinti == 1 case: " << endl << endl;
            size_t j = 0;
            bool ex = false;
            while(!ex)
            {
//                            //cout << tmp << " -> ";
//                            //cout << "ITT" << endl;
//                            //cout << "i: " << i << " ";
//                            //cout <<"j: " << j << endl;
//                            //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
                if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
                else j++;
            }
            Pos act = count_pos(tmp->position, j);
            Block* temp = v[0];
            //cout << temp << endl;
            int step_in = ((j+2)%4);
            bool ok = true;
            for(int it = 0; it < 4 && ok; it++)
            {
                Pos posit = count_pos(act, (step_in + it + 1)%4);
                set<Pos>::iterator it_p = data->ups.find(posit);
                if(it_p != data->ups.end())
                {
                    //cout << "####--" << (*it_p).b << " --###-- " << (temp->sides[((step_in + it + 1)%4)]->RGB) << " " << (((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB) << endl;
                    ok = ok && (temp->sides[((step_in + it + 1)%4)]->RGB == ((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB);
                }
            }
            if(ok)
            {
                temp->position = act;
                act.b = temp;
                data->ups.insert(act);

                connect_block(act,data->ups,data->connect);

//                            Connection con(tmp,j,temp,((j+2)%4));
//                            data->connect[cv->indx].push_back(con);
//                            //cout << con;

                v.erase(v.begin());

                ///set-bõl a szomszédok kellenek
                ///Connection con_2();
            }
            cout << endl << "#### v.size szerinti == 1 case END: " << endl << endl;
        }
        else
        {
            cout << "#### v.size szerinti != 1 case: " << endl << endl;
//                    //cout << (data->ups.find(last) == data->ups.end()) << endl;
            size_t j = 0;
            bool ex = false;
            while(!ex)
            {
                //cout << tmp << " -> ";
                //cout << "ITT" << endl;
                //cout << "i: " << i << " ";
                //cout <<"j: " << j << endl;
                //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
                if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
                else j++;
            }
            //cout << "i:" << i << " j:" << j << "\n";
            vector<pair<double, Block*>> options;
            Pos act = count_pos(tmp->position, j);
            debug;
            for(Block* b : v)
            {
                //cout << b << " -> ";
                //cout << b->sides[3]->RGB << endl;
                //cout << (data->ups.find(act) == data->ups.end()) << " " << (tmp->sides[j] == b->sides[((j+2)%4)]) << endl;
                if(data->ups.find(act) == data->ups.end() && tmp->sides[j]->RGB == b->sides[((j+2)%4)]->RGB)
                {
                    int step_in = ((j+2)%4);
                    bool ok = true;
                    for(int it = 0; it < 3 && ok; it++)
                    {
                        Pos posit = count_pos(act, (step_in + it + 1)%4);
                        set<Pos>::iterator it_p = data->ups.find(posit);
                        if(it_p != data->ups.end())
                        {
                            ok = ok && (b->sides[((step_in + it + 1)%4)] == (*it_p).b->sides[(((step_in + it + 1)%4)+2)%4]);
                        }
                    }
                    if(ok)
                    {
                        //cout << b << "j:" << j << endl;
                        double w = weight(act,last,b->path_types[cv->indx],((j+2)%4), distance);
                        cout << "option: block: " << b << " weight: " << w << endl;
                        if(w >= 0)
                        {
                            options.push_back(make_pair(w, b));
                        }
                    }
                }
            }
            cout << "options before sort: \n";
              for(pair<double, Block*> o: options)
            {
                cout << "opt. : " << o.second  << "w: " << o.first << endl;
            }
            bubbleSort(options);
            cout << "options after sort: \n";
            for(pair<double, Block*> o: options)
            {
                cout << "opt. : " << o.second  << " w: " << o.first << endl;
            }
            Block* temp = options[0].second;
                    cout << "options size: " << options.size() << endl;
            temp->position = act;
            act.b = temp;
            data->ups.insert(act);
//                    //cout << data->ups.size() << endl;


            connect_block(act, data->ups, data->connect);

//                        Connection con(tmp,j,temp,((j+2)%4));
//                        data->connect[cv->indx].push_back(con);
//                        //cout << con;



            int iter = 0;
            for(Block* findable : v)
            {
                if(findable == temp)
                    break;
                iter++;
            }
            v.erase(v.begin()+iter);
            i = ((j+2)%4);
            tmp = temp;
            cout << "new temp block: " << temp << endl;
//                    //cout << (*data->ups.find(last)).y << endl;
            cout << endl << "#### v.size szerinti != 1 case END: " << endl << endl;
        }

    }

    /////////////
}

///XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

list<Connection>::iterator find_next(Block* b, int side_id, list<Connection> &ls)
{
    list<Connection>::iterator it= ls.begin();
    while(it != ls.end())
    {
        if((*it).first == b && side_id == (*it).first_s_id)
            return it;
        if((*it).second == b && side_id == (*it).second_s_id)
        {
            Block* b = (*it).first;
            (*it).first = (*it).second;
            (*it).second = b;
            int a = (*it).first_s_id;
            (*it).first_s_id = (*it).second_s_id;
            (*it).second_s_id = a;
            return it;
        }
        it++;
    }
    return it;
}
 ///nagy komment

void max_circle(vector<Circle> all_circles)
{
	if(all_circles.size() != 0)
    {
		Circle max_circle = all_circles[0];

		/*for(int i = 1; i < all_circles.size(); i++)
		{
			if(all_circles[i].ups.size() > max_circle.ups.size())
			{
				max_circle = all_circles[i];
			}
		}*/
		//cout << "---------------EZ ITT A MÉRETE:" << all_circles.size() << endl;
		ofstream of("2o3i.txt");

		for(set<Pos>::iterator it = max_circle.ups.begin(); it != max_circle.ups.end(); it++)
		{
			set<Pos>::iterator szar = max_circle.ups.end();
			szar--;
			cout << "kor eleme: " << (*it).b << endl;
 			if(it != szar)
				of << (*it).b << endl;
			else
				of << (*it).b;

		}
    }
	else cout << "nincs kör :(\n";
}

void flat_smart(Database* data, Color_value* cv)
{
    ofstream of("2o3i.txt");
    Block_prototype* max_p = *data->all.begin();
    for(Block_prototype* p : data->all)                 ///egyenes (legnagyobb elemszámú) típus megkeresése
    {
        if(max_p->elements.size() < p->elements.size())
            max_p = p;
    }
    unsigned int smart_parameter;
    smart_parameter = max_p->elements.size() / 2;
    cout << "smart parameter: " << smart_parameter << endl;
    if(data->rotateable)                                ///frogatás esetén kanyarok kreálása
    {
        cout << "forgatasos indy: \n";
        vector<Block_prototype*> to_generate;

        for(Block_prototype* p : data->all)             ///kanyar típusúak kiválasztása
        {
            cout << "current bp path_type: " << p->path_types[cv->indx] <<endl;
            switch (p->path_types[cv->indx])
            {
                case 0 : break;
                case 1 : break;
                case 2 :
                    {
                        to_generate.push_back(p);
                        break;
                    }
                case 3 :
                    {
                        to_generate.push_back(p);
                        break;
                    }
                case 4 :
                    {
                        to_generate.push_back(p);
                        break;
                    }
                case 5 :
                    {
                        to_generate.push_back(p);
                        break;
                    }
             }
        }
        for(Block_prototype* p : to_generate)           ///kanyar típusúak megfelelő mennyiségű reprodukálása
        {
            if(to_generate.size() == 1)
            {
                for(int rot = 1; rot < 4; rot++)
                {
                    Block_prototype* new_b_type = new Block_prototype(p);
                    for(int i=0; i<rot; i++)
                    {
                        new_b_type->rotate_one();
                    }
                    data->all.insert(new_b_type);
                }
            }
            else if(to_generate.size() == 2)
            {
                for(int rot = 1; rot < 2; rot++)
                {
                    Block_prototype* new_b_type = new Block_prototype(p);
                    for(int i=0; i<rot; i++)
                    {
                        new_b_type->rotate_one();
                    }
                    data->all.insert(new_b_type);
                }
            }
        }

    }
    else                                                ///nincs forgatás, van elég kanyar
    {
        cout << "forgatas nelkuli indy: \n";

    }

    for(Block_prototype* p : data->all)                 ///adathalmaz okos lepakolása, kiíratása, pontszámítás kéne még
    {
        Pos curr_pos;
        Block* current_tile = (*p->elements.begin());
        current_tile->rotation = p->rotation;
        p->elements.pop_back();
        cout << "current tile path_type: " << p->path_types[cv->indx] << endl;
        switch (p->path_types[cv->indx])
        {
            case 0 :
                {
                    current_tile->rotate_one();
                    for(int x = 1; x <1 + smart_parameter; x++)
                    {
                        curr_pos = Pos(x,0);
                        current_tile->set_pos(curr_pos);
                        cout << "current tile: " << current_tile << endl;
                        of << current_tile << endl;

                        curr_pos = Pos(x, 1);
                        current_tile->set_pos(curr_pos);
                        cout << "current tile: " << current_tile << endl;
                        of << current_tile << endl;
                    }
                    break;
                }
            case 1 :
                {
                    for(int x = 1; x <1 + smart_parameter; x++)
                    {
                        curr_pos = Pos(x,0);
                        current_tile->set_pos(curr_pos);
                        cout << "current tile: " << current_tile << endl;
                        of << current_tile << endl;

                        curr_pos = Pos(x, 1);
                        current_tile->set_pos(curr_pos);
                        cout << "current tile: " << current_tile << endl;
                        of << current_tile << endl;
                    }
                    break;
                }
            case 2 :
                {
                    curr_pos = Pos(0,0);
                    current_tile->set_pos(curr_pos);
                    cout << "current tile: " << current_tile << endl;
                    of << current_tile << endl;
                    break;
                }
            case 3 :
                {
                    curr_pos = Pos(1 + smart_parameter, 0);
                    current_tile->set_pos(curr_pos);
                    cout << "current tile: " << current_tile << endl;
                    of << current_tile << endl;
                    break;
                }
            case 4 :
                {
                    curr_pos = Pos(1 + smart_parameter, 1);
                    current_tile->set_pos(curr_pos);
                    cout << "current tile: " << current_tile << endl;
                    of << current_tile << endl;
                    break;
                }
            case 5 :
                {
                    curr_pos = Pos(0, 1);
                    current_tile->set_pos(curr_pos);
                    cout << "current tile: " << current_tile << endl;
                    of << current_tile << endl;
                    break;
                }
        }
    }
}

void find_circle(Database* data)
{
    if(data->rotateable)                                ///VAN FORGATAS
    {
        cout << "van forgatas \n";
        for(Color_value* cv : data->color_values)
        {
            if(data->straight_count[cv->indx])              ///VAN EGYENES
            {
                cout << "van egyenes \n";
                for(Color_value* cv : data->color_values)
                {
                    cout << "forgatos indy kene legyen, okos keresessel: \n \n";
                    flat_smart(data, cv);
                    exit(0);
                }
            }
            else                                            ///NINCS EGYENES
            {
                cout << "nincs egyenes\n";
                for(Color_value* cv : data->color_values)
                {
                    if(data->all_blocks.size() <= 4)         ///ALAP MEGOLDO
                    {
                        alap_force(data, cv);
                        set<Block*> canvas;
                        ofstream of("2o3i.txt");
                        for(int i = 0; i < 3; i++)
                        {
                            for(Connection c : data->connect[i])
                            {
                                canvas.insert(c.first);
                                canvas.insert(c.second);
                            }
                        }
                        for(Block* b : canvas)
                        {
                            of << b << endl;
                        }
                        cout << "circle found: size: " << canvas.size() << " weight: " << cv->value << " color: " << cv->col << endl;
                    exit(0);
                    }
                    else                                    ///MONOKROM MEGOLDO
                    {
                        cout << "monokrom kene legyen, rekurziv keresessel: \n \n";
                        rek_fv_prev(data, cv);
                        cout << "elso pozicioju csempe: " << (*(data->ups.begin())).b << endl;
                        cout << "kereses megtortent, kiiratas jon: \n \n";
                        max_circle(data->circles);
                        break;
                    }
                }
            }
        }
    }
    else                                                ///NINCS FORGATAS
    {
        cout << "nincs forgatas \n";
        for(Color_value* cv : data->color_values)
        {
            if(data->straight_count[cv->indx])          ///TEREPASZTAL VAGY INDY MEGOLDO
            {
                cout << "van egyenes \n";
                if(data->is_small_circle[cv->indx] == true) ///INDY MEGOLDO
                {
                    cout << "indy kene legyen, okos keresessel: \n \n";
                    flat_smart(data, cv);
                    break;
                }
                else                                    ///TEREPASZTAL MEDOLDO
                {
                    rek_fv_prev(data, cv);
                    max_circle(data->circles);
                    break;
                }
            }
            else                                        ///ACHILLES MEGOLDO
            {
                cout << "nincs egyenes\n";
                brute_force(data, cv);
                set<Block*> canvas;
                ofstream of("2o3i.txt");
                for(int i = 0; i < 3; i++)
                {
                    for(Connection c : data->connect[i])
                    {
                        canvas.insert(c.first);
                        canvas.insert(c.second);
                    }
                }
                for(Block* b : canvas)
                {
                    of << b << endl;
                }
                exit(0);
                ///kurva nagy komment
                {
//                Block_prototype* min_p = *data->all.begin();
//                for(Block_prototype* p : data->all)
//                {
//                    if(min_p->elements.size() > p->elements.size())
//                        min_p = p;
//                }
//                vector<Block*> v;
//                int minimum = min_p->elements.size();
//                for(Block_prototype* p : data->all)
//                {
//                    for(int i = 0; i < minimum; i++)
//                    {
//                        v.push_back(*p->elements.begin());
//                        p->elements.pop_front();
//                    }
//                }
//                int distance = v.size()/2;
//                Block* first = v[0];
//                v.erase(v.begin());
//                first->set_pos(0,0);
//                first->position.b = first;
//                data->ups.insert(first->position);
//                size_t i = 0;
//                do
//                {
//                    if(first->sides[i]->colors[cv->indx]) break;
//                }while(i++);
////                //cout << first->position.x << " " << first->position.y << "  " << i << endl;
//                Pos last = count_pos(first->position, i);
//                //cout << last.x  << " " << last.y << endl;
//
//
//                Block* tmp = first;
//                ///
//                while(data->ups.find(last) == data->ups.end())
//                {
//                    if(v.size() == 1)
//                    {
//                        size_t j = 0;
//                        bool ex = false;
//                        while(!ex)
//                        {
////                            //cout << tmp << " -> ";
////                            //cout << "ITT" << endl;
////                            //cout << "i: " << i << " ";
////                            //cout <<"j: " << j << endl;
////                            //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
//                            if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
//                            else j++;
//                        }
//                        Pos act = count_pos(tmp->position, j);
//                        Block* temp = v[0];
//                        //cout << temp << endl;
//                        int step_in = ((j+2)%4);
//                        bool ok = true;
//                        for(int it = 0; it < 4 && ok; it++)
//                        {
//                            Pos posit = count_pos(act, (step_in + it + 1)%4);
//                            set<Pos>::iterator it_p = data->ups.find(posit);
//                            if(it_p != data->ups.end())
//                            {
//                                //cout << "####--" << (*it_p).b << " --###-- " << (temp->sides[((step_in + it + 1)%4)]->RGB) << " " << (((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB) << endl;
//                                ok = ok && (temp->sides[((step_in + it + 1)%4)]->RGB == ((*it_p).b->sides[(((step_in + it + 1)%4)+2)%4])->RGB);
//                            }
//                        }
//                        if(ok)
//                        {
//                            temp->position = act;
//                            act.b = temp;
//                            data->ups.insert(act);
//
//                            connect_block(act,data->ups,data->connect);
//
////                            Connection con(tmp,j,temp,((j+2)%4));
////                            data->connect[cv->indx].push_back(con);
////                            //cout << con;
//
//                            v.erase(v.begin());
//
//                            ///set-bõl a szomszédok kellenek
//                            ///Connection con_2();
//                        }
//                    }
//                    else
//                    {
//    //                    //cout << (data->ups.find(last) == data->ups.end()) << endl;
//                        size_t j = 0;
//                        bool ex = false;
//                        while(!ex)
//                        {
//                            //cout << tmp << " -> ";
//                            //cout << "ITT" << endl;
//                            //cout << "i: " << i << " ";
//                            //cout <<"j: " << j << endl;
//                            //cout << ((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) << endl;
//                            if((tmp->sides[j]->colors[cv->indx] == 1) && (j != i)) {ex = true;}
//                            else j++;
//                        }
//                        //cout << "i:" << i << " j:" << j << "\n";
//                        vector<pair<double, Block*>> options;
//                        Pos act = count_pos(tmp->position, j);
//                        debug;
//                        for(Block* b : v)
//                        {
//                            //cout << b << " -> ";
//                            //cout << b->sides[3]->RGB << endl;
//                            //cout << (data->ups.find(act) == data->ups.end()) << " " << (tmp->sides[j] == b->sides[((j+2)%4)]) << endl;
//                            if(data->ups.find(act) == data->ups.end() && tmp->sides[j]->RGB == b->sides[((j+2)%4)]->RGB)
//                            {
//                                int step_in = ((j+2)%4);
//                                bool ok = true;
//                                for(int it = 0; it < 3 && ok; it++)
//                                {
//                                    Pos posit = count_pos(act, (step_in + it + 1)%4);
//                                    set<Pos>::iterator it_p = data->ups.find(posit);
//                                    if(it_p != data->ups.end())
//                                    {
//                                        ok = ok && (b->sides[((step_in + it + 1)%4)] == (*it_p).b->sides[(((step_in + it + 1)%4)+2)%4]);
//                                    }
//                                }
//                                if(ok)
//                                {
//                                    //cout << b << "j:" << j << endl;
//                                    options.push(make_pair(weight(act,last,b->path_types[cv->indx],((j+2)%4), distance), b));
//                                }
//                            }
//                        }
//                        Block* temp = options.top().second;
//    //                    //cout << "options size: " << options.size() << endl;
//                        temp->position = act;
//                        act.b = temp;
//                        data->ups.insert(act);
//    //                    //cout << data->ups.size() << endl;
//
//
//                        connect_block(act, data->ups, data->connect);
//
////                        Connection con(tmp,j,temp,((j+2)%4));
////                        data->connect[cv->indx].push_back(con);
////                        //cout << con;
//
//
//
//                        int iter = 0;
//                        for(Block* findable : v)
//                        {
//                            if(findable == temp)
//                                break;
//                            iter++;
//                        }
//                        v.erase(v.begin()+iter);
//                        i = ((j+2)%4);
//                        tmp = temp;
//    //                    //cout << (*data->ups.find(last)).y << endl;
//                    }
//                    }
                }

                /////////////

            break;
            }
        }
    }


}




#endif // FIND_HPP_INCLUDED
