#pragma once
#include <stdlib.h>
#include <vector>
#include <array>
/*
#define ALIGNED(X) __attribute__ ((aligned (16))) X
struct ALIGNED(eth_address) {
    unsigned char data[20];
};
*/

typedef std::array<unsigned char, 20> eth_address;

typedef std::array<unsigned char, 32> key256;
typedef std::array<unsigned char, 32> value256;
// typedef std::vector<unsigned char> key256;
// typedef std::vector<unsigned char> value256;

bool eth_account_create(eth_address& address, int64_t ram_quota = 0);
bool eth_account_exists(eth_address& address);

uint64_t eth_account_get_info(eth_address& address, int32_t* nonce, int64_t* ram_quota, int64_t* amount);
bool eth_account_set_info(eth_address& address, int32_t nonce, int64_t ram_quota, int64_t amount);

int64_t eth_account_get_balance(eth_address& address);
bool eth_account_set_balance(eth_address& address, int64_t amount);


bool eth_account_get_code(eth_address& address, std::vector<unsigned char>& code);
bool eth_account_set_code(eth_address& address, const std::vector<unsigned char>& code);


bool eth_account_get_value(eth_address& address, key256& key, value256& value);
bool eth_account_set_value(eth_address& address, key256& key, value256& value);
bool eth_account_clear_value(eth_address& address, key256& key);


bool eth_account_get_nonce(eth_address& address, uint32_t& nonce);
bool eth_account_set_nonce(eth_address& address, uint32_t nonce);


void token_create( uint64_t issuer, int64_t maximum_supply, uint64_t sym);
void token_issue( uint64_t to, int64_t quantity, uint64_t sym, const char* memo, size_t size2 );
void token_transfer( uint64_t from, uint64_t to, int64_t quantity, uint64_t sym, const char* memo, size_t size2);
