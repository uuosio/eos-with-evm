/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <fc/variant.hpp>

#include <eosio/http_plugin/http_plugin.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>

#include <appbase/application.hpp>

namespace eosio {

using namespace appbase;

struct call_contract_params {
   name code;
   name action;
   vector<char> binargs;
};

struct call_contract_results {
   fc::variant                 results;
};

class debug_api_plugin : public plugin<debug_api_plugin> {
public:
   APPBASE_PLUGIN_REQUIRES((http_plugin) (chain_plugin))

   debug_api_plugin() = default;
   debug_api_plugin(const debug_api_plugin&) = delete;
   debug_api_plugin(debug_api_plugin&&) = delete;
   debug_api_plugin& operator=(const debug_api_plugin&) = delete;
   debug_api_plugin& operator=(debug_api_plugin&&) = delete;
   virtual ~debug_api_plugin() override = default;

   virtual void set_program_options(options_description& cli, options_description& cfg) override {}
   void plugin_initialize(const variables_map& vm);
   void plugin_startup();
   void plugin_shutdown() {}

   call_contract_results call_contract(const call_contract_params& params) const;
   string call_contract_off_chain(uint64_t contract, uint64_t action, const vector<char>& binargs) const;

private:
   chain_plugin* chain_plug = nullptr;
};

}

FC_REFLECT( eosio::call_contract_params, (code)(action)(binargs) )
FC_REFLECT( eosio::call_contract_results, (results) )
