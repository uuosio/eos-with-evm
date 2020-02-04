#include <eosio/action.hpp>
void check_result(bool b, const char *msg) {
    eosio::check(b, msg);
}