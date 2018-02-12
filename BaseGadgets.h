#ifndef BASEGADGETS_H
# define BASEGADGETS_H

#include "ROPGen.h"

class BaseGadgets : public ROPGen {
public:
  BaseGadgets(uint cookieValue, uint slideValue,
			  const char* fileName, uint maxSize);

protected:
  // nop
  void nop();
  // call function with blx
  void callFunc(const char* symbol, 
				uint isThumb = TRUE);

  // pivot (sp = r)
  void pivot_r(uint reg);
  // to use with new payload after pivot
  void finish_pivot();

  // used to pivot
  void mov_sp_r4();  

  // pop r0-8
  void pop(uint reg);

  // mov
  void mov(uint reg1, uint reg2);

  // add
  void add_r4_sp_r4();
  // str
  void str_r1__r4();  

  // lsrs r0
  void lsrs_r0_r0_24();
  void lsrs_r0_r0_16();
  void lsrs_r0_r0_8();

  // lsrs r1
  void lsrs_r1_r1_8();
  void lsrs_r1_r1_24();
  void lsrs_r1_r1_28();
  // lsls r1
  void lsls_r1_r1_8();

  // lsrs/lsls generic 8
  void ls_r_8(char direction, uint reg);

  // sub sp
  void sub_sp();
};

#endif //BASEGADGETS_H
