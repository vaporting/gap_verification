#include <iostream> 
#include <fstream>
#include <queue>
#include <algorithm>
#include <time.h>
#include "group.h"

typedef struct polygon{
	int id;
	int x,y;
	int width,height;
	group *gid;
}Polygon;

typedef struct polygon_point{
	int x,y; //�I��x,y
	int in_out;
	Polygon *p;
}Polygon_point;

typedef struct pair{
	polygon_point first;
	polygon_point second;
}Pair;

bool compareY (Polygon_point *a, Polygon_point *b)  
{  
	return a->y>b->y;  
} 

bool compareX (Polygon_point *a, Polygon_point *b)  
{  
	return a->x<b->x;  
} 

bool compare(Pair *a,Pair *b)
{
	if(a->first.p->id==b->first.p->id)//first id�@�ˤ��second id
		return a->second.p->id<b->second.p->id;
	else
		return a->first.p->id<b->first.p->id;

}

void set(Polygon_point * point,int id,int x,int y,int width,int height,int in_out);
int* set_listY(vector<Polygon_point *> pointlist,Polygon_point **ylist,int ycount);
vector<Pair *> find_violate(Polygon_point **ylist,int *x_index,int spacing,int ycount);
int set_near(Polygon_point * curlist,Polygon_point cur_point,int cur_index,int spacing);
void set_pair(vector<Pair *> *violate,Polygon_point first,Polygon_point second);
void make_group(Polygon_point *current_point,Polygon_point *up_point);

int main(int argc, char *argv[]){
	//unsigned long start = clock();
	int i,n,polygon_number=0,spacing=0,time=0,ycount;
	int id,x,y,width,height;
	int *x_index;
	vector<Pair *> violate;
	ifstream file(argv[1]);
	if(!file)
		cout<<"open fail"<<endl;
	file>>time;
	while(time>0)
	{
		file>>polygon_number>>spacing;
		vector<Polygon_point *> pointlist(polygon_number*2);
		for(i=0;i<polygon_number*2;i+=2)
		{
			file>>id>>x>>y>>width>>height;
			polygon_point *temp = new polygon_point();
			pointlist[i]=temp;
			set(pointlist[i],id,x,y,width,height,1);
			polygon_point *copy = new polygon_point();
			pointlist[i+1]=copy;
			set(pointlist[i+1],id,x,y,width,height,2);
		}
		make_heap(pointlist.begin(),pointlist.end(),compareY);
		sort_heap(pointlist.begin(),pointlist.end(),compareY);
		ycount=0;
		for(i=0;i<pointlist.size()-1;i++) //�ϬۦP��y�̷�x�Ѥp��j�Ʀn
		{
			n=i+1;
			while(n<pointlist.size()&&pointlist[i]->y==pointlist[n]->y)
				n++;
			if(n>i+1)
			{
				make_heap(pointlist.begin()+i,pointlist.begin()+n,compareX);
				sort_heap(pointlist.begin()+i,pointlist.begin()+n,compareX);
				i=n-1;
			}			
			ycount++;
		}
		if(n==(pointlist.size()-1))
			ycount++;
		Polygon_point **ylist = new Polygon_point*[ycount];
		x_index=set_listY(pointlist,ylist,ycount);
		violate = find_violate(ylist,x_index,spacing,ycount);
		make_heap(violate.begin(),violate.end(),compare);
		sort_heap(violate.begin(),violate.end(),compare);
		for(i=0;i<violate.size();i++)
		{
			if(violate[i]->first.p->gid!=NULL&&violate[i]->second.p->gid!=NULL)
			{
				if(violate[i]->first.p->gid->get_ancestor_address()!=violate[i]->second.p->gid->get_ancestor_address())
					cout<<"("<<violate[i]->first.p->id<<" "<<violate[i]->second.p->id<<")";
			}
			else
				cout<<"("<<violate[i]->first.p->id<<" "<<violate[i]->second.p->id<<")";
		}
		//delete
		for(i=ycount-1;i>-1;i--)
				delete ylist[i];
		delete x_index;
		pointlist.clear();
		violate.clear();
		cout<<endl;
		time--;
	}
	//unsigned long end = clock();
	//cout<<"total time: "<<(end-start)/1000.0<<" sec"<<endl;
	file.close();
	system("pause");
	return 0;
}

void set(Polygon_point *point,int id,int x,int y,int width,int height,int in_out)
{
	Polygon *temp = new Polygon(); 
	point->p=temp;
	point->p->id=id;
	point->p->x=x;
	point->p->y=y;
	point->p->width=width;
	point->p->height=height;
	point->p->gid=NULL;
	point->in_out=in_out;
	if(in_out==1) //���W�I
	{
		point->x=x;
		point->y=y;
	}
	else //�k�U�I
	{
		point->x=x+width;
		point->y=y-height;
	}
}

int* set_listY(vector<Polygon_point *> pointlist,Polygon_point **ylist,int ycount)
{
	int i,n=0,m=1,same=0,yline=0;
	int *x_index = new int[ycount];
	for(i=0;i<pointlist.size();i++)
	{
		for(n=i;n<pointlist.size();n++) //���Ҧ��ۦPy��point
		{
			if(pointlist[i]->y!=pointlist[n]->y)
				break;
		}
		Polygon_point *temp = new Polygon_point[n];
		same=0;
		for(m=i;m<n;m++)
		{
			temp[same]=*pointlist[m];
			same++;
		}
		ylist[yline]=temp;
		x_index[yline]=same;
		yline++;
		i=n-1; //����ۦPy����
	}
	return x_index;
}

vector<Pair *> find_violate(Polygon_point **ylist,int *x_index,int spacing,int ycount)
{
	int i,n,k,yline;
	vector<Pair *> violate;
	for(i=ycount-1;i>-1;i--)//��loop�P�U��loop ����ylist���k�U�}�l�@��@�橹�W�� ��Ҧ��k�U���I O(2N)
	{
		for(n=x_index[i]-1;n>-1;n--)
		{
			if(ylist[i][n].in_out==2)//���k�U���I
			{
				k=n;
				yline=i;
				while(yline>-1&&ylist[yline][0].y<=(ylist[i][n].y+ylist[i][n].p->height)) //���P��polygon�����W�I��y����
				{
					if(ylist[yline][k].x<(ylist[i][n].p->x-ylist[i][n].p->width-spacing)&&ylist[i][n].p->y==ylist[yline][k].y)
						break;
					if(ylist[yline][k].in_out==2&&ylist[i][n].p->id!=ylist[yline][k].p->id)//�J��k�U���I
					{
						
						if((ylist[i][n].x-ylist[i][n].p->width)==ylist[yline][k].x) //�P��epolygon��������H
							make_group(&ylist[i][n],&ylist[yline][k]);
						if((ylist[i][n].y+ylist[i][n].p->height)==ylist[yline][k].y&&ylist[i][n].x>=(ylist[yline][k].x-ylist[yline][k].p->width)&&ylist[yline][k].x>=(ylist[i][n].x-ylist[i][n].p->width)) //�P��epolygon���W����H
							make_group(&ylist[i][n],&ylist[yline][k]);
						if(ylist[i][n].x-ylist[i][n].p->width>ylist[yline][k].x&&(ylist[i][n].x-ylist[i][n].p->width-ylist[yline][k].x)<=spacing) //�b�d�򤧤��B�b����
						{
							if((ylist[i][n].x-ylist[i][n].p->width-ylist[yline][k].x)>0)//narrow
							{
								if(ylist[i][n].p->id<ylist[yline][k].p->id)
									set_pair(&violate,ylist[i][n],ylist[yline][k]);
								else
									set_pair(&violate,ylist[yline][k],ylist[i][n]);
							}
						}
						else if(ylist[i][n].x<(ylist[yline][k].x-ylist[yline][k].p->width)&&(ylist[yline][k].x-ylist[yline][k].p->width)-ylist[i][n].x<=spacing) //�d�򤧤��B�b�k��
						{
							if((ylist[yline][k].x-ylist[i][n].p->width-ylist[i][n].x)>0)//narrow
							{
								if(ylist[i][n].p->id<ylist[yline][k].p->id)
									set_pair(&violate,ylist[i][n],ylist[yline][k]);
								else
									set_pair(&violate,ylist[yline][k],ylist[i][n]);
							}
						}
						else //�W�X�d��
							k=0;
					}
					if(ylist[yline][k].in_out==1&&ylist[i][n].p->id!=ylist[yline][k].p->id)
					{
						if(ylist[i][n].x==ylist[yline][k].x) //�P��epolygon���k����H
							make_group(&ylist[i][n],&ylist[yline][k]);
						if(ylist[i][n].y==ylist[yline][k].y&&ylist[i][n].x>=ylist[yline][k].x&&(ylist[yline][k].x+ylist[yline][k].p->width)>=(ylist[i][n].x-ylist[i][n].p->width)) //�P��epolygon���U����H
							make_group(&ylist[i][n],&ylist[yline][k]);
					}
					k--;
					if(k==-1)//ylist���W
					{
						yline--;
						if(yline==-1)//��ylist����M�F
							break;
						k=set_near(ylist[yline],ylist[i][n],x_index[yline],spacing);
					}
				}
			}
		}
	}
	return violate;
}

int set_near(Polygon_point * curlist,Polygon_point cur_point,int cur_index,int spacing)
{
	int n=cur_index-1;
	int i=n/2,m=0;
	int inrange=0;
	while(m<n)
	{
		if(curlist[i].x==(cur_point.x+spacing))
		{
			inrange=1;
			break;
		}
		if(curlist[i].x>(cur_point.x+spacing))
		{
			n=i-1;
			i=(m+n)/2;
		}
		if(curlist[i].x<(cur_point.x+spacing))
		{
			m=i+1;
			i=(m+n)/2;
		}		
	}
	while(i!=cur_index-1&&inrange==1) //���̥k�䪺�۵�
	{
		if(curlist[i].x==curlist[i+1].x)
			i++;
		else
			break;
	}
	return i;
}

void set_pair(vector<Pair *> *violate,Polygon_point first,Polygon_point second)
{
	Pair *temp = new Pair();
	temp->first=first;
	temp->second=second;
	violate->push_back(temp);
}

void make_group(Polygon_point *current_point,Polygon_point *up_point) 
{
	if(current_point->p->gid!=NULL&&up_point->p->gid!=NULL)//���group�X��
	{
		group *g = new group();
		current_point->p->gid->set_ancestor(g);
		current_point->p->gid->set_ancestor(g);
	}
	if(current_point->p->gid!=NULL&&up_point->p->gid==NULL)
		up_point->p->gid=current_point->p->gid;
	if(current_point->p->gid==NULL&&up_point->p->gid!=NULL)
		current_point->p->gid=up_point->p->gid;
	if(current_point->p->gid==NULL&&up_point->p->gid==NULL)
	{
		group *g = new group();
		g->set_father(g);
		current_point->p->gid=g;
		up_point->p->gid=g;
	}
}