#include "ROPGen.h"

// ########################
// ropchain.cpp
// ########################

#include <iostream>
#include <bitset>
#include <climits>

ROPGen::ROPGen(uint cookieValue, uint slideValue,
			   const char* fileName, uint maxSize) : 
  ROPHelper(fileName)
{
  _cookieValue = cookieValue;
  _slideValue = slideValue;
  _payload = new BYTE[maxSize];
  memset(_payload, EMPTY_BYTE, maxSize);
  _pcOffset = _payload;
  _initialPC = 0;
  _slideSP = 0;
  _gadgetCount = 0;
  _allowsZeros = FALSE;

  thumbMode = 1;
 
  // default max rop size set to 30%
  _maxROPSize = maxSize - (maxSize / 10 * 3);
}

ROPGen::~ROPGen() {
  delete _payload;
}

void ROPGen::printChainHex() {
  uint size = _maxROPSize + (_maxROPSize % 16 + 1);
  uint groupBy = 16;
  char c;
  uint align = 0;

  if (DEBUG_PRINT)
  	align = 3;
  
  for (uint i = 0; i < size; i++) {
	
	if (!(i % groupBy)) {
	  printf("0x%.4x: ", i);
	}
	
	// display 1 byte at a time
	if (!DEBUG_PRINT)
	  printf("%.2x", *(_payload + i + align));
	
	if (!((i + 1) % 4)) {
	  // display 4 bytes at a time as integer
	  if (DEBUG_PRINT)
		printf("%.8x ", *(uint*)(_payload + i + align + 1));
	  else
		printf(" ");
	}
	
	if (!((i + 1) % groupBy)) {
	  for (uint j = 0; j < groupBy; j++) {
		if (!(j % 4))
		  printf(" ");
		c = *(_payload + i - groupBy + 1 + j + align);
		printf("%c", isprint(c) ? c : '.');
	  }
	  printf("\n");
	}
  }
  printf("\n");
}

BYTE ROPGen::verifyPayload() {
  uint size = _maxROPSize - (_maxROPSize % 16 + 1);
  BYTE found = 0;

  for (uint i = 0; i < size; i++) {
	// read 1 byte at a time
	if (*(_payload + i) == 0) {
	  if (!found++)
		printf("\nFound locations with 0: ");
	  printf("0x%x ", i);
	}
  }
  if (found) {
	printf("\n");
	printf("Error: The payload contains %d bytes set to 0.\n"
		   "It could be due to the cookie or "
		   "function addresses.\n", found);
	return FALSE;
  }

  return TRUE;
}

void ROPGen::printChainB64() {
  if (DEBUG_PAYLOAD)
	printf("<value>%s</value>\n", utils::base64_encode(_payload, _maxROPSize).c_str());
  else
	printf("%s", utils::base64_encode(_payload, _maxROPSize).c_str());
}

void ROPGen::printChainB2() {
  for (uint i = 0; i < _maxROPSize; i++) {
	if (!((i) % 8))
	  printf("0x%.4x: ", i);
	cout << std::bitset<CHAR_BIT>(*(_payload + i)) << " ";
	if (!((i + 1) % 8))
	  cout << "\n";
  }	
}

void ROPGen::printChainBinary() {
  for (uint i = 0; i < _maxROPSize; i++) {
	printf("%c", *(_payload + i));
  }
}

void ROPGen::printStats() {
  printf("Gadget used: %d\n", _gadgetCount / 4);
  printf("Size payload without extra: %d (0x%x)\n", 
		 _slideSP, _slideSP);
}

// protected:

void ROPGen::writeValue(uint value, uint pos) {
  *(uint*)(_payload + pos) = value;
}

void ROPGen::setPClocation(uint pos) {
  _pcOffset = _payload + pos;
  _pcLoc = pos;
  _initialPC = _pcOffset;
}

void ROPGen::allowsZeros() {
  _allowsZeros = TRUE;
}

BYTE ROPGen::areZerosAllowed() {
  return _allowsZeros;
}

void ROPGen::setChainMaxSize(uint size) {
  _maxROPSize = size;
  // end the chain with four 0
  for (int i = 0; i < 4; i++)
	writeValue(0, size - 1 + i);
}

void ROPGen::addGadget(uint gadgetAddr) {
  addBytes(gadgetAddr + _slideValue + thumbMode, 
		   ADDR_SIZE, false);
  _gadgetCount++;
}

void ROPGen::addSymbol(uint funcAddr,
					   BYTE debug /* = TRUE */) {
  addBytes(funcAddr + _slideValue, ADDR_SIZE, debug);
}

void ROPGen::addBytes(uint value /*= DEFAULT_ADDED_BYTES */, 
					  uint nb_bytes /* = ADDR_SIZE */,
					  BYTE debug /* = FALSE */ ) {

  if (DEBUG_PAYLOAD && debug) {
	if (nb_bytes == ADDR_SIZE)
	  printf("......++0x%x\n", value);
	else
	  printf("......++0x%x (%dx)\n", 
			 value, nb_bytes / ADDR_SIZE);
  }

  for (uint i = 0; i < nb_bytes; i += ADDR_SIZE) {

	if (areZerosAllowed() &&
		DEBUG_PAYLOAD && utils::isThereANull(value))
	  {
		printf("WARNING! contains a 0, gadgetaddr: 0x%x!\n",
			   value);
	  }

	*(uint*)(_pcOffset + i) = value;
  }

  _pcOffset += nb_bytes;
  _slideSP += nb_bytes;
}

uint ROPGen::getNewSP() {
  return _slideSP;
}

BYTE* ROPGen::getInitialPC() {
  return _initialPC;
}  

uint ROPGen::getPCLoc() {
  return _pcLoc;
}

BYTE* ROPGen::getPayload() {
  return _payload;
}

uint ROPGen::getMaxROPSize() {
  return _maxROPSize;
}

uint ROPGen::getSlideValue() {
  return _slideValue;
}

