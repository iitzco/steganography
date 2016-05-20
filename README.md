# Steganography on WAV files

### How to use

To compile, run
```bash
$ make
```

The program receives the wav file, the steganographic method and the text to hide. It creates a new wav file (samples/dup.wav) that contains the hidden data. After that, it reads this last wav file in order to retrieve the hidden text. 

```bash
$ ./stegowav input_file lsb[1|4] text_to_hide
```

Where:
* input_file is a wav file where the text_to_hide will be hidden
* lsb1 or lsb4 is the steganographic method
* text_to_hide is the text that will be hidden in the wav file

For example:

```bash
$ ./stegowav samples/fun.wav lsb4 HI,im_a_hidden_text!
```

This should display:
```bash
Opening file...
Reading file...
Writing hidden info in samples/dup.wav...
Reading hidden info in samples/dup.wav...
Hidden info:
HI,im_a_hidden_text!
```

Note that if the text to hide is too large and the method is lsb4, samples/dup.wav will have noticeable noise.

To check WAV format specification, go to [Microsoft WAVE specs](http://soundfile.sapp.org/doc/WaveFormat/)

***

Work In Progress 🚜

## Collaborators

* Chervonagura, Ioji
* Bond, Federico
* Gutierrez, Julian
* Itzcovich, Ivan
