//#include<mpi.h>

#include<iostream>
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;


int main(){
	int x,y,z;
	time_t start, end;
	cin>>x>>y>>z;
	vector<vector<int>> matrix1;
	for(int i=0; i<x; i++){
		vector<int> vec;
		for(int j=0; j<y; j++){
			int num;
			num = rand()%100;
			vec.push_back(num);
		}
		matrix1.push_back(vec);
	}
	
	vector<vector<int>> matrix2;
	for(int i=0; i<y; i++){
		vector<int> vec;
		for(int j=0; j<z; j++){
			int num;
			num = rand()%100;
			vec.push_back(num);
		}
		matrix2.push_back(vec);
	}
	
	vector<vector<int>> matrix3;
	for(int i=0; i<x; i++){
		vector<int> vec;
		for(int j=0; j<z; j++){
			vec.push_back(0);
		}
		matrix3.push_back(vec);
	}
	time(&start);
	for(int i=0; i<x; i++){
		for(int j=0; j<z; j++){
			for(int k=0; k<y; k++){
				matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	time(&end);
	cout<<"Diff - "<<difftime(end,start)<<endl;
	/*for(int i=0; i<x; i++){
		for(int j=0; j<z; j++){
			cout<<matrix3[i][j]<<" ";
		}
		cout<<endl;
	}*/
	time(&start);
	#pragma omp parallel for
	for(int i=0; i<x; i++){
		for(int j=0; j<z; j++){
			for(int k=0; k<y; k++){
				matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	time(&end);
	cout<<"Diff 2"<<difftime(end, start)<<endl;



}

