#ifndef ROPGEN_H
# define ROPGEN_H

#include "ROPHelper.h"

#include <iostream>
#include <bitset>
#include <climits>

class ROPGen : public ROPHelper {
private:
  BYTE*		_payload;
  BYTE*		_pcOffset;
  BYTE*		_initialPC;
  uint		_cookieValue;
  uint		_slideSP;
  uint		_maxROPSize;
  uint		_gadgetCount;
  uint		_pcLoc;
  BYTE		_allowsZeros;

protected:
  uint		_slideValue;
  BYTE		thumbMode;

public:
  ROPGen(uint cookieValue, uint slideValue,
		 const char* fileName, uint maxSize);

  virtual ~ROPGen();

  void printChainHex();

  void printChainB64();

  void printChainB2();

  void printChainBinary();

  void printStats();

  BYTE verifyPayload();

protected:
  void writeValue(uint value, uint pos);

  void setPClocation(uint pos);

  BYTE areZerosAllowed();

  void setChainMaxSize(uint size);

  void allowsZeros();

  void addGadget(uint gadgetAddr);

  void addSymbol(uint funcAddr, BYTE debug = TRUE);

  void addBytes(uint value = DEFAULT_ADDED_BYTES, 
				uint nb_bytes = ADDR_SIZE,
				BYTE debug = TRUE);

  uint getNewSP();

  uint getSlideValue();

  uint getPCLoc();

  BYTE* getPayload();

  BYTE* getInitialPC();

  uint getMaxROPSize();

};

#endif // ROPGEN_H
