# Setup Pre Requirements:

## Install eosio.cdt 1.7.0

### Ubuntu:

```
wget https://github.com/eosio/eosio.cdt/releases/download/v1.7.0/eosio.cdt_1.7.0-1-ubuntu-18.04_amd64.deb
sudo apt install ./eosio.cdt_1.7.0-1-ubuntu-18.04_amd64.deb
```

### Mac OS X:

```
brew tap eosio/eosio.cdt
brew install eosio.cdt
```

## Install Solidity

### Ubuntu 18.04:

```
sudo add-apt-repository ppa:ethereum/ethereum
sudo apt-get update
sudo apt-get install solc
```

### Mac OS X:

```
brew update
brew upgrade
brew tap ethereum/ethereum
brew install solidity
```

# Build

```
git clone --branch evm4eosio https://github.com/learnforpractice/uuos2 evm4eosio --recursive
cd evm4eosio
./script/eosio_build.sh
```

# Run Examples

## Start a Local Testnet

```
./nodeos/nodeos --http-server-address 127.0.0.1:8888 --p2p-listen-endpoint 127.0.0.1:8801 --verbose-http-errors --http-max-response-time-ms 50 --data-dir dd --config-dir cd --wasm-runtime eos-vm-jit --contracts-console -p eosio -e --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::contract_api_plugin 
```

## Clone Solidity Examples From Github
```
git clone https://github.com/uuosio/solidity-smart-contract-programming
cd solidity-smart-contract-programming
```

## Setup Python Environment

```
python3.7 -m pip install virtualenv
python3.7 -m virtualenv .venv
. .venv/bin/activate
```

### Install PyEosKit

#### Ubuntu

```
python3.7 -m pip https://github.com/learnforpractice/pyeoskit/releases/download/v0.7.0/pyeoskit-0.7.0-cp37-cp37m-linux_x86_64.whl
```

#### Mac OS X
```
python3.7 -m pip https://github.com/learnforpractice/pyeoskit/releases/download/v0.7.0/pyeoskit-0.7.0-cp37-cp37m-macosx_10_9_x86_64.whl
```

### Install Jupyter Notebook
```
python3.7 -m pip install notebook
```

### Install Solc Compiler
```
python3.7 -m pip install py-solc-x
```

### Install Web3

```
python3.7 -m pip install --pre web3[tester]
```


### Open Jupyter Notebook

```
python3.7 -m notebook
```

Open hello_evm.ipynb and run code in cell one by one

