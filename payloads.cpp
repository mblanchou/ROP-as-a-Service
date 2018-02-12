#include "main.h"
#include "FctCallHelper.h"
#include "CreatePayload.h"

class Payload_1 : public FctCallHelper
{
public:
  Payload_1(uint cookieValue, uint slideValue,
		   const char* fileName) : 
	FctCallHelper(cookieValue,
		      slideValue, 
		      fileName, 
		      0x500) // max total Size to use (EXAMPLE)
  {
    // Use all helper functions to create the payload
  }  
};

class Payload_2 : public FctCallHelper
{
public:
  Payload_2(uint cookieValue, uint slideValue,
		   const char* fileName) : 
	FctCallHelper(cookieValue,
		      slideValue, 
		      fileName, 
		      0x42000) // max total Size to use (EXAMPLE)
  {
    // Use all helper functions here to create the payload
  }
};

// ETC.

IROP* CreatePayload::generate(uint payloadNumber)
{
  if (payloadNumber == 1)
	_rop = new Payload_1(_cookieValue, _slideValue, _fileName);
  else if (payloadNumber == 2)
	_rop = new Payload_2(_cookieValue, _slideValue, _fileName);
  // ETC.
  else
	exit(printf("Payload %d does not exit\n", payloadNumber));
  return _rop;
}

CreatePayload::~CreatePayload()
{
  if (_rop != NULL)
	delete _rop;
}
