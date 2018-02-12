#include "ROPHelper.h"

// ########################
// rophelper.cpp
// ########################

ROPHelper::ROPHelper(const char* fileName) : 
  _fileName(fileName) {
  // do not load file here as the slide is not set in the object yet
  // and it is needed for the checks
}

void ROPHelper::loadFile(const char* fileName,
						 BYTE loadAll /* = FALSE */)
{
    uint				line = 0;
	ifstream			inFile;
	string				strLine;
	uint				addr;
	string				gadget;

	inFile.open(fileName);
	if (!inFile.is_open())
	  exit(printf("File [%s] could not"
				  " be opened.\n", fileName));

	while (inFile.good()) {
	  getline(inFile, strLine);
	  // trim
	  strLine = utils::ltrim(strLine);
	  if (!strLine.empty() && 
		  strLine[strLine.size() - 1] == '\r')
		strLine.erase(strLine.size() - 1);

	  // parse line, has to follow the following format:
	  // 0x2c00e138 : GADGET
	  if (strLine.size() > 14 &&
		  strLine.find_first_of(":") == 11)
		{
		  if (DEBUG) {
			cout << "{";
			cout << strLine.substr(2, 8) << "|";
			cout << strLine.substr(13, -1) << "}";
		  }

		  // extract addr
		  std::stringstream	ss;
		  ss << std::hex << strLine.substr(2, 8).c_str();
		  ss >> addr;
		  
		  gadget = strLine.substr(13, -1);
		  // do not store gadgets containing 0 in the address
		  if (areZerosAllowed() || loadAll ||
			  !utils::isThereANull(addr + getSlideValue())) {
			// 	// map gadget
			_gadgets[gadget] = addr;
		  }
		  else if (DEBUG)
		   	printf("--> Gadget addr 0x%x contains a 0 "
				   "for gadget {%s}, skipping\n",
				   addr, gadget.c_str());
        }
	  line++;
	  if (DEBUG)
		cout << "\n";
	}
	inFile.close();
}

uint ROPHelper::findGadget(string instructions,
						   BYTE debug /* = TRUE*/)
{
  uint gadget = _gadgets[instructions];
  if (!gadget) {
	// try again, with more gadgets
	// (some of which could contain 0s)
	if (!areZerosAllowed()) {
	  loadFile(_fileName, TRUE);
	  gadget = _gadgets[instructions];
	}
	if (!gadget)
	  exit(printf("Cannot find gadget [%s]\n", instructions.c_str()));
  }
  
  if (DEBUG_PAYLOAD && debug)
  	printf("......0x%x  %s\n", 
  		   gadget + getSlideValue(),
  		   instructions.c_str());
  
  return gadget;
}

uint ROPHelper::findGadget_fast(string instructions)
{
  uint gadget = _gadgets[instructions];
  if (!gadget)
	return 0;

  if (DEBUG_PAYLOAD)
  	printf("......0x%x  %s\n", 
  		   gadget + getSlideValue(),
  		   instructions.c_str());

  return gadget;
}
