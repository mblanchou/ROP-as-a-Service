#ifndef FCTCALLHELPER_H
# define FCTCALLHELPER_H

#include "ROPStringHelper.h"

class FctCallHelper : public ROPStringHelper
{
 public:
 FctCallHelper(uint cookieValue, uint slideValue,
	       const char* fileName, uint maxSize) : 
  ROPStringHelper(cookieValue, slideValue, fileName, maxSize) {
	
  }

  // use template functions
  // todo
  
  /* template<typename T1, typename T2, typename T3, typename T4>  */
  /* 	void FctCallHelper::genCall(T1 arg1, T2 arg2, T3 arg3, T4 arg4) { */
	
  /* }; */

  /* template<typename T1, typename T2,  */
  /* 	typename T3, typename T4>  */
  /* 	genCall(T1 arg1, T2 arg2,  */
  /* 		 T3 arg3, T4 arg4); */

  // overloads various types

 public:

  enum reg_t {R0, R1, R2, R3, R4, R5, R6, R7, R8};

  void call(const char* fctName)
  {
	callFunc(fctName);	
  }

  void call(const char* fctName,
			const char* arg1, int arg2)
  {
	mov_r_str(0, arg1);
	mov_r_x(1, arg2);
	call(fctName);
  }

  void call(const char* fctName,
			int arg1, const char* arg2)
  {
	mov_r_str(1, arg2);
	mov_r_x(0, arg1);
	call(fctName);
  }

  void call(const char* fctName,
			int arg1, const char* arg2, int arg3)
  {
	mov_r_str(1, arg2);

	mov_r_x(0, arg3);
	mov(2, 0);

	mov_r_x(0, arg1);

	call(fctName);
  }


  void call(const char* fctName,
			const char* arg1, const char* arg2)
  {
	r0_str1__r1_str2(arg1, arg2);
	call(fctName);
  }

  void call(const char* fctName,
			const char* arg1, const char* arg2, int arg3)
  {
	mov_r_x(0, arg3);
	mov(2, 0);

	r0_str1__r1_str2(arg1, arg2);

	call(fctName);
  }


  void call(const char* fctName,
			reg_t arg1)
  {
	mov(0, restoreValue(arg1));
	call(fctName);
  }

  void call(const char* fctName,
			int arg1)
  {
	mov_r_x(0, arg1);
	call(fctName);
  }

  void call(const char* fctName,
			int arg1, int arg2)
  {
	// mov_r1_x not fully implemented yet
	//mov_r_x(1, arg2);
	mov_r_x(0, arg2);
	mov(1, 0);
	mov_r_x(0, arg1);

	call(fctName);
  }

  void call(const char* fctName,
			int arg1, int arg2, int arg3)
  {
	// mov_r1_x not fully implemented yet
	//mov_r_x(1, arg2);
	mov_r_x(0, arg3);
	mov(2, 0);
	mov_r_x(0, arg2);
	mov(1, 0);
	mov_r_x(0, arg1);

	call(fctName);
  }

  void call(const char* fctName,
			reg_t arg1, int arg2, int arg3, reg_t arg4)
  {
	//set arg3
	mov_r_x(0, arg3);
	mov(2, 0);

	// set arg 2
	mov_r_x(1, arg2);

	// restore arg 1 and 4
	mov(3, restoreValue(arg4));
	mov(0, restoreValue(arg1));

	call(fctName);
  }

  reg_t saveRet() {
	// only save in r6 and r8 for now
	static uint last_reg_used = 0;
	
	if (!last_reg_used || last_reg_used == 6) {
	  mov(8, 0);
	  last_reg_used = 8;
	  return R8;
	}
	else if (last_reg_used == 8) {
	  mov(6, 0);
	  last_reg_used = 6;
	  return R6;
	}
	else {
	  exit(printf("Error in saveRet.\n"));
	}
  }

 private:
  uint restoreValue(reg_t reg)
  {
	return reg;
  }

};



#endif // FCTCALLHELPER_H
