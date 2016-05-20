# Steganography on WAV files

### How to use

To compile, run
```bash
$ make
```

The program receives the carrier wav file, the steganographic method, the text to hide and the output wav file. For examples:

```bash
$ ./stegowav --in samples/file_to_hide.txt --p samples/fun.wav --out samples/dup.wav --steg LSB1
```

This should display:
```bash
Opening file samples/fun.wav...
Reading file...
Writing hidden info in samples/dup.wav...
Reading hidden info from samples/dup.wav...

Hidden info:
This is an example file.
```

Note that if the text to hide is too large and the method is lsb4, the output wav file will have noticeable noise.

To check WAV format specification, go to [Microsoft WAVE specs](http://soundfile.sapp.org/doc/WaveFormat/)

***

Work In Progress 🚜

## Collaborators

* Chervonagura, Ioji
* Bond, Federico
* Gutierrez, Julian
* Itzcovich, Ivan
