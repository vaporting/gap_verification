#ifndef GROUP_H
#define GROUP_H
#include <iostream> 

using namespace std;

class group
{
public:
	group();
	~group();
	group* get_ancestor_address();
	group** get_ancestor();
	void set_ancestor(group *a);
	void set_father(group *f);
	
private:	
	group **ancestor;
};

#endif