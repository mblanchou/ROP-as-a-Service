#ifndef ROPSTRINGHELPER_H
# define ROPSTRINGHELPER_H

#include "GadgetHelper.h"

class ROPStringHelper : public GadgetHelper {
 public:
  ROPStringHelper(uint cookieValue, 
				  uint slideValue,
				  const char* fileName,
				  uint maxSize);

 protected:
  // store string on stack with r pointing to it
  void mov_r_str(uint reg, const char* str1,
				 BYTE setToBeginning = FALSE);

  void r0_str1__r1_str2(const char* str1, const char* str2,
						BYTE setToBeginning = FALSE);

 private:
  
  uint mov_r_str_helper(const char* str1,
						BYTE setToBeginning = FALSE);
  
 private:
  int _last_s_offset;
};

#endif //ROPSTRINGHELPER_H

