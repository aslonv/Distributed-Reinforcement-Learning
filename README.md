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

Compile the master and worker nodes:

gcc master.c -o master
gcc worker.c -o worker


Run the master node:

./master

Run the worker nodes (in separate terminals or on different machines):

./worker

Configuration
Modify the code to specify the number of worker nodes and other parameters as needed.
Documentation
master.c: Implementation of the master node.
worker.c: Implementation of the worker node.
LICENSE: MIT License for the project.
License
This project is licensed under the MIT License.

MIT License
MIT License

Copyright (c) [2024] [Bekali Aslonov]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.