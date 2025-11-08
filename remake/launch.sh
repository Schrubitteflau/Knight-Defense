#!/bin/bash

gcc -Wall -o KD.exe src/*.c src/resource/*.c $(pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_primitives-5 --libs --cflags) -lm
./KD.exe

