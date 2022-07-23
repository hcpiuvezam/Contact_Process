#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
	double lambda = 3.2978, // This value is the critical value for CP in 1D
			prob_creation,
			r,
		    t,
			t_max = 10000.;
	
	int L,
		trials,
		Nsurviving,
		x,
		y,
		s;
			
	vector<int> cells,
					surviving;
									 
	L = atoi(argv[1]); // Three numbers given by the user. (Size of the system)
	trials = atoi(argv[2]); // (Amount of "runs")
	lambda = atof(argv[3]); // (Value of control parameter)
	
    char name[100];

	ofstream outfile;
	
    sprintf(name,"CP_L_%d_lambda_%.3f.dat", L, lambda);

	outfile.open(name, ios::trunc);
	
	srand(time(NULL));//10263);
	
	prob_creation = lambda/(1+lambda);
		
    for(int j = 0; j<trials; j++){

        t = 0.;
    
        for(int i = 0; i<L; i++)
            cells.push_back(0); // all cells start "inactive".
        
        x = ((int) rand()%L);
        cells[x] = 1; // except for one.
        
        outfile << t << ' ' << 1 << '\n';

        while(t<=t_max){
            s = 0;
            x = ((int) rand()%L);
            
            if(cells[x] == 1){
                r = ((double) rand()/RAND_MAX);
                if(r<=prob_creation){
                    if(x == L-1)
                        y = L-2;
                    else if(x == 0)
                        y = 1;
                    else
                        y = (rand() > RAND_MAX/2) ? x-1 : x+1;
                    cells[y] = 1;
                }
                else
                    cells[x] = 0;
            }
            
            for(auto& n:cells)
            s += n;

            outfile << t << ' ' << s << '\n';

            if(s == 0)
                break;
                
            t+=((double) 1/L);
            
        }
            
        cells.clear();	
    }

	outfile.close();
	
	return(0);
}