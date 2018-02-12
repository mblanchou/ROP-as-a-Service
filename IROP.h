#ifndef IROP_H
# define IROP_H

class IROP {
 public:
  virtual void loadFile(const char* fileName, BYTE loadAll) = 0;
  virtual uint findGadget(string gadget, BYTE debug) = 0;
  virtual uint findGadget_fast(string gadget) = 0;
  virtual ~IROP() {}

  virtual void printChainHex() = 0;
  virtual void printChainB64() = 0;
  virtual void printChainB2() = 0;
  virtual void printChainBinary() = 0;
  virtual void printStats() = 0;
  virtual BYTE verifyPayload() = 0;

protected:
  virtual void writeValue(uint value, uint pos) = 0;
  virtual void setPClocation(uint pos) = 0;
  virtual void setChainMaxSize(uint size) = 0;
  virtual void addGadget(uint gadgetAddr) = 0;
  virtual void addSymbol(uint funcAddr, BYTE debug) = 0;
  virtual void addBytes(uint value,	uint nb_bytes, BYTE debug) = 0;
  virtual uint getNewSP() = 0;
  virtual BYTE* getInitialPC() = 0;
  virtual uint getMaxROPSize() = 0;
  virtual uint getSlideValue() = 0;
  virtual BYTE areZerosAllowed() = 0;
};

#endif // IROP_H
