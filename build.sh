#!/bin/bash

set -xe

cc main.c -o main -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
