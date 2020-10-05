#include"helper.h"

void Phase1()
{
    printSudoku(); // function to print sudoku
	pthread_mutex_init(&m1,NULL);
	pthread_mutex_init(&m2,NULL);
	pthread_mutex_init(&m3,NULL);
	pthread_t workers[27];
	int next=0;
	for (int m=0;m<9;m++)
	{
 		for (int n=0;n<9;n++)
		{
			// Column Checker //
			if (m==0)
			{ 
				dimension d1;
				d1.row=m;
				d1.column=n;
				pthread_create(&workers[thread_count],NULL,colChecker,&d1);
				pthread_join(workers[thread_count],NULL);
				thread_count++;
			}

			// Row Checker //
			if (n==0)
			{
				dimension d2;
				d2.row=m;
				d2.column=n;
				pthread_create(&workers[thread_count],NULL,rowChecker,&d2);
				pthread_join(workers[thread_count],NULL);
				thread_count++;
			}
			
			// 3x3 Grid Checker //
			if (m%3==0 && n%3==0)
			{
				dimension d3;
				d3.row=m;
				d3.column=n;
				pthread_create(&workers[thread_count],NULL,gridChecker,&d3);
				pthread_join(workers[thread_count],NULL);
				thread_count++;
			}
		}
	}

	int count=0;   // Used to check valid or invalid sudoku puzzle //
	int inDup=0;   // Used to display duplicates count //
	int inDig=0;   // Used to display invalid digits count //

	for (int v=0;v<27;v++)
	{
		if (validEntries[v]==1)
		{	
			count++;
		}	
	}

	if (inRowInd.size()!=0 || inValRow.size()!=0)
	{	
		cout<<endl;
		cout<<" --- Invalid Rows --- "<<endl<<endl;
	}	

	// Display Invalid Row Values //
	for (int a=0;a<inVal.size();a++)
	{
		cout<<"Invalid row = "<<inValRow[a]<<" Invalid value = "<<inVal[a]<<endl;
	}

	// Display Duplicates Row Values //
	for (int a2=0;a2<inRowInd.size();a2++)
	{
		cout<<"Invalid row = "<<inRowInd[a2]<<" Invalid value = "<<inDupInd[a2]<<endl;
	}

 	if (inColInd.size()!=0 || inValCol.size()!=0)
	{	
		cout<<endl;
		cout<<" --- Invalid Columns --- "<<endl<<endl;
	}

	// Display Invalid Column Values //
	for (int b=0;b<inVal.size();b++)
	{
		cout<<"Invalid column = "<<inValCol[b]<<" Invalid value = "<<inVal[b]<<endl;
	}

	// Display Duplicates Column Values //
	for (int b2=0;b2<inColInd.size();b2++)
	{
		cout<<"Invalid column = "<<inColInd[b2]<<" Invalid value = "<<inDupInd[b2]<<endl;
	}

	if (inValBox.size()!=0)
	{
		cout<<endl;
		cout<<" --- Invalid Boxes --- "<<endl<<endl;
	}	
	// Display Invalid Boxes //
	for (int a7=0;a7<inValBox.size();a7++)
	{
		cout<<"Invalid Box = "<<inValBox[a7]<<" Invalid Value = "<<inValBoxV[a7]<<endl;	
	}

	if (inThreads.size()!=0)
	{
		cout<<endl;
		cout<<" --- Thread IDs --- "<<endl<<endl;
	}
	// Display Thread IDs //
	for (int a6=0;a6<inThreads.size();a6++)
	{
		cout<<"Thread ID = "<<inThreads[a6]<<" Invalid Output = "<<inThreadsV[a6]<<endl;	
	}

	// Display Count of Invalid Entries //
	for (int a3=0;a3<inDupInd.size();a3++)
	{
		inDup++;
	}
	
	for (int a4=0;a4<inVal.size();a4++)
	{	
		inDig++;
	}

	cout<<endl;
	cout<<" --- Total number of Invalid Entries --- "<<endl<<endl;
	cout<<"Duplicates = "<<inDup<<endl;
	cout<<"Invalid Digits = "<<inDig<<endl;
	cout<<endl;

	
	if (count==27)
	{
		cout<<"\t***Sudoku valid***"<<endl;
	}
	else
	{
		cout<<"\t***Sudoku invalid***"<<endl;	
	}
	cout<<endl;

	cout<<"\n\tFinal Output Grid\n";
	for(int p=0;p<27;p++)
	{
		cout<<"\t"<<validEntries[p];
		if(p==8)cout<<"Boxes ";
		if(p==17)cout<<"Rows ";
		if(p==26)cout<<"Columns ";
		if (p==8 || p==17 || p==26)cout<<endl;
	}

}