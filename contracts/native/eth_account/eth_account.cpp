#include "eth_account.hpp"

#include <eosio/print.hpp>
#include <eosio/name.hpp>
#include <eosio/action.hpp>

#include <eosio/singleton.hpp>

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/fixed_bytes.hpp>

#include <eosio/system.hpp>

using namespace eosio;

extern "C" {
    __attribute__((eosio_wasm_import))
    uint32_t db_get_table_count(uint64_t a, uint64_t b, uint64_t c);

    __attribute__((eosio_wasm_import))
    int32_t db_store_i256( uint64_t scope, uint64_t table, uint64_t payer, void* id, int size, const char* buffer, size_t buffer_size );

    __attribute__((eosio_wasm_import))
    void db_update_i256( int iterator, uint64_t payer, const char* buffer, size_t buffer_size );

    __attribute__((eosio_wasm_import))
    void db_remove_i256( int iterator );

    __attribute__((eosio_wasm_import))
    int32_t db_get_i256( int iterator, char* buffer, size_t buffer_size );

    __attribute__((eosio_wasm_import))
    int32_t db_find_i256( uint64_t code, uint64_t scope, uint64_t table, void* id, size_t size );

    __attribute__((eosio_wasm_import))
    int db_previous_i256( int itr, void* primary, size_t id_size );

    __attribute__((eosio_wasm_import))
    int db_next_i256( int itr, void* primary, size_t id_size );

    __attribute__((eosio_wasm_import))
    int db_lowerbound_i256( uint64_t code, uint64_t scope, uint64_t table, void* id, size_t id_size );

    __attribute__((eosio_wasm_import))
    int db_upperbound_i256( uint64_t code, uint64_t scope, uint64_t table, void* id, size_t id_size );
}

#include <string>
#include <algorithm>
#include <iterator>

struct [[eosio::table]] ethaccount {
    uint64_t                        index;
    int64_t                         ram_quota;
    int32_t                         nonce;
    uint64_t                        version;
    std::array<unsigned char, 20>   address;
    asset                           balance;
    std::vector<unsigned char>      code;
    checksum256                     code_hash;

    uint64_t primary_key() const { return index; }

    checksum256 get_secondary() const {
       auto ret = checksum256();//address;
       memset(ret.data(), 0, sizeof(checksum256));
       memcpy(ret.data(), address.data(), 20);
       return ret;
    }
    EOSLIB_SERIALIZE( ethaccount, (index)(nonce)(ram_quota)(address)(balance)(code) )
};

struct [[eosio::table]] accountcounter {
    uint64_t                        count;
    EOSLIB_SERIALIZE( accountcounter, (count) )
};

typedef eosio::singleton< "global"_n, accountcounter >   account_counter;


typedef multi_index<"ethaccount"_n,
                ethaccount,
                indexed_by< "bysecondary"_n,
                const_mem_fun<ethaccount, checksum256, &ethaccount::get_secondary> > > ethaccount_table;

bool eth_account_create(eth_address& address, int64_t ram_quota) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    account_counter counter(name(code), scope);

    ethaccount_table mytable( name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();
/*
    uint32_t index = db_get_table_count(code, scope, table);
    index /= 2;
    index += 1;
*/
    auto idx = idx_sec.find(_address);
    if (idx == idx_sec.end()) {
        uint64_t index;
        accountcounter a = {0};
        a = counter.get_or_default(a);
        a.count += 1;
        index = a.count;
        counter.set(a, name(payer));

        mytable.emplace( name(payer), [&]( auto& row ) {
            asset a(0, symbol("EOS", 4));

            row.balance = a;
            memcpy(row.address.data(), address.data(), 20);
            row.index = index;
            row.nonce = 0;
            row.ram_quota = 0;
        });
        return true;
    } else {
        return false;
        /*
        mytable.modify( itr, payer, [&]( auto& row ) {
            index = row.index + 1;
            row.index = index;
        });
        */
    }
}

bool eth_account_exists(eth_address& address) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    ethaccount_table mytable(name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();
    
    auto idx = idx_sec.find(_address);
    if (idx == idx_sec.end()) {
        return false;
    }
    return true;
}

uint64_t eth_account_get_info(eth_address& address, int32_t* nonce, int64_t* ram_quota, int64_t* amount) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    ethaccount_table mytable(name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto idx = idx_sec.find(_address);
    if (idx == idx_sec.end()) {
        return 0;
    }
    if (nonce) {
        *nonce = idx->nonce;
    }
    if (ram_quota) {
        *ram_quota = idx->ram_quota;
    }
    if (amount) {
        *amount = idx->balance.amount;
    }
    return idx->index;;
}

bool eth_account_set_info(eth_address& address, int32_t nonce, int64_t ram_quota, int64_t amount) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    account_counter counter(name(code), scope);
    ethaccount_table mytable( name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto itr = idx_sec.find(_address);

    if (itr == idx_sec.end()) {
        uint64_t index;
        /*
        index = db_get_table_count(code, scope, table);
        index /= 2;
        index += 1;
        */
        accountcounter a = {0};
        a = counter.get_or_default(a);
        a.count += 1;
        index = a.count;
        counter.set(a, name(payer));

        mytable.emplace( name(payer), [&]( auto& row ) {
            row.nonce = nonce;
            row.ram_quota = ram_quota;
            asset a(amount, symbol("EOS", 4));
            row.balance = a;
            row.index = index;
            memcpy(row.address.data(), address.data(), 20);
        });
    } else {
        auto itr2 = mytable.find(itr->index);
        mytable.modify( itr2, name(payer), [&]( auto& row ) {
            row.nonce = nonce;
            row.ram_quota = ram_quota;
            row.balance.amount = amount;
        });
    }
    return true;
}

bool eth_account_get(eth_address& address, ethaccount& account) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    ethaccount_table mytable(name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto idx = idx_sec.find(_address);
    if (idx == idx_sec.end()) {
        return false;
    }
    account = *idx;
    return true;
}

bool eth_account_set(eth_address& address, const ethaccount& account) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    account_counter counter(name(code), scope);
    ethaccount_table mytable( name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto itr = idx_sec.find(_address);
    if (itr == idx_sec.end()) {
        uint64_t index;
        /*
        index = db_get_table_count(code, scope, table);
        index /= 2;
        index += 1;
        */
        accountcounter a = {0};
        a = counter.get_or_default(a);
        a.count += 1;
        index = a.count;
        counter.set(a, name(payer));

        mytable.emplace( name(payer), [&]( auto& row ) {
            row = account;
            row.index = index;
        });
    } else {
        auto itr2 = mytable.find(itr->index);
        mytable.modify( itr2, name(payer), [&]( auto& row ) {
            uint64_t index = row.index;
            row = account;
            row.index = index;
        });
    }
    return true;
}

int64_t eth_account_get_balance(eth_address& address) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    ethaccount_table mytable(name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto idx = idx_sec.find(_address);
    if (idx == idx_sec.end()) {
        return 0;
    }
    return idx->balance.amount;
}

bool eth_account_set_balance(eth_address& address, int64_t amount) {
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t receiver = current_receiver().value;
    uint64_t table = "ethaccount"_n.value;

    uint64_t payer = current_receiver().value;

    checksum256 _address;
    memset(_address.data(), 0, sizeof(checksum256));
    memcpy(_address.data(), address.data(), 20);

    ethaccount_table mytable( name(code), scope);
    auto idx_sec = mytable.get_index<"bysecondary"_n>();

    auto itr = idx_sec.find(_address);
    if (itr == idx_sec.end()) {
        return false;
    } else {
        auto itr2 = mytable.find(itr->index);
        mytable.modify( itr2, name(payer), [&]( auto& row ) {
            row.balance.amount = amount;
        });
    }
    return true;
}

bool eth_account_get_code(eth_address& address, std::vector<unsigned char>& code) {
    ethaccount account;
    if (!eth_account_get(address, account)) {
        return false;
    }
    code = account.code;
    return true;
}

bool eth_account_set_code(eth_address& address, const std::vector<unsigned char>& code) {
    ethaccount account;
    if (!eth_account_get(address, account)) {
        return 0;
    }
    account.code = code;
    return eth_account_set(address, account);
}

bool eth_account_get_nonce(eth_address& address, uint32_t& nonce) {
    ethaccount account;
    if (!eth_account_get(address, account)) {
        return false;
    }
    nonce = account.nonce;
    return true;
}

bool eth_account_set_nonce(eth_address& address, uint32_t nonce) {
    ethaccount account;
    if (!eth_account_get(address, account)) {
        return 0;
    }
    account.nonce = nonce;
    return eth_account_set(address, account);
}



uint64_t eth_account_get_index(eth_address& address) {
    return eth_account_get_info(address, nullptr, nullptr, nullptr);
}

bool eth_account_get_value(eth_address& address, key256& key, value256& value) {
    uint64_t index = eth_account_get_index(address);
    if (index == 0) {
        return false;
    }
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    int itr = db_find_i256(code, scope, index, key.data(), 32);
    if (itr < 0) {
        return false;
    }

    int size = db_get_i256(itr, (char*)value.data(), 32);
    check(size == 32, "bad storage!");
    return true;
}

bool eth_account_set_value(eth_address& address, key256& key, value256& value) {
    uint64_t index = eth_account_get_index(address);
    if (index == 0) {
        return false;
    }
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t payer = code;

    int itr = db_find_i256(code, scope, index, key.data(), 32);
    if (itr < 0) {
        db_store_i256(scope, index, payer, (char*)key.data(), 32, (char*)value.data(), 32);
    } else {
        db_update_i256(itr, payer, (char*)value.data(), 32);
    }
    return true;
}


bool eth_account_clear_value(eth_address& address, key256& key) {
    uint64_t index = eth_account_get_index(address);
    if (index == 0) {
        return false;
    }
    uint64_t code = current_receiver().value;
    uint64_t scope = code;
    uint64_t payer = code;

    int itr = db_find_i256(code, scope, index, key.data(), 32);
    if (itr < 0) {
        return false;
    } else {
        db_remove_i256(itr);
        return true;
    }
}
