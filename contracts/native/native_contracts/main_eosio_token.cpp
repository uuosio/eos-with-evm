
#include <eosio.token/eosio.token.hpp>

#define EOSIO_DISPATCH_NATIVE( TYPE, MEMBERS ) \
extern "C" { \
   __attribute__ ((visibility ("default"))) void native_eosio_token_apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( code == receiver ) { \
         switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

#ifdef EOSIO_NATIVE
EOSIO_DISPATCH_NATIVE( eosio::token, (create)(issue)(transfer)(open)(retire) )
#endif

//EOSIO_DISPATCH( eosio::token, (create)(issue)(transfer)(open)(retire) )


