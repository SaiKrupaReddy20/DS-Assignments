#include<iostream>
#include<bits/stdc++.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
using namespace std;

typedef
struct each_process {
	int num;
	char* prefix;
	int start_index;
	int end_index;
	}each_process;

int calculated_sum = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* process_file(void* process)
{
	each_process* pro = (each_process*) process;
    int num = pro->num;
	char* prefix = pro->prefix;
	int start_index = pro->start_index;
	int end_index = pro->end_index;		
	//cout<<prefix<<start_index<<end_index<<endl;
    int sum = 0;
	int file_number = start_index/num;
	char filename[10];
    snprintf(filename, 10, "%s%d.txt", prefix, file_number);
	//cout<<filename<<endl;
    FILE* fp = fopen(filename, "r");
    fseek(fp, 4*(start_index % num) , SEEK_SET);
	int total_ele = end_index-start_index+1;
	int number;
	bool open_file = true;
	while(total_ele > 0)
	{	
		number = getw(fp);
		sum += number;
		total_ele--;
		start_index++;
		if(start_index/num != file_number)
		{
			file_number = start_index/num;
			fclose(fp);
			open_file = false;
			if(total_ele > 0)
			{
				snprintf(filename, 10, "%s%d.txt", prefix, file_number);
				fp = fopen(filename, "r");
				open_file = true;
			}
		}
	}
	if(open_file)
		fclose(fp);
	
	pthread_mutex_lock( &mutex1 );
    calculated_sum += sum;
    pthread_mutex_unlock( &mutex1 );

    delete((each_process*) process);
    return (void*) 0;
}

int main(int argc, char* argv[])
{
	int a_files = atoi(argv[1]);
	int b_files = atoi(argv[2]);
	int num = atoi(argv[3]);
	int num_of_parent_threads = atoi(argv[4]);
	int num_of_child_threads = atoi(argv[5]);
	int actual_sum = 0;
	for(int i=0; i<a_files; i++)
	{
		char filename[10];
        snprintf(filename, 10, "a_%d.txt", i);
		FILE* fp;
		fp = fopen(filename, "w");
		srand(i);
		for(int j=0; j<num; j++)
		{
			int number = rand()%100;
			putw(number, fp);
			actual_sum += number;
		}
		fclose(fp);
	}
	
	for(int i=0; i<b_files; i++)
	{
		char filename[10];
        snprintf(filename, 10, "b_%d.txt", i);
		FILE* fp;
		fp = fopen(filename, "w");
		srand(i);
		for(int j=0; j<num; j++)
		{
			int number = rand()%100;
			putw(number, fp);
			actual_sum += number;
		}
		fclose(fp);
	}
	//cout<<actual_sum<<endl;
	
	pid_t pid;
	int pipedata[2];
	pipe(pipedata);
	
	pid = fork();
	if(pid == 0)
	{
		close(pipedata[0]);
		char prefix[] = "b_";
		pthread_t* child_t = new pthread_t[num_of_child_threads];
		int total_elements = num * b_files;
		int min_elem_in_thread = total_elements/num_of_child_threads;
		int freq = total_elements % num_of_child_threads;
		int counter = 0;
		int val_count = 0;
		for(int i = 0; i < num_of_child_threads; i++){
            each_process* pro = new each_process ;
			if(counter < freq)
			{
				pro->num = num;
				pro->prefix = prefix;
				pro->start_index = val_count;
				val_count += min_elem_in_thread;
				pro->end_index = val_count;
				val_count++;
				counter++;
			}
			else
			{
				pro->num = num;
				pro->prefix = prefix;
				pro->start_index = val_count;
				val_count += min_elem_in_thread-1;
				pro->end_index = val_count;
				val_count++;
			}
			
			//cout<<"fgh:"<<endl;
            pthread_create(&child_t[i], NULL, &process_file, (void*) pro);
        }
		//cout<<"fgh:"<<endl;
        for(int i = 0; i < num_of_child_threads; i++){
            pthread_join(child_t[i], NULL);
        }
		//cout<<"fgh:"<<endl;
        delete(child_t);
		write(pipedata[1], &calculated_sum, sizeof(calculated_sum));
		close(pipedata[1]);
		//cout<<"fgh:"<<endl;
	}	
	else{
		close(pipedata[1]);
		char prefix[] = "a_";
		pthread_t* parent_t = new pthread_t[num_of_parent_threads];
		int total_elements = num * a_files;
		int min_elem_in_thread = total_elements/num_of_parent_threads;
		int freq = total_elements % num_of_parent_threads;
		int counter = 0;
		int val_count = 0;
		for(int i = 0; i < num_of_parent_threads; i++){
            each_process* pro = new each_process ;
			if(counter < freq)
			{
				pro->num = num;
				pro->prefix = prefix;
				pro->start_index = val_count;
				val_count += min_elem_in_thread;
				pro->end_index = val_count;
				val_count++;
				counter++;
			}
			else
			{
				pro->num = num;
				pro->prefix = prefix;
				pro->start_index = val_count;
				val_count += min_elem_in_thread-1;
				pro->end_index = val_count;
				val_count++;
			}
            pthread_create(&parent_t[i], NULL, &process_file, (void*) pro);
        }
        for(int i = 0; i < num_of_parent_threads; i++){
            pthread_join(parent_t[i], NULL);
        }
        delete(parent_t);
		wait(NULL);
		int child_process_sum;
        read(pipedata[0], &child_process_sum, sizeof(child_process_sum));
		close(pipedata[0]);
		int parent_process_sum = calculated_sum;
		int total_calculated_sum = parent_process_sum + child_process_sum;
		cout<<"Actual Sum:"<<actual_sum<<endl;
		cout<<"Total Calculated Sum:"<<total_calculated_sum<<endl;
		if(total_calculated_sum == actual_sum)
			cout<<"SUCCESS"<<endl;
		else
			cout<<"FAILURE"<<endl;
	}
}
