#include"Phase2.h"

int main()
{
	readSudoku("project.txt");
	SWAP=false;
	
	Phase1();
	while(!Quit)
	{
		
		cout<<"\n\tPhase 2A Processing";
		printSudoku();
		Phase3();
		Phase2A();
		Phase3();
		Phase2B();

		cout<<"\n\tPhase 2B Processing";
		
		if(SWAP==true)
		SWAP=false;
		else
		SWAP=true;
	}

	printVectors();
	cout<<"\n\t***Final***";
	printSudoku();

	cout<<"\n\tMAIN ENDED SUCCESSFULLY\n";
	pthread_exit(NULL);
	return 0;
	
}
