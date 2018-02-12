#ifndef _UTILS_H
# define _UTILS_H

//main stream op
#include <iostream>
#include <sstream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <list>
#include <vector>

//strlen
#include <string.h>

// types
#include <stdint.h>
typedef uint32_t		uint;
typedef uint8_t			BYTE;

#ifndef __bool_true_false_are_defined
# define TRUE 1
# define FALSE 0
#endif

using namespace std;

//utils.h
namespace utils {
  std::string base64_encode(BYTE const* buf, unsigned int bufLen);
  std::string &ltrim(std::string &s);
  uint changeEndianness32(int32_t tmp);
  uint isThereANull(int32_t tmp);
  uint isThereANull_24(int32_t tmp);
  BYTE isDecNumber(char* str);
  BYTE isHexNumber(char* str);
}

#endif // _UTILS_H
