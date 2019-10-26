# cryptobadge

## Version : 1.0.0

Folder structure

- `src` directory, the `cpp` files are there.
- `include` directory, the `hpp` files are there.
- `test` directory, test scripts are there.
- `ricardian` contains the Ricardian contract files.
- `.vscode` directory that contains *IntelliSense* definitions, task 
  definitions, etc.
- `test` directory, buil file and script.
- `example` directory, an example to deploy/run smart contract.
- `CMakeLists.txt` file, CMake definitions.

Dependencies:
* [eosio v1.8.x](https://github.com/EOSIO/eos/releases/tag/v1.8.0)
* [eosio.cdt v1.6.x](https://github.com/EOSIO/eosio.cdt/releases/tag/v1.6.1)

To build the contracts and the unit tests:
* First, ensure that your __eosio__ is compiled to the core symbol for the EOSIO blockchain that intend to deploy to.
* Second, make sure that you have ```sudo make install```ed __eosio__.
* Third, make sure that you have install CDT version > 1.6.x
* Then just go to each smart contract folder and following below instruction

## Prerequisites for Ubuntu & MacOS

* We assume you have installed [Python 3.5](https://www.python.org/downloads/release/python-356/) (or higher).
* We assume you have installed [pip3 18.1](https://pypi.org/project/pip/) (or higher).

## Install *EOSFactory* on Ubuntu & MacOS


Clone *EOSFactory* source code from the repository:

```bash
git clone https://github.com/tokenika/eosfactory.git
```

Open a bash terminal and navigate to the `eosfactory` folder:

```bash
cd eosfactory
```

Then run the `install` script by pointing it to the *EOSIO* source code:

```bash
./install.sh
```

## Build & Test

Open `build` folder in projects

```bash
cd /cryptobadge
cd build
```

To configure your project:

```bash
cmake ..
```

The output should look like this:

```bash
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/c/Workspaces/EOS/contracts/foo_bar/build
```

To build your project:

```bash
make
```

And finally, after your contract has been successfully built, run unit tests with this command:

```bash
cd ..
python3 tests/test1.py
```

An example using smart contract by cloes at `example` directory