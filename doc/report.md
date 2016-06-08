% Primer
Pablo Iojanan Chervonagura, Federico Bond, Julian Gutierrez, Ivan Itzcovich
% June 6, 2016

# Abstract

`stegowav` is an implementatino of steganographic methods for WAVE files. It
supports several steganography modes and optional file encryption. It was
developed during the introductory Cryptography and Security course at ITBA.

# Introduction

Steganography is the practice of concealing a file within another file.

 - background on steganography
 - current implementations

# Development

We tried to follow standard C conventions for the structure of the project and
tools. The project is compiled with `make` and integration tests are run with
`make test`.

# Design choices

 - temporary files used during encryption
 - why not start in another location?
 - comparison of different steganographic algorithms
 - why not put extension at the beginning of the file

# Commentary about LSBE

 - paper (lack of) quality
 - not implemented: "powerful encryption", custom padding

# Encryption Capabilities

Our `stegowav` program supports AES128, AES192, AES256 and DES encryption
algorithms in ECB, CFB, OFB and CBC encryption modes. No authenticated
modes have been implemented.

The encryption keys are derived using MD5 with the `EVP_BytesToKey` function
from the OpenSSL library, as described in the course material. A better
approach would be to use a modern key derivation algorithm like PBKDF2 with
a reasonable amount of iterations.

# Stegoanalysis

 - detailed description of what is hidden inside each file

# Future extensions

As we noted previously, supporting authenticated encryption modes and a
more modern key derivation algorithm would be a welcome addition to the
current set of features.

# Conclusion

During the course of this this project, we learned about steganographic
techniques in practice and how to use the capabilities of the OpenSSL
library, one of the most used libraries for cryptography in real-world
projects.
