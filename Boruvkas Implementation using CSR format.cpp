/*
Parallel Variant of Boruvka’s Algorithm

1)  Find the minimum weighted edge for each vertex and store it in vert_minedge array.
2)  Remove mirrored edges i.e. if there is an edge ab and also an edge ba then remove ab as b>a.
3)  Initialize each vertex with its successor’s id. If there is no successor for a vertex then initialize it with itself.
4)  Give a unique color to for each distinct successor id.
5)  Create a flag array by marking the vertex 1 whose successor vertex is itself as it marks a unique segment and mark other vertex as 0.
6)  Create an array exclusive_prefox_sum corresponding to flag starting with the first vertex as 0 and adding corresponding flag index to it for next entry. This provides the unique id for each segment.
7)  Now we have distinct segment ids.
8)  Create an array representing outdegree for each segment.
9)  Create an array to find the first edge for each segment using exclusive prefix sum.
10) This first edge is used to keep track of where to insert new edge.
11) Now insert new edges using new vertex ids.

Advantages:
•	Implementation does not need to change the weight of edges if the graph is large.
•	Implementation done using CSR format better than adjacency list or matrix representation for GPU.
•	No need to create supervertex for creating a segment. Propagating colors for each segment make the task easy for creating segments.

*/
#include<iostream>
#include<stdio.h>
using namespace std;
void mst(int dest[],int weight[],int first_edge[],int outdegree[],int vertex,int edges){

		int j=0;
		int min_weight[vertex];
		for(int i=0;i<vertex;i++){
			min_weight[i] = 100;
		}
		int min_vertex[vertex];
		for(int j=0;j<vertex;j++){
			int a = first_edge[j];
			int b = first_edge[j]+outdegree[j];
			for(int i=a;i<b;i++){
				if(weight[i]<min_weight[j]){
					min_weight[j] = weight[i];
					min_vertex[j] = dest[i];
				}
			}
		}
		cout<<"Minimum Edges"<<endl;
		for(int i=0;i<vertex;i++){
			cout<<i+1<<"\t";
		}
		cout<<endl;
		for(int i=0;i<vertex;i++){
			cout<<min_weight[i]<<"\t";
		}
		cout<<endl;
		for(int i=0;i<vertex;i++){
			cout<<min_vertex[i]<<"\t";
		}
		for(int i=0;i<vertex;i++){
			for(int j=i+1;j<vertex;j++){
				if(min_vertex[i]==j+1&&min_vertex[j]==i+1){
					min_vertex[i] = i+1;
				}
			}
		}
		cout<<endl;
		cout<<"Removing duplicates"<<endl;
		for(int i=0;i<vertex;i++){
			cout<<min_vertex[i]<<"\t";
		}
		cout<<endl;
		
		int succesor[vertex];
		for(int i=0;i<vertex;i++)
        {
		succesor[i]=min_vertex[min_vertex[i]-1];
		}
		cout<<"SUCCESSOR ARRAY\n";
		for(int i=0;i<vertex;i++)
		{
			cout<<succesor[i]<<"\t";
			
		}
		cout<<endl;
		
		int flag[vertex];
		for(int i=0;i<vertex;i++){
			flag[i] = 0;
		}
		for(int i=0;i<vertex;i++){
			flag[succesor[i]-1] = 1;
		}
		cout<<"Flag"<<endl;
		for(int i=0;i<vertex;i++){
			cout<<flag[i]<<"\t";
		}
		cout<<endl;
		int exclusive_prefix_sum[vertex];
		exclusive_prefix_sum[0] = 0;
		for(int i=1;i<vertex;i++){
			exclusive_prefix_sum[i] = exclusive_prefix_sum[i-1]+flag[i-1];
		}
		cout<<"Exclusive Prefix Sum"<<endl;
		for(int i=0;i<vertex;i++){
			cout<<exclusive_prefix_sum[i]<<"\t";
		}
		cout<<endl;
		int new_vertex[10];
		j=0;
		for(int i=0;i<vertex;i++){
			if(flag[i]==1){
				new_vertex[j] = exclusive_prefix_sum[i];
				j++;
			}
		}
		int new_ver = j;
		cout<<"New Vertices"<<endl;
		for(int i=0;i<new_ver;i++){
			cout<<new_vertex[i]<<"\t";
		}
		cout<<endl;
		int new_outdegree[j];
		for(int i=0;i<new_ver;i++){
			new_outdegree[i] = 0;
		}
		int p=0;
		int new_dest[edges];
		int new_edges = 0;
		int new_weight[edges] ;
		int dest_supervertex[edges];
		for(int i=0;i<vertex;i++){
			if(flag[i]==1){
				
				int index = i+1;
				for(int l=0;l<vertex;l++){
					if(succesor[l]==index){
						int a = first_edge[l];
						int b = first_edge[l]+outdegree[l];
						for(int k=a;k<b;k++){
							if(index==succesor[dest[k]-1]){
								continue;
							}
							new_outdegree[p]++;
							new_dest[new_edges] = dest[k];
							dest_supervertex[new_edges] = succesor[new_dest[new_edges]-1];
							new_weight[new_edges] = weight[k];
							new_edges++;
						}
					}
				}
				p++;
			}
		}
		int k=1,count=0;
		for(int i=0;i<vertex;i++){
			for(int j=0;j<new_edges;j++){
				if(dest_supervertex[j]==i){
					dest_supervertex[j] = k;
					count++;
				}
			}
			if(count>0){
				count=0;
				k++;
			}
		}
		cout<<"New Outdegree"<<endl;
		for(int i=0;i<new_ver;i++){
			cout<<new_outdegree[i]<<"\t";
		}
		cout<<endl;
		if(new_ver==1)
		return;
		int new_first_edge[new_ver];	
		new_first_edge[0] = 0;
		for(int i=1;i<new_ver;i++){
			new_first_edge[i] = new_first_edge[i-1] + new_outdegree[i-1];
		}
		cout<<"New First Edge"<<endl;
		for(int i=0;i<new_ver;i++){
			cout<<new_first_edge[i]<<"\t";
		}
		cout<<endl;
		cout<<"New Destinations"<<endl;
		for(int i=0;i<new_edges;i++){
			cout<<new_dest[i]<<"\t";
		}
		cout<<endl;
		cout<<"Dest Supervertex"<<endl;
		for(int i=0;i<new_edges;i++){
			cout<<dest_supervertex[i]<<"\t";
		}
		cout<<endl;
		cout<<"New Weight"<<endl;
		for(int i=0;i<new_edges;i++){
			cout<<new_weight[i]<<"\t";
		}
		cout<<endl;
		mst(dest_supervertex,new_weight,new_first_edge,new_outdegree,new_ver,new_edges);
}
int main(){

	/*	Input with the graph --> 
	FILE *in_file;
	char filename[] = "graph.txt";
	in_file = fopen(filename ,"r");
    if (in_file == NULL)
    {
      fprintf(stderr, "Error: Unable to open file %s\n\n", filename);
    }
    int vertices,edges;
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
		 ch = getc(in_file);	
    }
    fclose(in_file);
    printf("fine");
	int source[edges],weight[edges],dest[edges],first_edge[vertices],outdegree[vertices];
	k=0;
	for(int i=0;i<edges;i++)
	{
		source[i] = a[k];
		k=k+3;
	
	}
	k=2;
	for(int i=0;i<edges;i++)
	{
		weight[i] = a[k];
		k=k+3;
	}
	k=1;
	for(int i=0;i<edges;i++)
	{
		dest[i] = a[k];
		k=k+3;
	}
	for(int i=0;i<edges;i++)
	{
		for(int j=0;j<edges-i-1;j++)
		{
			if(source[j]>source[j+1])
			{
				int temp[3];
				temp[0] = source[j];
				temp[1] = dest[j];
				temp[2] = weight[j];
				source[j] = source[j+1];
				dest[j] = dest[j+1];
				weight[j] = weight[j+1];
				source[j+1] = temp[0];
				dest[j+1] = temp[1];
				weight[j+1] = temp[2];
			}
		}
	}
	int j=0;
	first_edge[0] = 0;
	for(int i=0;i<edges;i++)
	{
		int count=0;
		while(source[i]==source[i+1])
		{
			count++;
			i++;
		}
		outdegree[j] = count+1;
		first_edge[j+1] = first_edge[j] + outdegree[j];
		j++;
	} 
	mst(dest,weight,first_edge,outdegree,vertices,edges);
	<--- */
		int weight[24] = {4,9,12,
					4,7,8,
					2,5,8,
					3,6,12,
					3,9,7,5,13,1,
					6,1,11,
					2,11,13};
	int dest[24] = {2,5,4,
					1,5,3,
					7,5,2,
					5,6,1,
					4,1,2,3,7,6,
					4,5,7,
					3,6,5
					};
	int first_edge[7] = {0,3,6,9,12,18,21};
	int outdegree[7] = {3,3,3,3,6,3,3};
	mst(dest,weight,first_edge,outdegree,7,24);
}
