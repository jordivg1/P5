#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Instrumentseno::Instrumentseno(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  int N;
FILE * f = fopen("tblfile.log","a");

  if (!kv.to_int("N",N))
    N = 40; //default value

  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);;
    fprintf(f,"%f\n",tbl[i]);
    phase += step;
  }
fclose(f);
}


void Instrumentseno::command(long cmd, long note, long vel) {

f0 = 440*pow(2,(note - 69.)/12);
//fprintf(stdout,"f0-->%f\n",f0);
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index = 0;
	phas = 0;
//	increment = 10*(f0/(SamplingRate/tbl.size()));
//	increment = 2 * M_PI * (f0/SamplingRate);
	increment = ((f0 / SamplingRate) * tbl.size());
//	increment = SamplingRate / (f0) ;
//	fprintf(stdout,"increment-->%d\n",increment);
//	fprintf(stdout,"tblsize-->%d\n",tbl.size());
	A = vel / 127.;
	phas = 0;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & Instrumentseno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;
FILE * fp;
fp = fopen("xvector.log","a");
  for (unsigned int i=0; i<x.size(); ++i) {

  //fprintf(stdout,"index--->%d\n",index); 
// 	x[i] = A * sin(phas);
	phas = phas + increment;

	x[i] = A * tbl[round(phas)];


//Amb interpolació
//x[i] =tbl[floor(phas)]+(phas-floor(phas))*(tbl[floor(phas+1)]-tbl[floor(phas)])/(floor(phas+1)-floor(phas));

fprintf(fp,"%f\n",x[i]);
	 while(phas >= tbl.size()) phas = phas - tbl.size();

  }
  adsr(x); //apply envelope to x and update internal status of ADSR
fclose(fp);
  return x;
}
