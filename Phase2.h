#include"Phase1.h"


bool checkColConflict(int col,int number)// for checking duplicates when inserting new value in sudoku
{
	bool conflict=false;
	for(int i=0;i<9;i++)
	for(int k=0;k<9;k++)
	if(sudoku_array[i][col]==sudoku_array[k][col] && i!=k)
		conflict=true;
	return conflict;
}
bool checkRowConflict(int row,int number)// for checking duplicates when inserting new value in sudoku
{
	bool conflict=false;
	for(int i=0;i<9;i++)
	for(int k=0;k<9;k++)
	if(sudoku_array[row][i]==sudoku_array[row][k] && i!=k)
		conflict=true;
	return conflict;
}


void Phase3()
{


	
	thread_count=0;
	inDupInd.clear(); 
	inRowInd.clear(); 
	inColInd.clear(); 
	inVal.clear();
	inValRow.clear();   
	inValCol.clear();   
	Quit=1;

	for(int k=0;k<9;k++)
	for(int i=0;i<9;i++)
	if(sudoku_array[k][i]>9 ||sudoku_array[k][i]<1)
	{inValRow.push_back(k);inValCol.push_back(i);inVal.push_back(sudoku_array[k][i]);}

	for(int number=1;number<10;number++)
	{
	for(int row=0;row<9;row++)
	if(checkRowConflict(row,number)==true)
	{inRowInd.push_back(row);Quit=0;validEntries[row+9]=0;}
	
	for(int col=0;col<9;col++)
	if(checkColConflict(col,number)==true)
	{inColInd.push_back(col);Quit=0;validEntries[col+9+9]=0;}

	}
}


bool checkRowConflictPhase2(int row,int number)
{
    for(int i=0;i<9;i++)
    for(int j=0;j<9;j++)
    if(number==sudoku_array[row][i])
    return true;

    return false;
}
bool checkColConflictPhase2(int col,int number)
{
    //cout<<"\n\tcolumn conflict "<<col;
    for(int i=0;i<9;i++)
    if(number==sudoku_array[i][col])
    return true;

    return false;
}


void* clearInvalids(void *arg)//to clear invalids
{

        dimension *temp;
    temp=(dimension *)arg;
    int original[9]={1,2,3,4,5,6,7,8,9};

    pthread_mutex_lock(&m1);
    for(int i=0;i<9;i++)
    if(checkColConflictPhase2(temp->column,original[i])==false && checkRowConflictPhase2(temp->row,original[i])==false)
    sudoku_array[temp->row][temp->column]=original[i];

     pthread_mutex_unlock(&m1);
	pthread_exit(NULL);
}

void* clearRowDup(void *arg)
{
    dimension *temp;
    temp=(dimension *)arg;
    int original[9]={1,2,3,4,5,6,7,8,9};

    pthread_mutex_lock(&m1);

   
    
    for(int i=0;i<9;i++)
    for(int j=i+1;j<9;j++)
    {
        if(sudoku_array[temp->row][i]==sudoku_array[temp->row][j] && i!=j)
        {
        if(SWAP==false)    
        temp->column=j;
        else
        temp->column=i;
        
        for(int k=0;k<9;k++)
        if(checkColConflictPhase2(temp->column,original[k])==false)
        if(checkRowConflictPhase2(temp->row,original[k])==false)
        {sudoku_array[temp->row][temp->column]=original[k];break;}

        if(SWAP==true)    
        temp->column=j;
        else
        temp->column=i;
        for(int k=0;k<9;k++)
        if(checkRowConflictPhase2(temp->row,original[k])==false)
        {sudoku_array[temp->row][temp->column]=original[k];break;}

        }
    }


    pthread_mutex_unlock(&m1);
	pthread_exit(NULL);
}


void* clearColDup(void *arg)
{
    dimension *temp;
    temp=(dimension *)arg;
    int original[9]={1,2,3,4,5,6,7,8,9};

    pthread_mutex_lock(&m1);

   
    
    for(int i=0;i<9;i++)
    for(int j=i+1;j<9;j++)
    {
        if(sudoku_array[i][temp->column]==sudoku_array[j][temp->column] && i!=j)
        {
        if(SWAP==false)    
        temp->row=j;
        else
        temp->row=i;
    
        for(int k=0;k<9;k++)
        if(checkRowConflictPhase2(temp->row,original[k])==false)
        if(checkColConflictPhase2(temp->column,original[k])==false)
        {sudoku_array[temp->row][temp->column]=original[k];
        break;}

        if(SWAP==true)    
        temp->row=j;
        else
        temp->row=i;
        for(int k=0;k<9;k++)
        if(checkColConflictPhase2(temp->column,original[k])==false)
        {sudoku_array[temp->row][temp->column]=original[k];
        break;}
    

        }
    }


    pthread_mutex_unlock(&m1);
	pthread_exit(NULL);
}


void Phase2A()
{
    pthread_mutex_init(&m1,NULL);

    pthread_t invalidThread[inVal.size()];

    for(int i=0;i<inVal.size();i++)
    {
        dimension temp;
        temp.row=inValRow[i];
        temp.column=inValCol[i];
        pthread_create(&invalidThread[i],NULL,clearInvalids,&temp);
        cout<<"\nThreadID "<<invalidThread[i]<<" clearing invalid at "<<inValRow[i]<<" "<<inValCol[i];
        pthread_join(invalidThread[i],NULL);
    } 

    pthread_t RowThread[inRowInd.size()];
   for(int i=0;i<inRowInd.size();i++)
    {
        dimension temp;
        temp.row=inRowInd[i];
        pthread_create(&RowThread[i],NULL,clearRowDup,&temp);
        cout<<"\nThreadID "<<RowThread[i]<<" clearing duplicates at row "<<inRowInd[i];
        pthread_join(RowThread[i],NULL);
    } 
}

void Phase2B()
{
     pthread_t ColThread[inColInd.size()];
     for(int i=0;i<inColInd.size();i++)
    {
        dimension temp;
        temp.column=inColInd[i];
        pthread_create(&ColThread[i],NULL,clearColDup,&temp);
        cout<<"\nThreadID "<<ColThread[i]<<" clearing duplicates at col "<<inColInd[i];
        pthread_join(ColThread[i],NULL);
    }
}