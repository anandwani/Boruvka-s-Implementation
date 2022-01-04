/*
Algorithm ---> 

First implementation of boruvka’s

1)  Create a list X containing weight w and outgoing vertex v for each edge in graph.
2)  Create a flag array, F, corresponding to edge list where 1 indicate the start of segment in edge list and 0 indicate the edge of the same segment.
3)	Perform segmented min scan on X. Find the minimum outgoing edge from each segment formed by flag array F and store it in array NWE.
4)	Find the successor of each vertex and add to successor array, S.
5)	Remove cycle making edges from NWE using S, and identify representatives vertices.
6)	Mark remaining edges from NWE as part of output in MST.
7)	Propagate representative vertex ids using pointer doubling i.e. represent all the vertices of the segment using the representative vertex.
8)	Append successor array’s entries with its index to form a list, L.
9)	Split L, create flag over split output and scan the flag to find new ids per vertex, store new ids in C.
10)	Find supervertex ids of u and v for each edge using C.
11) Remove edge from edge-list if u, v have same supervertex id.
12)	Remove duplicate edges using split over new u, v and w.
13) Create the new edge-list and weight list.
14)	Build the vertex list from the newly formed edge-list.
15) Call the MST Algorithm on the newly created graph until a single vertex remains.

<-- 
Disadvantages
•	Each time you need to assign specific space in list for storing vertex (22-24 bits) and weight (8-10 bits).
•	Limits the number of vertices and edge weights of input graph.
•	User need to change edge weight if the graph is large.

*/

#include<iostream>
#include<stdio.h>
using namespace std;
class node{
	public :
		int u,v;
		int weight;
		int idx;

};
int counting;
int mx;
void sort(node* n[],int edges)
{
	for(int i=0;i<edges;i++)
	{
		for(int j=0;j<edges-i-1;j++)
		{
			if(n[j]->u>n[j+1]->u)
			{
				node* temp;
				temp = n[j];
				n[j] = n[j+1];
				n[j+1] = temp;
			}
		}
	}
}
void func(node *n[],int cnt,int vertices,int edges){
	int ed = edges;
	int j;
	sort(n,edges);
	int flag[ed],i=0;
	j=0;
	while(i<ed)
	{
		flag[i] = 1;
		i++;
		while(n[j]->u==n[j+1]->u)
		{
			flag[i] = 0;
            i++;
			j++;
			if(i==ed)
				break;
		}
		j++;
	}
	cout<<"Edge Description"<<endl<<endl;
	cout<<"Start Points"<<endl;
	for(int i=0;i<ed;i++)
	{
		cout<<n[i]->u<<"\t";
	}
	cout<<endl;
	cout<<"Weight"<<endl;
	for(int i=0;i<ed;i++)
	{
		cout<<n[i]->weight<<"\t";
	}
	cout<<endl;
	cout<<"End Points"<<endl;
	for(int i=0;i<ed;i++)
	{
		cout<<n[i]->v<<"\t";
	}
	cout<<endl;
	cout<<"Flag"<<endl;
	for(int i=0;i<ed;i++)
	{
		cout<<flag[i]<<"\t";
	}
	cout<<endl;
	j=0;
	node *s[vertices];
	for(int i=0;i<vertices;i++)
	{
		s[i] = new node();
	}

	for(int i=0;i<vertices;i++)
	{
		if(flag[j]==1)
		{
			node *min = new node();
			min = n[j];
			if(j!=edges-1)
			{
				while(flag[++j]!=1)
				{
					if(n[j]->weight<min->weight)
					{
						min = n[j];
					}
					if(j==edges-1)
						break;
				}
			}
			s[i] = min;
		}
	}

	cout<<"Successor Array"<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->u<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->weight<<"\t";
	}
	cout<<endl;
		for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->v<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		int u1 = s[i]->u;
		int v1 = s[i]->v;
		for(int j=i+1;j<vertices;j++)
		{
			if(s[j]->u==v1&&s[j]->v==u1)
			{
				s[i]->weight = 0;
				s[i]->v = u1;
			}
		}
	}
	cout<<"Removing Cycles..."<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->u<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->weight<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->v<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
		s[i]->idx = 0;
	int c = 1;
	for(int i=0;i<vertices;i++)
	{
		if(s[i]->u==s[i]->v)
			continue;
		else
		{
			if(s[i]->idx==0)
			{
				s[i]->idx = c++;
			}
			for(int j=0;j<vertices;j++)
				{
					if(s[j]->u==s[i]->v||s[j]->v==s[i]->v)
					{
						s[j]->idx = s[i]->idx;
					}
				}
		}
	}
	int new_vert=0;
	int new_edge=0;
	cout<<"Representative edge"<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->u<<"\t";
	}
 
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->weight<<"\t";
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->v<<"\t";
		
	}
	cout<<endl;
	for(int i=0;i<vertices;i++)
	{
		cout<<s[i]->idx<<"\t";
		
	}
	cout<<endl;		

	
	cnt++;
	

	if(c==2)
	{
		mx = cnt;
		return;
	}
	node *wi[ed];
	for(int i=0;i<ed;i++)
	{
		wi[i] = new node();
		
	
	}
	int count=0;
	for(int i=0;i<ed;i++)
	{
		int start,end;
		for(int j=0;j<vertices;j++)
		{
			if(n[i]->u==s[j]->u)
			{
				start = s[j]->idx;
			}
		}
		for(int j=0;j<vertices;j++)
		{
			if(n[i]->v==s[j]->u)
			{
				end = s[j]->idx;
			}
		}
		if(start!=end)
		{
			wi[count]->u = start;
			wi[count]->weight = n[i]->weight;
			wi[count]->v = end;
			count++;
		}
	}

	cout<<"New Edge list"<<endl;
	for(int i=0;i<count;i++)
	{
		cout<<wi[i]->u<<"\t";
		new_edge++;
	}
	cout<<endl;
	for(int i=0;i<count;i++)
	{
		cout<<wi[i]->weight<<"\t";
	}
	cout<<endl;
	for(int i=0;i<count;i++)
	{
		cout<<wi[i]->v<<"\t";
	}
	cout<<endl;
	new_vert = s[0]->idx;
	for(int i=0;i<vertices-1;i++){
		if(s[i]->idx>new_vert)
			new_vert = s[i]->idx;
	}
	/*
		**recursion function
	*/
	func(wi,cnt,new_vert,new_edge);		
}
int main()
{
	
	int vertices,edges;
	int a[72] = {1,2,4,1,4,12,1,5,9,2,1,4,2,3,8,2,5,7,3,2,8,3,7,2,3,5,5,4,1,12,4,6,6,4,5,3,5,1,9,5,2,7,5,3,5,5,7,13,5,6,1,5,4,3,6,4,6,6,7,11,6,5,1,7,3,2,7,5,13,7,6,11};
	vertices = 7;
	edges = 24;
	
	/* Reading from a file ---> 
	FILE *in_file;
	char filename[] = "graph.txt";
	in_file = fopen(filename ,"r");
    if (in_file == NULL)
    {
      fprintf(stderr, "Error: Unable to open file %s\n\n", filename);
    }
    
	fscanf(in_file,"%d", &vertices);
    fscanf(in_file,"%d", &edges);
    printf("%d %d\n",vertices,edges);
    static int a[27000];
    int k=0,ch_int;
    while(getc(in_file)!='\n');
    for(int i=0;i<edges;i++){
    	char ch = getc(in_file);
    	for (int j=0; j<3; j++)
		  	{
	        		fscanf(in_file,"%d", &ch_int);
	            	a[k] = ch_int;
					k++;		
		  	}
		 ch = getc(in_file);  	
    }
    fclose(in_file);
    <---- */
	
	node* n[edges];
	for(int i=0;i<edges;i++)
	{
		n[i] = new node();
	}	
	int j=0;
	for(int i=0;i<3*edges;i=i+3)
	{
		n[j]->u = a[i];
		n[j]->v = a[i+1];
		n[j]->weight = a[i+2];
		n[j]->idx = j;
		j++;
	}
	func(n,0,vertices,edges);
}
