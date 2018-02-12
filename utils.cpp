#include "utils.h"

// ltrim
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// b64
#include <vector>
#include <string>
#include <iostream>

static const std::string base64_chars = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";

// ltrim
std::string&		utils::ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// endianness
uint				utils::changeEndianness32(int32_t tmp) {
  return (tmp << 24) |
	((tmp <<  8) & 0x00ff0000) |
	((tmp >>  8) & 0x0000ff00) |
	((tmp >> 24) & 0x000000ff);
}

BYTE utils::isDecNumber(char* str) {
  for (int i = 0; str[i]; i++) {
	if (str[i] < '0' || str[i] > '9')
	  return FALSE;
  }
  return TRUE;
}

BYTE utils::isHexNumber(char* str) {
  for (int i = 0; str[i]; i++) {
	if ((str[i] >= '0' && str[i] <= '9') || 
		(str[i] >= 'A' && str[i] <= 'F') || 
		(str[i] >= 'a' && str[i] <= 'f'))
	  continue;
	return FALSE;
  }
  return TRUE;
}

// check if any byte is set to 0
uint utils::isThereANull(int32_t tmp) {
  return 
	!(tmp & 0x000000ff) ||
	!(tmp & 0x0000ff00) ||
	!(tmp & 0x00ff0000) ||
	!(tmp & 0xff000000);
}

// check if any byte is set to 0
// check only 3 bytes
uint utils::isThereANull_24(int32_t tmp) {
  return 
	!(tmp & 0x000000ff) ||
	!(tmp & 0x0000ff00) ||
	!(tmp & 0x00ff0000);
}

// Rene Nyffenegger's B64 decoder
std::string utils::base64_encode(BYTE const* buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
	  
      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }
  
  if (i) {
	for(j = i; j < 3; j++)
	  char_array_3[j] = '\0';
	
	char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	char_array_4[3] = char_array_3[2] & 0x3f;
	
	for (j = 0; (j < i + 1); j++)
	  ret += base64_chars[char_array_4[j]];
	
	while((i++ < 3))
	  ret += '=';
  }
  
  return ret;
}
