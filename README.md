# Setup pre requirements:

## Install eosin.cdt 1.7.0

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

## Install solidity

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
./script/eosio_build.sh

# Run Examples

## Start a local testnet

```
./nodeos/nodeos -p eosio -e --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::producer_api_plugin --wasm-runtime eos-vm-jit --contracts-console
```

## Clone solidity examples from github
```
git clone https://github.com/uuosio/solidity-smart-contract-programming
cd solidity-smart-contract-programming
```

## Setup Python environment

```
python3.7 -m pip install virtualenv
python3.7 -m virtualenv .venv
. .venv/bin/activate
```

### Install pyeoskit

#### Ubuntu

```
```

#### Mac OS X
```
python3.7 -m pip https://github.com/learnforpractice/pyeoskit/releases/download/v0.7.0/pyeoskit-0.7.0-cp37-cp37m-macosx_10_9_x86_64.whl
```

### Install Jupyter notebook
```
python3.7 -m pip install notebook
```

### Install solc compiler
```
python3.7 -m pip install py-solc-x
```

### Install web3

```
python3.7 -m pip install --pre web3[tester]
```


### Open jupyter notebook

```
python3.7 -m notebook
```

Open hello_evm.ipynb and run code in cell one by one

