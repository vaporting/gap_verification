#ifndef POLYGON_H
#define POLYGON_H
#include <iostream> 

using namespace std;

class polygon
{
public:
	polygon();
	~polygon();
	void set_id(int i);
	void set_X(int i);
	void set_Y(int i);
	void set_width(int i);
	void set_height(int i);
	int get_id();
	int get_X();
	int get_Y();
	int get_width();
	int get_height();
private:
	int id;
	int x,y;
	int width,height;
};

#endif