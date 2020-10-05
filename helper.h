#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<vector>
#include<fstream>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
using namespace std;

bool Quit=0; //TO END recalling of Phase1() & Phase2()
int sudoku_array[9][9];
bool SWAP=0;

// Array used for Valid Entries //
int validEntries[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

pthread_mutex_t m1,m2,m3;
int thread_count=0;
vector<int>inVal;   // Invalid Values Vector //
vector<int>inValRow;   // Invalid Values Rows Vector //
vector<int>inValCol;   // Invalid Values Columns Vector //
vector<int>inDupInd;   // Invalid Duplicates Vector //
vector<int>inRowInd;   // Invalid Rows Vector //
vector<int>inColInd;   // Invalid Columns Vector //
vector<int>inValBox;   // Invalid Boxes Vector //
vector<int>inValBoxV;   // Invalid Box Values Vector //
vector<int>inThreads;   // Invalid Threads Vector //
vector<int>inThreadsV;   // Invalid Threads Values Vector //

struct dimension
{
	int row;
	int column;
};

// Function to check the rows of sudoku puzzle //
void *rowChecker(void *arg)
{
	pthread_mutex_lock(&m1);
	int breaker=0;
	int rowValid[9]={0,0,0,0,0,0,0,0,0};
	dimension *rowDim;
	rowDim=(dimension *)arg;
	int r=rowDim->row;
	int c=rowDim->column;
	for (int ch=0;ch<9;ch++)
	{
		if (sudoku_array[r][ch]>=1 && sudoku_array[r][ch]<=9)
		{
			for (int cv=0;cv<9;cv++)
			{
				if (sudoku_array[r][ch]==rowValid[cv])   // Duplicates Checker //
				{
					inRowInd.push_back(r);  
					inColInd.push_back(ch);
					inDupInd.push_back(sudoku_array[r][ch]);  
					inThreads.push_back(thread_count);
					inThreadsV.push_back(sudoku_array[r][ch]);
					breaker=1;
				}
			}
			rowValid[ch]=sudoku_array[r][ch];
		}
		else
		{
			inVal.push_back(sudoku_array[r][ch]);   // Invalid Values Checker //
			inValRow.push_back(r);
			inValCol.push_back(ch);
			inThreads.push_back(thread_count);
			inThreadsV.push_back(sudoku_array[r][ch]);
			breaker=1;
		}
	}

	if (breaker==1)
	{
		pthread_mutex_unlock(&m1);
  		pthread_cancel(pthread_self());
	}
	else
	{
		validEntries[9+r]=1;   // 9 to 17 Indexes for Rows //	
		pthread_mutex_unlock(&m1);
		pthread_exit(NULL);
	}
}

// Function to check the columns of sudoku puzzle //
void *colChecker(void *arg)
{
	pthread_mutex_lock(&m2);
	int breaker2=0;
	int colValid[9]={0,0,0,0,0,0,0,0,0};
	dimension *colDim;
	colDim=(dimension *)arg;
	int r2=colDim->row;
	int c2=colDim->column;
	for (int rh=0;rh<9;rh++)
	{
		if (sudoku_array[rh][c2]>=1 && sudoku_array[rh][c2]<=9)
		{
			for (int cv2=0;cv2<9;cv2++)
			{
				if (sudoku_array[rh][c2]==colValid[cv2])   // Duplicates Checker //
				{
					inRowInd.push_back(rh);
					inColInd.push_back(c2);
					inDupInd.push_back(sudoku_array[rh][c2]);
					inThreads.push_back(thread_count);
					inThreadsV.push_back(sudoku_array[rh][c2]);
					breaker2=1;
				}
			}
			colValid[rh]=sudoku_array[rh][c2];
		}
		else
		{
			breaker2=1;
		}
	}

	if (breaker2==1)
	{
		pthread_mutex_unlock(&m2);
  		pthread_cancel(pthread_self());
	}
	else
	{
		validEntries[18+c2]=1;    // 18 to 26 Indexes for Columns  //
		pthread_mutex_unlock(&m2);
		pthread_exit(NULL);
	}

}

// Function to check the 3x3 grid of sudoku puzzle //
void *gridChecker(void *arg)
{
	pthread_mutex_lock(&m3);
	int breaker3=0;
	int next=0;
	int gridValid[9]={0,0,0,0,0,0,0,0,0};
	dimension *gridDim;
	gridDim=(dimension *)arg;
	int r3=gridDim->row;
	int c3=gridDim->column;
	for (int gh=r3;gh<r3+3;gh++)
	{
		for (int dh=c3;dh<c3+3;dh++)
		{
			if (sudoku_array[gh][dh]>=1 && sudoku_array[gh][dh]<=9)
			{
				for (int cv3=0;cv3<9;cv3++)
				{
					if (sudoku_array[gh][dh]==gridValid[cv3])   // Duplicates Checker //
					{
						inValBox.push_back(r3+c3/3+1);
						inValBoxV.push_back(sudoku_array[gh][dh]);
						inThreads.push_back(thread_count);
						inThreadsV.push_back(sudoku_array[gh][dh]);
						breaker3=1;
					}
				}
				gridValid[next]=sudoku_array[gh][dh];
			}
			else
			{
				inValBox.push_back(r3+c3/3+1);
				inValBoxV.push_back(sudoku_array[gh][dh]);
				breaker3=1;
			}
			next++;
		}
	}

	if (breaker3==1)
	{
		pthread_mutex_unlock(&m3);
  		pthread_cancel(pthread_self());
	}
	else
	{
		validEntries[r3+c3/3]=1;    // 0 to 8 Indexes for 3x3 Grid //
		pthread_mutex_unlock(&m3);
		pthread_exit(NULL);
	}
}
void printSudoku()
{
	ofstream filer;
	filer.open("output.txt");
    cout<<"\n\tSUDOKU ARRAY\n";
	for(int i=0;i<9;i++)
	{for(int j=0;j<9;j++)
	{cout<<"  "<<sudoku_array[i][j];filer<<sudoku_array[i][j];filer<<' ';}
	cout<<endl;filer<<endl;}
	
}

void readSudoku(string file)
{
	ifstream filer;
	filer.open(file);
	string line;
	int array[9][9];
	int k=0;
	int j=0;
	while ( !filer.eof() )
    {
		filer>>line;
		int temp=0;
		for(int i=0;i<line.length();i++)
		{temp=temp*10;
		temp+=line[i]-48;}
		sudoku_array[k][j]=temp;
		line='\0';
		if(j==9)
		{k++;j=0;}
		j++;
	}
	filer.close();
}


void printVectors()
{
    for(int i=0;i<inVal.size();i++)
    cout<<"\nInvalid row "<<inValRow[i]<<" col "<<inValCol[i];

    for(int i=0;i<inRowInd.size();i++)
    cout<<"\nDup row "<<inRowInd[i];

    for(int i=0;i<inColInd.size();i++)
    cout<<"\nDup col "<<inColInd[i];

}

