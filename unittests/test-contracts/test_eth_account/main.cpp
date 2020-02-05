/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#include "main.hpp"
#include "eth_account.hpp"

using namespace eosio;

__attribute__((eosio_wasm_import))
extern "C" int evm_execute(const unsigned char* code, size_t size);

extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
   //2c7536E3605D9C16a7a3D7b1898e529396a65c23
   const char *_eth_address1 = ",u6\xe3`]\x9c\x16\xa7\xa3\xd7\xb1\x89\x8eR\x93\x96\xa6\\#";
   const char *_eth_address2 = ",u6\xe3`]\x9c\x16\xa7\xa3\xd7\xb1\x89\x8eR\x93\x96\xa7\\#";
   const char *_eth_address3 = ",u6\xe3`]\x9c\x16\xa7\xa3\xd7\xb1\x89\x8eR\x93\x96\xa8\\#";
   eth_address eth_address1;
   eth_address eth_address2;
   eth_address eth_address3;
   memcpy(eth_address1.data(), _eth_address1, 20);
   memcpy(eth_address2.data(), _eth_address2, 20);
   memcpy(eth_address3.data(), _eth_address3, 20);

   if (action == "testcreate"_n.value) {
      key256 key;
      value256 value;
      memcpy(key.data(), "11", 2);
      memcpy(value.data(), "33", 2);

      check(!eth_account_set_value(eth_address1, key, value), "eth_account_set_value should return false before an ethereum account created");

      bool ret = eth_account_create(eth_address1, 0);
      check(ret, "eth_account_create failed");
      ret = eth_account_exists(eth_address1);
      check(ret, "account does not exists");

      ret = eth_account_create(eth_address2);
      check(ret, "eth_account_create failed");
      ret = eth_account_exists(eth_address2);
      check(ret, "account does not exists");

      uint64_t index = eth_account_get_info(eth_address2, nullptr, nullptr, nullptr);
      check(index == 2, "bad account index");

      std::vector<unsigned char> code;
      ret = eth_account_get_code(eth_address3, code);
      check(ret == false, "eth_account_get_code expected to return false");

      eth_account_create(eth_address3);
      ret = eth_account_get_code(eth_address3, code);
      check(ret, "eth_account_get_code expected to return true");

      check(code.size() == 0, "code size should be 0");

      code.resize(10);
      eth_account_set_code(eth_address3, code);

      code.resize(0);
      eth_account_get_code(eth_address3, code);
      check(code.size() == 10, "code size should be 10");


      check(!eth_account_get_value(eth_address1, key, value), "value expected to not exist");

      eth_account_set_value(eth_address1, key, value);
      check(eth_account_get_value(eth_address1, key, value), "value expected to exist");

      value256 expected_value;
      memcpy(expected_value.data(), "33", 2);
      check(value == expected_value, "value not as expected!");

      check(eth_account_clear_value(eth_address1, key), "eth_account_clear_value expected to return true");
      check(!eth_account_clear_value(eth_address1, key), "eth_account_clear_value expected to return false");
      check(!eth_account_get_value(eth_address1, key, value), "value expected to not exist");

      uint32_t nonce = 0;
      ret = eth_account_get_nonce(eth_address1, nonce);
      check(ret == true, "bad eth_account_get_nonce return");
      check(nonce == 0, "bad eth_account_get_nonce return");

      nonce = 1;
      eth_account_set_nonce(eth_address1, nonce);
      ret = eth_account_get_nonce(eth_address1, nonce);
      check(ret == true, "bad eth_account_get_nonce return");
      check(nonce == 1, "bad eth_account_get_nonce return");

   } else if (action == "testexists"_n.value) {
      bool ret = eth_account_create(eth_address1);
      check(!ret, "eth_account_create should return false");
      ret = eth_account_exists(eth_address1);
      check(ret, "account does not exists");
   } else if (action == "testsetcode"_n.value) {
      unsigned char sender_address[] = {0x2c, 0x75, 0x36, 0xe3, 0x60, 0x5d, 0x9c, 0x16, 0xa7, 0xa3, 0xd7, 0xb1, 0x89, 0x8e, 0x52, 0x93, 0x96, 0xa6, 0x5c, 0x23};
      unsigned char *_addr = (unsigned char *)"\xc0\xaa\xe1\xed\xd7\xa7\x6c\x8c\xf9\x9e\x5b\xa3\xca\x69\x59\x9e\xd2\x95\x40\xea";

      eth_address addr;
      memcpy(addr.data(), sender_address, 20);
      //create account first
      bool ret = eth_account_create(addr);
      check(ret, "+++eth_account_create return false");
      std::vector<unsigned char> code;
      int32_t code_size = eosio::action_data_size();
      code.resize(code_size);
      eosio::read_action_data(code.data(), code_size);
      evm_execute(code.data(), code_size);

      memcpy(addr.data(), _addr, 20);
      check(eth_account_exists(addr), "account should exists");
   } else if (action == "testcall"_n.value) {
      std::vector<unsigned char> code;
      int32_t code_size = eosio::action_data_size();
      code.resize(code_size);
      eosio::read_action_data(code.data(), code_size);
      evm_execute(code.data(), code_size);
   }
}
