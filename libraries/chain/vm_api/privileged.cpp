void set_resource_limits( uint64_t account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight ) {
   EOS_ASSERT(ram_bytes >= -1, wasm_execution_error, "invalid value for ram resource limit expected [-1,INT64_MAX]");
   EOS_ASSERT(net_weight >= -1, wasm_execution_error, "invalid value for net resource weight expected [-1,INT64_MAX]");
   EOS_ASSERT(cpu_weight >= -1, wasm_execution_error, "invalid value for cpu resource weight expected [-1,INT64_MAX]");
   if( ctx().control.get_mutable_resource_limits_manager().set_account_limits(name(account), ram_bytes, net_weight, cpu_weight) ) {
      ctx().trx_context.validate_ram_usage.insert( name(account) );
   }
}

void get_resource_limits( uint64_t account, int64_t* ram_bytes, int64_t* net_weight, int64_t* cpu_weight ) {
   ctx().control.get_resource_limits_manager().get_account_limits( name(account), *ram_bytes, *net_weight, *cpu_weight);
}

int64_t set_proposed_producers_common( vector<producer_authority> && producers, bool validate_keys ) {
   EOS_ASSERT(producers.size() <= config::max_producers, wasm_execution_error, "Producer schedule exceeds the maximum producer count for this chain");
   EOS_ASSERT( producers.size() > 0
               || !ctx().control.is_builtin_activated( builtin_protocol_feature_t::disallow_empty_producer_schedule ),
               wasm_execution_error,
               "Producer schedule cannot be empty"
   );

   const auto num_supported_key_types = ctx().db.get<protocol_state_object>().num_supported_key_types;

   // check that producers are unique
   std::set<account_name> unique_producers;
   for (const auto& p: producers) {
      EOS_ASSERT( ctx().is_account(p.producer_name), wasm_execution_error, "producer schedule includes a nonexisting account" );

      p.authority.visit([&p, num_supported_key_types, validate_keys](const auto& a) {
         uint32_t sum_weights = 0;
         std::set<public_key_type> unique_keys;
         for (const auto& kw: a.keys ) {
            EOS_ASSERT( kw.key.which() < num_supported_key_types, unactivated_key_type,
                        "Unactivated key type used in proposed producer schedule");

            if( validate_keys ) {
               EOS_ASSERT( kw.key.valid(), wasm_execution_error, "producer schedule includes an invalid key" );
            }

            if (std::numeric_limits<uint32_t>::max() - sum_weights <= kw.weight) {
               sum_weights = std::numeric_limits<uint32_t>::max();
            } else {
               sum_weights += kw.weight;
            }

            unique_keys.insert(kw.key);
         }

         EOS_ASSERT( a.keys.size() == unique_keys.size(), wasm_execution_error, "producer schedule includes a duplicated key for ${account}", ("account", p.producer_name));
         EOS_ASSERT( a.threshold > 0, wasm_execution_error, "producer schedule includes an authority with a threshold of 0 for ${account}", ("account", p.producer_name));
         EOS_ASSERT( sum_weights >= a.threshold, wasm_execution_error, "producer schedule includes an unsatisfiable authority for ${account}", ("account", p.producer_name));
      });


      unique_producers.insert(p.producer_name);
   }
   EOS_ASSERT( producers.size() == unique_producers.size(), wasm_execution_error, "duplicate producer name in producer schedule" );

   return ctx().control.set_proposed_producers( std::move(producers) );
}

int64_t set_proposed_producers( char* packed_producer_schedule, uint32_t datalen ) {
   datastream<const char*> ds( packed_producer_schedule, datalen );
   vector<producer_authority> producers;

   vector<legacy::producer_key> old_version;
   fc::raw::unpack(ds, old_version);

   /*
      * Up-convert the producers
      */
   for ( const auto& p: old_version ) {
      producers.emplace_back(producer_authority{ p.producer_name, block_signing_authority_v0{ 1, {{p.block_signing_key, 1}} } } );
   }

   return set_proposed_producers_common(std::move(producers), true);
}

bool is_privileged( uint64_t account )  {
   return ctx().db.get<account_metadata_object, by_name>( name(account) ).is_privileged();
}

void set_privileged( uint64_t n, bool is_priv ) {
   const auto& a = ctx().db.get<account_metadata_object, by_name>( name(n) );
   ctx().db.modify( a, [&]( auto& ma ){
      ma.set_privileged( is_priv );
   });
}

void set_blockchain_parameters_packed(char* packed_blockchain_parameters, uint32_t datalen) {
   datastream<const char*> ds( packed_blockchain_parameters, datalen );
   chain::chain_config cfg;
   fc::raw::unpack(ds, cfg);
   cfg.validate();
   ctx().db.modify( ctx().control.get_global_properties(),
      [&]( auto& gprops ) {
           gprops.configuration = cfg;
   });

}

uint32_t get_blockchain_parameters_packed(char* packed_blockchain_parameters, uint32_t buffer_size) {
   auto& gpo = ctx().control.get_global_properties();

   auto s = fc::raw::pack_size( gpo.configuration );
   if( buffer_size == 0 ) return s;

   if ( s <= buffer_size ) {
      datastream<char*> ds( packed_blockchain_parameters, s );
      fc::raw::pack(ds, gpo.configuration);
      return s;
   }
   return 0;
}

void activate_feature( int64_t f ) {
   EOS_ASSERT( false, unsupported_feature, "Unsupported Hardfork Detected" );
}
