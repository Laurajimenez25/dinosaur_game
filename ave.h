#ifndef Ave_h
#define Ave_h
#include "Arduino.h"

class Ave {
  private:    
    int x;         

  public:
    Ave(int posicionInicial);
    void mover();
    int obtenerPosicion();    
};

#endif
