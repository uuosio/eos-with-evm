#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>

#include <eosio/crypto.hpp>
#include <eosio/dispatcher.hpp>


using namespace eosio;

extern "C" {
   int64_t eosio_system_get_delegated_balance(uint64_t owner) {
//      eosio::datastream<const char*> _ds = datastream<const char*>(nullptr, 0);
//      eosiosystem::system_contract t("eosio"_n, "eosio"_n, _ds);
      return eosiosystem::system_contract::get_delegated_balance(name(owner));
   }
   int64_t eosio_system_get_rex_fund(uint64_t owner) {
      return eosiosystem::system_contract::get_rex_fund(name(owner));
   }

   bool system_contract_is_vm_activated( uint8_t vmtype, uint8_t vmversion ) {
      return eosiosystem::system_contract::is_vm_activated(vmtype, vmversion);
   }
}

#ifdef _NATIVE

#define EOSIO_DISPATCH_NATIVE( TYPE, MEMBERS ) \
extern "C" { \
   __attribute__ ((visibility ("default"))) void eosio_system_apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( code == receiver ) { \
         switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_DISPATCH_NATIVE(eosiosystem::system_contract, (init)(setacctram)(setacctnet)(setacctcpu)(activate)(delegatebw)(deposit)
(withdraw)(buyrex)(unstaketorex)(sellrex)(cnclrexorder)(rentcpu)(rentnet)
(fundcpuloan)(fundnetloan)(defcpuloan)(defnetloan)(updaterex)(rexexec)(setrex)
(mvtosavings)(mvfrsavings)(consolidate)(closerex)(undelegatebw)(buyram)(buyrambytes)
(sellram)(refund)(regproducer)(unregprod)(setram)(setramrate)(voteproducer)(regproxy)
(claimrewards)(rmvproducer)(updtrevision)(bidname)(bidrefund)(setpriv)(setalimits)(setparams)
)

#endif
