# refactored-puzzles

## What is this project?
This project was originally started as an honors [research paper](https://drive.google.com/file/d/1jtvM6UVhxvXgecHfRE_qrmjUVxQiOerw/view?usp=drive_link) that was written to enhance the Client puzzle protocol [CPP](https://en.wikipedia.org/wiki/Client_Puzzle_Protocol).

The CPP is a protocol that is made to sit on another protocol or infrastructure that is vulnerable to DDoS attacks.
The goal of this protocol is not preventative but mitigative in nature, that is, it is engineered to slow down attackers by introducing some overhead for attackers
so that the proceeding protocol/infrastructure is not overwhelmed.

The goal os this project is to create an improvement to the initial proposal of the CPP. We focus on some of the areas in the initial proposal that we believe could
vulnerabilities for an attack. In essence, this project is an attempt to optimize the initial proposal. 
The areas of optimizations we propose are:
- Puzzle sourcing
- Puzzle distribution
- Puzzle verification

## Project Details
The GitHub repo features implementations of the Initial proposal and our proposal.
The implementations are simple client-server architectures that show how the areas of opimization play out and also allow us 
to collect statistical data for comparisons. We have to versisons of our proposal:
- The first version was used in an undergraduate honors paper, where we used the GRAIL library to leverage a Finite State Machines as data structure for storing, retrieving and verifying puzzles.
- The second version we simplified the storage, retrieval and verification of puzzles by using random access techniques on a file with a group of puzzles.

There are a few nuances and details that we will be explain in great detail in a future paper. Stay tuned for it.

##Getting Started
Running the program:
- run `make all` within any of the proposal directories.
- run `./server` then run `./client`

You may need to download this library on your system before compiling
$ sudo apt-get install libssl-dev


## Resources
- The presentation used during SCIENCE ATLANTIC 2021 can be found  [here](https://1drv.ms/p/s!As0IVKR1e3k7jKFQUiEyq63PKwBH9g?e=pI7c4P)
