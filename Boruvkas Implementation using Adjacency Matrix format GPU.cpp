#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include"cuda_runtime.h"
#include"device_launch_parameters.h"
struct node{
	int u, v, weight, idx;
};
__global__
void kernel_flag(node *deviceNodes,int* flag, int edges)
{
	int thread = blockDim.x*blockIdx.x + threadIdx.x;

	flag[0] = 1;
	if (thread <edges && thread>0)
	{
		if (deviceNodes[thread].u != deviceNodes[thread-1].u)
		{
			flag[thread] = 1;
		}
	}
}
void sort(node* n, int edges)
{
	for (int i = 0; i<edges; i++)
	{
		for (int j = 0; j<edges - i - 1; j++)
		{
			if (n[j].u>n[j + 1].u)
			{
				node temp;
				temp = n[j];
				n[j] = n[j + 1];
				n[j + 1] = temp;
			}
		}
	}
}
cudaError_t func(node *n, int vertices, int edges){
	cudaError_t cudaStatus;
	cudaStatus = cudaSetDevice(0);
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}*/
	int ed = edges;
	sort(n, edges);
	/*printf("Start Vertex\n");
	for (int i = 0; i < edges; i++)
	{
		printf("%d\t", n[i].u);
	}
	printf("\n");
	printf("Weight\n");
	for (int i = 0; i < edges; i++)
	{
		printf("%d\t", n[i].weight);
	}
	printf("\n");
	printf("Final Vertex\n");
	for (int i = 0; i < edges; i++)
	{
		printf("%d\t", n[i].v);
	}
	printf("\n");*/
	node *deviceNodes;
	node *deviceSucNodes;
	int *r_flag,i;
	int nodeSize = sizeof(node)*edges;
	int flagSize = sizeof(int)*edges;

	int *flag = (int*)malloc(flagSize);


	cudaStatus = cudaMalloc(&r_flag, flagSize);
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "Flag cudaMalloc failed!");
		goto Error;
	}*/
	cudaStatus = cudaMalloc(&deviceNodes, nodeSize);
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "Device Node cudaMalloc failed!");
		goto Error;
	}*/

	cudaStatus = cudaMemcpy(deviceNodes, n, nodeSize, cudaMemcpyHostToDevice);
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}
*/
	kernel_flag <<<ceil(edges/512.0), 512 >>>(deviceNodes,r_flag, edges);
	cudaStatus = cudaGetLastError();
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "Flag Kernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}*/
	cudaStatus = cudaDeviceSynchronize();
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}*/
	cudaStatus = cudaMemcpy(flag, r_flag, flagSize, cudaMemcpyDeviceToHost);
	/*if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}
*/
	int j = 0;
	int sucSize = sizeof(node)*vertices;
	node* s = (node*)malloc(sucSize);

	for (int i = 0; i<vertices; i++)
	{
		if (flag[j] == 1)
		{
			node min;
			min = n[j];

			if (j < edges - 1)
			{
				while (flag[++j] != 1)
				{
					if (n[j].weight<min.weight)
						min = n[j];
					if (j == edges - 1)
						break;
				}
			}
			s[i] = min;
		}
	}

	for (int i = 0; i<vertices; i++)
	{
		for (int j = i + 1; j<vertices; j++)
		{
			if (s[j].u == s[i].v&&s[j].v == s[i].u)
			{
				s[i].weight = 0;
				s[i].v = s[i].u;
			}
		}
	}


	for (int i = 0; i < vertices; i++)
		s[i].idx = 0;
	int c = 1;
	for (int i = 0; i<vertices; i++){
		if (s[i].u == s[i].v)
			continue;
		else if (s[i].idx == 0){
			s[i].idx = c++;

			int *arr = (int*)malloc(sizeof(int)*vertices);
			int index = 0;
			arr[index++] = s[i].u;
			arr[index++] = s[i].v;
			int x = 0;
			int j = 0;
			while (x!=index){
				if (arr[x] == s[j].u || arr[x] == s[j].v){
					s[j].idx = s[i].idx;
					int k, l;
					for (k = 0; k < index; k++){
						if (s[j].u == arr[k]){
							break;
						}
					}
					if (k == index){
						arr[index++] = s[j].u;
					}
					for (l = 0; l < index; l++){
						if (s[j].v == arr[l]){
							break;
						}
					}
					if (l == index){
						arr[index++] = s[j].v;
					}
				}
				j++;
				if (j == vertices){
					j = 0;
					x++;
				}
			}
		}

	}
	if (c == 2)
		return cudaStatus;
	int new_vert = 0;
	int new_edge = 0;

	//for (int i = 0; i < vertices; i++)
	//	printf("%d ", s[i].u);
	//printf("\n");
	//for (int i = 0; i < vertices; i++)
	//	printf("%d ", s[i].weight);
	//printf("\n");
	//for (int i = 0; i < vertices; i++)
	//	printf("%d ", s[i].v);
	//printf("\n");

	node *wi;
	wi = (node*)malloc(nodeSize);
	//if (wi == NULL)
	//{
	//	printf("Memory not Assigned\n");
	//}
	int count = 0;
	for (int i = 0; i<ed; i++)
	{

		int start = 0, end = 0;
		for (int j = 0; j<vertices; j++)
		{
			if (n[i].u == s[j].u)
			{
				start = s[j].idx;
			}
		}
		for (int j = 0; j<vertices; j++)
		{
			if (n[i].v == s[j].u)
			{
				end = s[j].idx;
			}
		}
		if (start != end)
		{
			wi[count].u = start;
			wi[count].weight = n[i].weight;
			wi[count].v = end;
			count++;
		}

	}
	
	new_vert = s[0].idx;
	for (int i = 0; i<vertices - 1; i++){
		if (s[i].idx>new_vert)
			new_vert = s[i].idx;
	}
	new_edge = count;

	//cudaStatus = cudaDeviceReset();
	//if (cudaStatus != cudaSuccess) {
	//	fprintf(stderr, "cudaDeviceReset failed!");
	//}
	func(wi, new_vert, new_edge);//recursion function	

Error:
	cudaFree(deviceNodes);
	cudaFree(r_flag);
	return cudaStatus;
Error2:
	cudaFree(deviceSucNodes);
	cudaFree(deviceNodes);
	cudaFree(r_flag);
	return cudaStatus;
}
int main()//MAIN Function
{

	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;
	double elapsedTime;
	QueryPerformanceFrequency(&frequency);


	int vertices, edges;
		/*int a[72] = {1,2,4,1,4,12,1,5,9,2,1,4,2,3,8,2,5,7,3,2,8,3,7,2,3,5,5,4,1,12,4,6,6,4,5,3,5,1,9,5,2,7,5,3,5,5,7,13,5,6,1,5,4,3,6,4,6,6,7,11,6,5,1,7,3,2,7,5,13,7,6,11};
		vertices = 7;
		edges = 24;*/

	FILE *in_file;
	char filename[] = "graph3.txt";
	in_file = fopen(filename, "r");
	if (in_file == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n\n", filename);
	}

	fscanf(in_file, "%d", &vertices);
	fscanf(in_file, "%d", &edges);
	printf("%d %d\n", vertices, edges);

	int *a;
	a = (int*)malloc(sizeof(int) * 3 * edges);
	int k = 0, ch_int;
	while (getc(in_file) != '\n');
	for (int i = 0; i<edges; i++){
		char ch = getc(in_file);
		for (int j = 0; j<3; j++)
		{
			fscanf(in_file, "%d", &ch_int);
			a[k] = ch_int;
			k++;
		}
		ch = getc(in_file);
	}
	fclose(in_file);

	int size = sizeof(node)*edges;
	node* n;

	n = (node*)malloc(size);

	for (int i = 0; i<edges; i++)
	{
		n[i].u = a[3*i];
		n[i].v = a[3*i + 1];
		n[i].weight = a[3*i + 2];
		n[i].idx = i;
	}

	QueryPerformanceCounter(&t1);
	cudaError_t cudaStatus = func(n,vertices, edges);
	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart)*1000.0 / frequency.QuadPart;
	printf("%f GPU Time\n", elapsedTime);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "function failed!");
		getch();
		return 1;
	}
	printf("Done");
	getch();
	return 0;
}
