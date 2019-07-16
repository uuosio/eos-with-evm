/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */

#include <eosiolib/system.h>

#include "vm_api.h"

extern "C" {

void eosio_abort() {
   get_vm_api()->eosio_abort();
}

void  eosio_assert( uint32_t test, const char* msg ) {
   get_vm_api()->eosio_assert( test, msg );
}

void  eosio_assert_message( uint32_t test, const char* msg, uint32_t msg_len ) {
   get_vm_api()->eosio_assert_message( test, msg, msg_len );
}

void  eosio_assert_code( uint32_t test, uint64_t code ) {
   get_vm_api()->eosio_assert_code( test, code );
}

void  eosio_exit( int32_t code ) {
   get_vm_api()->eosio_exit( code );
}

uint64_t  current_time() {
   return get_vm_api()->current_time();
}

uint32_t  now() {
   return (uint32_t)( current_time() / 1000000 );
}

void set_last_error(const char* error, size_t size) {
   get_vm_api()->set_last_error(error, size);
}

size_t get_last_error(char* error, size_t size) {
   return get_vm_api()->get_last_error(error, size);
}

void clear_last_error() {
   get_vm_api()->clear_last_error();
}

}
