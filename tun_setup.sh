#!/bin/bash
sudo ip addr add 192.168.1.1 dev turtle
sudo ip link set dev turtle up
sudo ip route add 192.168.1.0/24 dev turtle
