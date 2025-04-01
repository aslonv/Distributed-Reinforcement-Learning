# Distributed Reinforcement Learning System

This project implements a distributed reinforcement learning system using Deep Q-Networks (DQN) for enabling autonomous decision-making in complex environments.

## Overview

The system consists of a master node and multiple worker nodes. The master node coordinates communication and synchronization of Q-values among worker nodes, while each worker node interacts with the environment, collects experiences, and updates its local Q-network asynchronously.
