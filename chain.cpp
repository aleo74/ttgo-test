#include "AES.h"
#include "base64.h"
#include "chain.h"


Block::Block(long timestamp) {
  this->_timestamp = timestamp;
  //this->generateIV();
}

Block::Block(int timestamp, String dataBlock, String indexChain, String previousHash){
  this->_timestamp = timestamp;
  this->_dataBlock = dataBlock;
  this->_indexChain = indexChain;
  this->_previousHash = previousHash;
}

void Block::hashBlock() {
  
}

void Block::setTimestamp(long timestamp) {
  this->_timestamp = timestamp;
}

void Block::setData(String mydata) {
  this->_dataBlock = mydata;
}

void Block::verifyHash() {
  
}

void Block::generateIV(byte  *iv) {
  for (int i = 0 ; i < 16 ; i++ ) {
        iv[i]= (byte) random(10,20);
    }
}

long Block::getTimestamp(){
  return this->_timestamp;
}

String Block::getIV(){
  String str = "";
  for (uint8_t i = 0; i < sizeof(this->_my_iv); i++)
  {
    str += String(String(45, HEX) + this->_my_iv[i]);
  }
  return str;
}

void Block::setPreviousHash() {
  this->_previousHash = "0";
}

String Block::getHash() {
  return this->_hash;
}
void Block::setHash(String hash) {
  this->_hash = hash;
}
