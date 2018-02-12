#ifndef GADGETHELPER_H
# define GADGETHELPER_H

#include "BaseGadgets.h"

class GadgetHelper : public BaseGadgets {
 public:
  GadgetHelper(uint cookieValue, uint slideValue,
	       const char* fileName, uint maxSize);

 protected:
  // r0-6 = X
  void mov_r_x(uint reg, int x);
  // *(pc+X) = 0
  void str_0_sp_x(int x);
  // r0-2 = sp + X
  void mov_r_sp_x(uint reg, int x);
  // r4 = sp + X
  void mov_r4_sp_x(int x);  

 private:
  // r0 = x
  void mov_r0_x(int x);
  // r1 = x
  void mov_r1_x(int x);
  // r0-1 = 0
  void mov_r_0(uint reg, uint bytes = ADDR_SIZE);
};

#endif //GADGETHELPER_H

