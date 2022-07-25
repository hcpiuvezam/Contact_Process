#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h> 
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]){
	double lambda = 3.2978,
			prob_creation,
			rho_mold,
			rho_devaux,
			rho_mnew,
			Ns_inv,
			r,
			t,
			t_max;
	
	int L,
		trials,
		Ns = 0,
		x,
		y,
		s,
		count,
		v;
			
	std::vector<int> cells,
					rho_s;
	std::vector<double> rho_m,
						rho_dev;
	char name [100];
									 
	L = atoi(argv[1]);
	trials = atoi(argv[2]);
    //lambda = atof(argv[3]);

    /* In case one wants to test this outside the critical value: 
        uncomment the line above. */
	
    switch(L){
	case 20:
		t_max = 500.;
        break;
	case 50:
		t_max = 1000.;
        break;
	case 100:
		t_max = 3000.;
        break;
	case 200:
		t_max = 6000.;
        break;
	case 500:
		t_max = 10000.;
        break;
	default:
		t_max = 20000.;
        break;
    }

	std::ofstream outfile;
	
	sprintf(name,"CP_density_decay_L_%d_lambda_%.3f.dat", L, lambda);

	outfile.open(name, ios::trunc);
	
	srand(time(NULL));
	
	prob_creation = lambda/(1+lambda);
	
	while(Ns<=trials){
	
		t = 0.;
	
		for(int i = 0; i<L; i++)
			cells.push_back(1); // All sites start active
		
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
			
			for(std::size_t n=0; n<cells.size(); ++n)//(auto n : cells)
    			s += cells[n];
    		
    	rho_s.push_back(s);
    		
			if(s == 0)
				break;
					
			t+=((double) 1/L);
				
		}
			
		if(s!=0){ // Only count surviving runs
			if(Ns == 0){
				Ns++;
				for(std::size_t n = 0; n < rho_s.size(); ++n){//for(auto& rho:rho_s){
					rho_m.push_back(((double)rho_s[n]/L));
					rho_dev.push_back(0);
				}
			}
			else{
				Ns++;
				Ns_inv = ((double)1/(Ns+1));
				v = 0;
				for(std::size_t n = 0; n < rho_s.size(); ++n){//for(auto& rho:rho_s){
					rho_mold = rho_m[v];
					rho_mnew = rho_m[v];
					rho_mnew = Ns_inv*((double)rho_s[n]/L)+(1-Ns_inv)*rho_mnew;
					rho_m[v] = rho_mnew;
					rho_devaux = rho_devaux;
					rho_dev[v] = rho_devaux + (((double)rho_s[n]/L)-rho_mnew)*(((double)rho_s[n]/L)-rho_mold);
					v++;
				}
			}
		}
		rho_s.clear();
		cells.clear();
	}
	v = 0;
	for(std::size_t n = 0; n < rho_m.size(); ++n){//for(auto& rho_mean:rho_m){
		outfile << rho_m[n] << " " << rho_dev[v] << "\n";
		v++;
	}
	
	outfile.close();
	
	return(0);
}