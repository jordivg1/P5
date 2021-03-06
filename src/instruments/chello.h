#ifndef INSTRUMENT_CHELLO
#define INSTRUMENT_CHELLO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class Instrumentchello: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index,fs;
	float A;
//    std::string nombre_del_fichero = "";
    std::vector<float> tbl;
    float f0, increment,phas;
  public:
    Instrumentchello(const std::string &param = "");
//    std::string nombre_del_fichero = "";
    void command(long cmd, long note, long velocity=100); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
