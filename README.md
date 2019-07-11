
# Install Dependencies

## Install Dependencies on Ubuntu

#### Install Rust

```bash
curl https://sh.rustup.rs -sSf | sh
```

#### Install golang
```bash
sudo apt install golang
```

#### Install Protobuf
```bash
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.6.1/protobuf-all-3.6.1.tar.gz
tar -xf protobuf-all-3.6.1.tar.gz
cd protobuf-3.6.1
./configure
make
make check
sudo make install
sudo ldconfig
```

## Install Dependencies on Mac OS X

#### Install Rust

```bash
curl https://sh.rustup.rs -sSf | sh
```

#### Install golang

```
brew install golang
```

#### Install protobuf

```
brew install protobuf
brew upgrade protobuf
```

# Build
```bash
git clone https://github.com/uuosio/libraos
cd libraos
git submodule update --init --recursive
./scripts/eosio_build.sh
```

# Running a local test node

### Configuring

Save the following text in config/config.ini

```
abi-serializer-max-time-ms = 15000
chain-state-db-size-mb = 1024
chain-state-db-guard-size-mb = 128
reversible-blocks-db-size-mb = 340
reversible-blocks-db-guard-size-mb = 2
signature-provider = EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV=KEY:5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

#producer-name = eosio

# Plugin(s) to enable, may be specified multiple times
# plugin = 
plugin = eosio::chain_plugin
plugin = eosio::chain_api_plugin
plugin = eosio::producer_plugin
plugin = eosio::history_plugin
plugin = eosio::history_api_plugin
#plugin = eosio::state_history_plugin
plugin = eosio::db_size_api_plugin
```

### Running

```
export LD_LIBRARY_PATH=$(rustc --print sysroot)/lib:$LD_LIBRARY_PATH
cd cd programs
./nodeos/nodeos --data-dir data --config-dir config  --contracts-console -e -p eosio --filter-on "*" --verbose-http-errors
```

### Publish move ir code on test node
Refer to [move-smart-contract-programming](https://github.com/uuosio/move-smart-contract-programming) for tutorials on writing smart contract in move ir and test it.

# Happy coding! Have fun with it!

