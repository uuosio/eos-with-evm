/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

uint32_t read_action_data( void* msg, uint32_t buffer_size ) {
   auto s = ctx().get_action().data.size();
   if( buffer_size == 0 || msg == NULL) return s;

   auto copy_size = std::min( (size_t)buffer_size, s );
   memcpy( msg, ctx().get_action().data.data(), copy_size );

   return copy_size;

}

uint32_t action_data_size() {
   return ctx().get_action().data.size();
}

void get_action_info(uint64_t* account, uint64_t* name) {
   *account = ctx().get_action().account.to_uint64_t();
   *name = ctx().get_action().name.to_uint64_t();
}

uint64_t current_receiver() {
   return ctx().get_receiver().to_uint64_t();
}

void set_action_return_value( const char* packed_blob, size_t datalen ) {
   ctx().action_return_value.assign( packed_blob, packed_blob + datalen );
}

void require_recipient( uint64_t account ) {
   ctx().require_recipient(name(account));
}

void require_auth( uint64_t account ) {
   ctx().require_authorization(name(account));
}

void require_auth2( uint64_t account, uint64_t permission ) {
   ctx().require_authorization(name(account), name(permission));
}

bool has_auth( uint64_t account ) {
   return ctx().has_authorization(name(account));
}

bool is_account( uint64_t account ) {
   return ctx().is_account(name(account));
}

void send_inline(const char *data, size_t data_len) {
   //TODO: Why is this limit even needed? And why is it not consistently checked on actions in input or deferred transactions
   EOS_ASSERT( data_len < ctx().control.get_global_properties().configuration.max_inline_action_size, inline_action_too_big,
              "inline action too big" );

   action act;
   fc::raw::unpack<action>(data, data_len, act);
   ctx().execute_inline(std::move(act));
}

void send_context_free_inline(const char *data, size_t data_len) {
   //TODO: Why is this limit even needed? And why is it not consistently checked on actions in input or deferred transactions
   EOS_ASSERT( data_len < ctx().control.get_global_properties().configuration.max_inline_action_size, inline_action_too_big,
             "inline action too big" );

   action act;
   fc::raw::unpack<action>(data, data_len, act);
   ctx().execute_context_free_inline(std::move(act));
}

uint64_t  publication_time() {
   return static_cast<uint64_t>( ctx().trx_context.published.time_since_epoch().count() );
}

bool is_feature_activated(const char *digest, size_t size) {
   digest_type feature_digest(digest, size);
   return ctx().control.is_protocol_feature_activated( feature_digest );
}

void preactivate_feature(const char *digest, size_t size) {
   digest_type feature_digest(digest, size);
   ctx().control.preactivate_feature( feature_digest );
}


uint64_t get_sender() {
   return ctx().get_sender().to_uint64_t();
}
