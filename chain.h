#include "Arduino.h"
#ifndef chain_h

//  The #define statement defines this file as the myFirstLibrary
//  Header File so that it can be included within the source file.                                           
#define chain_h

class Block {
  private:
    unsigned long _timestamp;
    String _hash;
    String _dataBlock;
    String _indexChain;
    String _previousHash;
    byte _my_iv[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  public: 
    Block(long timestamp);
    Block(int timestamp, String dataBlock, String indexChain, String previousHash);
    void hashBlock();
    void setTimestamp(long timestamp);
    void setData(String mydata);
    void verifyHash();
    void generateIV(byte  *iv);
    long getTimestamp();
    String getIV();
    String getHash();
    void setHash(String hash);
    void setPreviousHash();
};

#endif
