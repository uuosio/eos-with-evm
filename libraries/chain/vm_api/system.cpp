/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

constexpr size_t max_assert_message = 1024;

void eosio_abort() {
   EOS_ASSERT( false, abort_called, "abort() called");
}

// Kept as intrinsic rather than implementing on WASM side (using eosio_assert_message and strlen) because strlen is faster on native side.
void eosio_assert( uint32_t condition, const char* msg ) {
   if( BOOST_UNLIKELY( !condition ) ) {
      const size_t sz = strnlen( msg, max_assert_message );
      std::string message( msg, sz );
      EOS_THROW( eosio_assert_message_exception, "assertion failure with message: ${s}", ("s",message) );
   }
}

void eosio_assert_message( uint32_t condition, const char* msg, uint32_t msg_len ) {
   if( BOOST_UNLIKELY( !condition ) ) {
      const size_t sz = msg_len > max_assert_message ? max_assert_message : msg_len;
      std::string message( msg, sz );
      EOS_THROW( eosio_assert_message_exception, "assertion failure with message: ${s}", ("s",message) );
   }
}

void  eosio_assert_code( uint32_t test, uint64_t error_code ) {
   if( BOOST_UNLIKELY( !test ) ) {
      edump((error_code));
      EOS_THROW( eosio_assert_code_exception,
                 "assertion failure with error code: ${error_code}", ("error_code", error_code) );
   }
}

void  eosio_exit( int32_t code ) {
   throw wasm_exit();
}

uint64_t  current_time() {
   return static_cast<uint64_t>( ctx().control.pending_block_time().time_since_epoch().count() );
}

uint32_t  now() {
   return (uint32_t)( current_time() / 1000000 );
}

static void checktime() {
   if (get_vm_api()->is_in_apply_context) {
      try {
         get_ctx_no_access_check().trx_context.checktime();
      } FC_LOG_AND_RETHROW();
   }
}

void check_context_free(bool context_free) {
   if( get_ctx_no_access_check().is_context_free() )
      EOS_ASSERT( context_free, unaccessible_api, "only context free api's can be used in this context" );
//   get_ctx_no_access_check().used_context_free_api |= !context_free;
}

bool contracts_console() {
   return ctx().control.contracts_console();
}

static std::vector<char> last_error;

void set_last_error(const char* error, size_t size) {
   last_error.resize(size);
   memcpy(last_error.data(), error, size);
}

size_t get_last_error(char* error, size_t size) {
   if (size == 0 || error == nullptr) {
      return last_error.size();
   }
   int copy_size = std::min(size, last_error.size());
   memcpy(error, last_error.data(), copy_size);
   return copy_size;
}

void clear_last_error() {
   last_error.clear();
}

