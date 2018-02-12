#ifndef CREATE_PAYLOAD_H
# define CREATE_PAYLOAD_H

#include "IROP.h"

// defined in payloads.cpp 
// to make it easier to
// add payloads

class CreatePayload {
private:
  uint _cookieValue;
  uint _slideValue;
  const char* _fileName;
  IROP*	_rop;

public:
  CreatePayload(uint cookieValue, 
				uint slideValue,
				const char* fileName) : 
    _cookieValue(cookieValue),
	_slideValue(slideValue), 
	_fileName(fileName),
	_rop(NULL) { }
  
  IROP* generate(uint payloadNumber);
  virtual ~CreatePayload();
};

#endif // CREATE_PAYLOAD_H
