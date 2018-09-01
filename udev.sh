#!/bin/bash

echo 'KERNEL=="mydevice[0-9]*", GROUP="root", MODE="0666"' >> /etc/udev/rules.d/99-my.rules
