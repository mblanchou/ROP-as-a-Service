#ifndef ROPHelper_H
# define ROPHelper_H

#include "main.h"
#include "IROP.h"

class ROPHelper : public IROP {
private:
  std::map<string, uint>		_gadgets;
  const char*				_fileName;

public:
  ROPHelper(const char* fileName);
  void loadFile(const char* fileName, BYTE loadAll = FALSE);

  uint findGadget(string gadget, BYTE debug = TRUE);
  uint findGadget_fast(string gadget);
};

#endif //ROPHelper_H
