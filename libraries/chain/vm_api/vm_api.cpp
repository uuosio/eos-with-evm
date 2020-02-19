#include <eosio/chain/apply_context.hpp>
#include <eosio/chain/controller.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/producer_schedule.hpp>
#include <eosio/chain/exceptions.hpp>
#include <boost/core/ignore_unused.hpp>
#include <eosio/chain/authorization_manager.hpp>
#include <eosio/chain/resource_limits.hpp>
/*
#include <eosio/chain/wasm_interface_private.hpp>
#include <eosio/chain/wasm_eosio_validation.hpp>
#include <eosio/chain/wasm_eosio_injection.hpp>
*/
#include <eosio/chain/global_property_object.hpp>
#include <eosio/chain/account_object.hpp>
#include <eosio/chain/symbol.hpp>

#include <fc/exception/exception.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/sha1.hpp>
#include <fc/io/raw.hpp>
#include <fc/uint128.hpp>

#include <softfloat.hpp>
#include <compiler_builtins.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <eosio/chain/protocol_state_object.hpp>

using namespace eosio::chain;
#include <fstream>
#include <dlfcn.h>

static apply_context *s_ctx = nullptr;


//vm_exceptions.cpp
void vm_throw_exception(int type, const char* fmt, ...);

//native/native.cpp
int transfer_inline(uint64_t to, int64_t amount, uint64_t symbol);
int transfer(uint64_t from, uint64_t to, int64_t amount, uint64_t symbol);

//evm4eos
extern "C" {
   void eosio_assert(uint32_t cnd, const char* msg);

   int evm_execute(const char *raw_trx, size_t raw_trx_size, const char *sender_address, size_t sender_address_size);
}

namespace eosio {
namespace chain {

#include <vm_api/vm_api.h>
#include <stacktrace.h>
static struct vm_api _vm_api = {
};

void set_apply_context(apply_context *ctx) {
   s_ctx = ctx;
   if (ctx == nullptr) {
      _vm_api.is_in_apply_context = false;
      _vm_api.allow_access_apply_context = false;
   } else {
      _vm_api.is_in_apply_context = true;
      _vm_api.allow_access_apply_context = true;
   }
}

apply_context *get_apply_context() {
   eosio_assert(s_ctx != nullptr, "not in apply context");
   return s_ctx;
}

static inline apply_context& ctx() {
   if (!_vm_api.allow_access_apply_context) {
      print_stacktrace();
      _vm_api.eosio_assert(0, "access apply context not allowed!");
   }
   return *s_ctx;
}

static inline apply_context& get_ctx_no_access_check() {
   eosio_assert(s_ctx != nullptr, "not in apply context");
   return *s_ctx;
}

#include "action.cpp"
#include "chain.cpp"
#include "system.cpp"
#include "crypto.cpp"
#include "db.cpp"
#include "privileged.cpp"
#include "transaction.cpp"
#include "print.cpp"
#include "permission.cpp"

#define MAX_BUFFER_SIZE 1024
static vector<char> print_buffer;
static char output[1024];

void log_(int level, int line, const char *file, const char *func, const char *fmt, ...) {
//void log_(const char *output, int level, int line, const char *file, const char *func) {
   va_list args;
   va_start(args, fmt);
   int len = vsnprintf(output, sizeof(output), fmt, args);
   va_end(args);

   for (int i=0;i<len;i++) {
      if (output[i] == '\n' || print_buffer.size()>MAX_BUFFER_SIZE) {
         string s(print_buffer.data(), print_buffer.size());

         FC_MULTILINE_MACRO_BEGIN
          if( (fc::logger::get(DEFAULT_LOGGER)).is_enabled( (fc::log_level::values)level ) )
             (fc::logger::get(DEFAULT_LOGGER)).log( fc::log_message( fc::log_context((fc::log_level::values)level, file, line, func ), s.c_str() ));
         FC_MULTILINE_MACRO_END;

         print_buffer.clear();

         continue;
      }
      print_buffer.push_back(output[i]);
   }
}
#if 0
bool is_debug_mode_() {
   return appbase::app().debug_mode();
}

bool is_unittest_mode() {
   return false;
}

void set_debug_mode(bool b) {
   appbase::app().set_debug_mode(b);
}

int get_wasm_runtime_type() {
   return options::get().get_wasm_runtime_type();
}

int is_contracts_console_enabled() {
   return options::get().is_contracts_console_enabled();
}
#endif

// void wasm_call(uint64_t contract, uint64_t func_name, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
//    ctx().control.get_wasm_interface().call(contract, func_name, arg1, arg2, arg3, ctx());
// }

// void vm_call(uint64_t contract, uint64_t func_name, uint64_t arg1, uint64_t arg2, uint64_t arg3, const char* extra_args, size_t in_size) {
//    vm_manager::get().call(contract, func_name, arg1, arg2, arg3, extra_args, in_size);
// }

// int call_contract_get_extra_args(void* extra_args, size_t size1) {
//    return vm_manager::get().get_extra_args((char*)extra_args, size1);
// }

// int call_contract_set_results(const void* result, size_t size1) {
//    return vm_manager::get().set_result((const char*)result, size1);
// }

// int call_contract_get_results(void* result, size_t size1) {
//    return vm_manager::get().get_result((char*)result, size1);
// }

static void __ashlti3(__int128* ret, uint64_t low, uint64_t high, uint32_t shift) {
    fc::uint128_t i(high, low);
    i <<= shift;
    *ret = (unsigned __int128)i;
}

static void __lshrti3(__int128* ret, uint64_t low, uint64_t high, uint32_t shift) {
   fc::uint128_t i(high, low);
   i >>= shift;
   *ret = (unsigned __int128)i;
}

static bool get_code_version(uint64_t contract, char *hash, size_t size) {
   if (hash == nullptr || size != 32) {
      return false;
   }
   if (!is_account(contract)) {
      return false;
   }
   try {
      const auto& account = ctx().control.db().get<account_metadata_object,by_name>(name(contract));
      memcpy(hash, account.code_hash.data(), 32);
      return true;
   } catch (...) {
   }
   return false;
}

using namespace eosio;

extern "C" void vm_api_init() {
   static bool s_init = false;
   if (!s_init){
      s_init = true;
      // _vm_api.vm_apply = vm_apply;
      _vm_api.read_action_data = read_action_data;
      _vm_api.action_data_size = action_data_size;
      _vm_api.get_action_info = get_action_info;
      
      _vm_api.require_recipient = require_recipient;
      _vm_api.require_auth = require_auth;
      _vm_api.require_auth2 = require_auth2;
      _vm_api.has_auth = has_auth;
      _vm_api.is_account = is_account;

      _vm_api.send_inline = send_inline;
      _vm_api.send_context_free_inline = send_context_free_inline;
      _vm_api.publication_time = publication_time;
      _vm_api.current_receiver = current_receiver;
      _vm_api.set_action_return_value = set_action_return_value;
      _vm_api.get_active_producers = get_active_producers;

      _vm_api.assert_sha256 = assert_sha256;
      _vm_api.assert_sha1 = assert_sha1;
      _vm_api.assert_sha512 = assert_sha512;
      _vm_api.assert_ripemd160 = assert_ripemd160;
      _vm_api.assert_recover_key = assert_recover_key;
      _vm_api.sha256 = sha256;
      _vm_api.sha1 = sha1;
      _vm_api.sha512 = sha512;
      _vm_api.ripemd160 = ripemd160;
      _vm_api.recover_key = recover_key;
      _vm_api.sha3 = nullptr;

      _vm_api.db_store_i64 = db_store_i64;
      _vm_api.db_update_i64 = db_update_i64;
      _vm_api.db_remove_i64 = db_remove_i64;
      _vm_api.db_get_i64 = db_get_i64;
      _vm_api.db_next_i64 = db_next_i64;

      _vm_api.db_previous_i64 = db_previous_i64;
      _vm_api.db_find_i64 = db_find_i64;
      _vm_api.db_lowerbound_i64 = db_lowerbound_i64;
      _vm_api.db_upperbound_i64 = db_upperbound_i64;
      _vm_api.db_end_i64 = db_end_i64;


      _vm_api.db_store_i256 = db_store_i256;
      _vm_api.db_update_i256 = db_update_i256;
      _vm_api.db_remove_i256 = db_remove_i256;
      _vm_api.db_get_i256 = db_get_i256;
      _vm_api.db_find_i256 = db_find_i256;

      _vm_api.db_previous_i256 = db_previous_i256;
      _vm_api.db_next_i256 = db_next_i256;

      _vm_api.db_upperbound_i256 = db_upperbound_i256;
      _vm_api.db_lowerbound_i256 = db_lowerbound_i256;
      _vm_api.db_end_i256 = db_end_i256;

      _vm_api.db_get_table_count = db_get_table_count;

      _vm_api.db_idx64_store = db_idx64_store;
      _vm_api.db_idx64_update = db_idx64_update;

      _vm_api.db_idx64_remove = db_idx64_remove;
      _vm_api.db_idx64_next = db_idx64_next;
      _vm_api.db_idx64_previous = db_idx64_previous;
      _vm_api.db_idx64_find_primary = db_idx64_find_primary;
      _vm_api.db_idx64_find_secondary = db_idx64_find_secondary;
      _vm_api.db_idx64_lowerbound = db_idx64_lowerbound;
      _vm_api.db_idx64_upperbound = db_idx64_upperbound;
      _vm_api.db_idx64_end = db_idx64_end;
      _vm_api.db_idx128_store = db_idx128_store;

      _vm_api.db_idx128_update = db_idx128_update;
      _vm_api.db_idx128_remove = db_idx128_remove;
      _vm_api.db_idx128_next = db_idx128_next;
      _vm_api.db_idx128_previous = db_idx128_previous;
      _vm_api.db_idx128_find_primary = db_idx128_find_primary;
      _vm_api.db_idx128_find_secondary = db_idx128_find_secondary;
      _vm_api.db_idx128_lowerbound = db_idx128_lowerbound;
      _vm_api.db_idx128_upperbound = db_idx128_upperbound;

      _vm_api.db_idx128_end = db_idx128_end;
      _vm_api.db_idx256_store = db_idx256_store;
      _vm_api.db_idx256_update = db_idx256_update;
      _vm_api.db_idx256_remove = db_idx256_remove;
      _vm_api.db_idx256_next = db_idx256_next;

      _vm_api.db_idx256_previous = db_idx256_previous;
      _vm_api.db_idx256_find_primary = db_idx256_find_primary;
      _vm_api.db_idx256_find_secondary = db_idx256_find_secondary;
      _vm_api.db_idx256_lowerbound = db_idx256_lowerbound;
      _vm_api.db_idx256_upperbound = db_idx256_upperbound;
      _vm_api.db_idx256_end = db_idx256_end;
      _vm_api.db_idx_double_store = db_idx_double_store;
      _vm_api.db_idx_double_update = db_idx_double_update;
      _vm_api.db_idx_double_remove = db_idx_double_remove;
      _vm_api.db_idx_double_next = db_idx_double_next;
      _vm_api.db_idx_double_previous = db_idx_double_previous;
      _vm_api.db_idx_double_find_primary = db_idx_double_find_primary;
      _vm_api.db_idx_double_find_secondary = db_idx_double_find_secondary;
      _vm_api.db_idx_double_lowerbound = db_idx_double_lowerbound;
      _vm_api.db_idx_double_upperbound = db_idx_double_upperbound;
      _vm_api.db_idx_double_end = db_idx_double_end;
      _vm_api.db_idx_long_double_store = db_idx_long_double_store;
      _vm_api.db_idx_long_double_update = db_idx_long_double_update;
      _vm_api.db_idx_long_double_remove = db_idx_long_double_remove;
      _vm_api.db_idx_long_double_next = db_idx_long_double_next;
      _vm_api.db_idx_long_double_previous = db_idx_long_double_previous;
      _vm_api.db_idx_long_double_find_primary = db_idx_long_double_find_primary;
      _vm_api.db_idx_long_double_find_secondary = db_idx_long_double_find_secondary;
      _vm_api.db_idx_long_double_lowerbound = db_idx_long_double_lowerbound;
      _vm_api.db_idx_long_double_upperbound = db_idx_long_double_upperbound;
      _vm_api.db_idx_long_double_end = db_idx_long_double_end;

      _vm_api.check_transaction_authorization = check_transaction_authorization;
      _vm_api.check_permission_authorization = check_permission_authorization;
      _vm_api.get_permission_last_used = get_permission_last_used;
      _vm_api.get_account_creation_time = get_account_creation_time;

      _vm_api.prints = prints;
      _vm_api.prints_l = prints_l;
      _vm_api.printi = printi;
      _vm_api.printui = printui;
      _vm_api.printi128 = printi128;
      _vm_api.printui128 = printui128;
      _vm_api.printsf = printsf;
      _vm_api.printdf = printdf;
      _vm_api.printqf = printqf;
      _vm_api.printn = printn;
      _vm_api.printhex = printhex;

      _vm_api.set_resource_limits = set_resource_limits;
      _vm_api.get_resource_limits = get_resource_limits;
      _vm_api.set_proposed_producers = set_proposed_producers;
      _vm_api.is_privileged = is_privileged;
      _vm_api.set_privileged = set_privileged;
      _vm_api.set_blockchain_parameters_packed = set_blockchain_parameters_packed;
      _vm_api.get_blockchain_parameters_packed = get_blockchain_parameters_packed;
      _vm_api.activate_feature = activate_feature;

      _vm_api.eosio_abort = eosio_abort;
      _vm_api.eosio_assert = eosio_assert;
      _vm_api.eosio_assert_message = eosio_assert_message;
      _vm_api.eosio_assert_code = eosio_assert_code;
      _vm_api.eosio_exit = eosio_exit;
      _vm_api.current_time = current_time;
      _vm_api.now = now;

      _vm_api.checktime = checktime;
      _vm_api.check_context_free = check_context_free;

      _vm_api.send_deferred = _send_deferred;
      _vm_api.cancel_deferred = _cancel_deferred;
      _vm_api.read_transaction = read_transaction;
      _vm_api.transaction_size = transaction_size;

      _vm_api.tapos_block_num = tapos_block_num;
      _vm_api.tapos_block_prefix = tapos_block_prefix;
      _vm_api.expiration = expiration;
      _vm_api.get_action = get_action;

      _vm_api.assert_privileged = assert_privileged;
      _vm_api.assert_context_free = assert_context_free;
      _vm_api.get_context_free_data = get_context_free_data;



//      _vm_api.vm_call = vm_call;
//      _vm_api.wasm_call = wasm_call;
      _vm_api.evm_execute = evm_execute;

      // _vm_api.call_contract_get_extra_args = call_contract_get_extra_args;
      // _vm_api.call_contract_set_results = call_contract_set_results;
      // _vm_api.call_contract_get_results = call_contract_get_results;

      _vm_api.get_last_error = get_last_error;
      _vm_api.set_last_error = set_last_error;
      _vm_api.clear_last_error = clear_last_error;

      _vm_api.is_feature_activated = is_feature_activated;
      _vm_api.preactivate_feature = preactivate_feature;
      _vm_api.get_sender = get_sender;

      _vm_api.__ashlti3 = __ashlti3;
      _vm_api.__lshrti3 = __lshrti3;

      _vm_api.get_code_version = get_code_version;

      _vm_api.log = log_;
      _vm_api.is_in_apply_context = false;
      _vm_api.allow_access_apply_context = false;
      _vm_api.read_only = false;
   }
   vm_register_api(&_vm_api);
}

}}
