#!/bin/bash
make
./game $1 $2 $3 $4
convert -filter point -resize 300%x300% -delay 20 gol_*.pbm gol.gif
code gol.gif