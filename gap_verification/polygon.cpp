#include "polygon.h"
 
polygon::polygon()
{
	id=0;
	x=0;
	y=0;
	width=0;
	height=0;
}

polygon::~polygon()
{}

void polygon::set_id(int i)
{
	id=i;
}

void polygon::set_X(int i)
{
	x=i;
}

void polygon::set_Y(int i)
{
	y=i;
}

void polygon::set_width(int i)
{
	width=i;
}

void polygon::set_height(int i)
{
	height=i;
}

int polygon::get_id()
{
	return id;
}

int polygon::get_X()
{
	return x;
}

int polygon::get_Y()
{
	return y;
}

int polygon::get_width()
{
	return width;
}

int polygon::get_height()
{
	return height;
}