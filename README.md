HopfieldNetwork

This program implements the hoplfield network algorithm in C.
It can read and memorize a couple of patterns. It is then able to restore distorted patterns.
This means that if you input a slightly different pattern (couple inverted bits) the network will correct those errors.
I used asynchronous updates and an update threshold of 0.

You can use the implemented fill_input function to read in a maximum of 10 binary patterns.
Look at the example inputs to see how the bits are endcoded.
If you want to use pictures/patterns of different size (currently 20x10 pictures) please adjust the Macros at the top of the HopfieldNetwork.h code. Description is there.

