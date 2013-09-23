#include "group.h"

using namespace std;

group::group()
{
	ancestor=NULL;
}

group::~group()
{
	delete ancestor;
}

group* group::get_ancestor_address()
{
	return *ancestor;
}

group** group::get_ancestor()
{
	return ancestor;
}

void group::set_ancestor(group *a)
{
	if(a->get_ancestor()!=NULL)//他不是祖先
		*ancestor=a->get_ancestor_address();
	else
		*ancestor=a;
}

void group::set_father(group *f)
{
	ancestor=&f;
}