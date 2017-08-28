# vineyard #

A work-in-progress hobby OS for the PC.

## Building ##
vineyard uses make as its build tool. In order to compile the kernel, run `make all`. This will download and compile all necessary tools on its first run, which takes about 30 minutes on my machine, as well as actually build the kernel binary.

If you want to clean the tree from object files, run `make clean`.

## Running ##
The build process generates a CD image, which is used by QEMU to boot the kernel. Running the emulator is as easy as running `make test`.

Alternatively, you can run `make test-virtualbox` in order to run it on VirtualBox.

## License ##
vineyard is licensed under the terms of the MIT (Expat) License. For details, see `LICENSE.md`.

## Third-Party Components ##

### uni-vga ###

[`uni-vga`](http://www.inp.nsk.su/~bolkhov/files/fonts/univga/) is a UTF-8 VGA font in the Adobe BDF format. It has been converted into the C file at `kernel/gfx/font.c`

> Copyright © 2001 by Dmitry Yu. Bolkhovityanov
>
> The UNI-VGA font can be distributed and modified freely, according to the X license.
>
> The Basic Latin block was taken almost unchanged from DosEmu's vga.bdf. All the other blocks (except noted) were created mainly from scratch by me, Dmitry Bolkhovityanov.
>
> Letters in the Hebrew block were taken unchanged from a public domain hebrew console font.
>
> Glyphs in Arabic, Arabic Presentation Forms-A, Arabic Presentation Forms-B and U+262B Farsi symbol were kindly donated by Behdad Esfahbod.
>
> Thanks to Birger Langkjer for idea, to Mark Leisher for his wonderful XmBDFEd, and to many others for their support.

### dlmalloc ###
[`dlmalloc`](http://g.oswego.edu/dl/html/malloc.html) is a widely-used memory allocator written by Doug Lea. It is located at `kernel/mm/dlmalloc.c` and `include/mm/dlmalloc.h`. It is licensed under the Creative Commons CC0 1.0 Universal:

> This is a version (aka dlmalloc) of malloc/free/realloc written by
> Doug Lea and released to the public domain, as explained at
> http://creativecommons.org/publicdomain/zero/1.0/ Send questions,
> comments, complaints, performance data, etc to dl@cs.oswego.edu
>
> * Version 2.8.6 Wed Aug 29 06:57:58 2012  Doug Lea
>  Note: There may be an updated version of this malloc obtainable at
>		 ftp://gee.cs.oswego.edu/pub/misc/malloc.c
>	   Check before installing!
