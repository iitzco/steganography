# Steganography on WAV files

### How to use the parser

```bash
$ make
$ ./stegowav samples/fun.wav
```

This should display:
```bash
Opening  file..
RIFF HEADER
parsed -> RIFF
native ->  52 49 46 46

OVERALL SIZE HEADER
parsed -> 1958692
native ->  24 e3 1d 00

FORMAT HEADER
parsed -> WAVE
native ->  57 41 56 45

FMT HEADER
parsed -> fmt
native ->  66 6d 74 20

FORMAT CATEGORY (16 for PCM)
parsed -> 16
native ->  10 00 00 00

AUDIO FORMAT CATEGORY (1 for PCM)
parsed -> 1 (PCM)
native ->  01 00

AUDIO CHANNEL
parsed -> 1
native ->  01 00

AUDIO SAMPLE RATE
parsed -> 8000
native ->  40 1f 00 00

AUDIO BYTE RATE
parsed -> 16000
native ->  80 3e 00 00

BLOCK ALIGNMENT
parsed -> 2
native ->  02 00

BYTES PER SAMPLE
parsed -> 16
native ->  10 00

DATA HEADER
parsed -> data
native ->  64 61 74 61

DATA SIZE
parsed -> 1958656
native ->  00 e3 1d 00

EXTRA DATA
Number of samples:979328
Size of each sample:2 bytes
Approx.Duration in seconds=122.418251
Approx.Duration in h:m:s=0:2:2.418
```

To check WAV format specification, go to [Microsoft WAVE specs](http://soundfile.sapp.org/doc/WaveFormat/)

***

Work In Progress 🚜

## Collaborators

* Chervonagura, Ioji
* Bond, Federico
* Gutierrez, Julian
* Itzcovich, Ivan
