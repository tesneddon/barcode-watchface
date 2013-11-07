This directory contains the original GIMP XCF format files that the
PNG barcodes are built from.

I did consider fonts and line drawing to develop the barcodes.  However,
this seemed to be the quickest and easiest way to get the bars.  The files
are name ?.xcf and ../?.png, where ? is the decimal digit they represent.

The two marker sides of the barcode are called left.* and right.*

Each of the decimal digit bars is a 14x120 image.  Each "line" in the bar
is 2px wide.  The bars in the markers are also 2px wide.

To edit these, create more, etc. then create a 14x120...

Move this stuff into the wiki for this code.
