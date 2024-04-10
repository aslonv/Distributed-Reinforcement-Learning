# Distributed Reinforcement Learning System

This project implements a distributed reinforcement learning system using Deep Q-Networks (DQN) for enabling autonomous decision-making in complex environments.

## Overview

The system consists of a master node and multiple worker nodes. The master node coordinates communication and synchronization of Q-values among worker nodes, while each worker node interacts with the environment, collects experiences, and updates its local Q-network asynchronously.

## Dependencies

- C Compiler (e.g., gcc)
- Socket Programming Libraries

## Usage

1. Clone the repository:

```bash
git clone https://github.com/bekaliaslonov/distributed-reinforcement-learning.git
cd distributed-reinforcement-learning


## Compile the master and worker nodes:

gcc master.c -o master
gcc worker.c -o worker


## Run the master node:

./master

## Run the worker nodes (in separate terminals or on different machines):

./worker

## Configuration

Modify the code to specify the number of worker nodes and other parameters as needed.

## Documentation

master.c: Implementation of the master node.
worker.c: Implementation of the worker node.
