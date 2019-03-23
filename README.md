ShakeYourHead
=====================

A little device used as artifact during encounter [game](http://vilnius.en.cx/GameDetails.aspx?gid=64323) which allows to see some text when rapidly shaking your head.

![](https://github.com/jauler/EN592-ShakeYourHead/raw/master/img/image1.jpg "Photo of the artifact")

![](https://github.com/jauler/EN592-ShakeYourHead/raw/master/img/image2.jpg "Photo of the artifact")


### Building

Assuming arm-none-eabi-gcc cross-compiler is installed and working - Just issue:

```
$ make
```

This assumes that make tools and toolchain is already installed and working.

If `st-flash` utility is installed and working, board can be flashed with
```
$ make flash
```

