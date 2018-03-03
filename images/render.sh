#!/bin/bash
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+0+0    +repage images/roulette.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+64+0   +repage images/loto.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+128+0  +repage images/8ball.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+192+0  +repage images/coin.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+256+0  +repage images/sandtimer.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+320+0  +repage images/videau.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+384+0  +repage images/space.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+448+0  +repage images/cards.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+0+64   +repage images/dice6.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+64+64  +repage images/dice8.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+128+64 +repage images/dice20.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+192+64 +repage images/dice2d6.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+256+64 +repage images/dice4df.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+320+64 +repage images/counter.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+384+64 +repage images/custom.png
convert -render -density 95 -verbose -background transparent images/icons.svg -crop 32x32+448+64 +repage images/dice5dp.png

convert -render -density 95 -verbose -background transparent images/tatoge.svg -define icon:auto-resize=64,48,32,16 package/windows/tatoge.ico
