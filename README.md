# Steganography on WAV files

### How to use

To compile, run
```bash
$ make
```

#### To embed

The program receives the carrier wav file, the steganographic method, the text to hide and the output wav file. 
For example:

```bash
$ ./stegowav --in samples/avatar.png --p samples/fun.wav --out samples/dup.wav --steg LSB4 --embed
```
#### To extract

The program receives the carrier wav file, the steganographic method and the output file. 
For example:

```bash
$ ./stegowav --p samples/dup.wav --out samples/output --steg LSB4 --extract
```
#### Important

To check if the input file that was embed and the output file that was extracted are equal, you can run:

```bash
cmp --silent file1 file2 || echo "files are different"
```

Note that if the data to hide is too large and the method is lsb4, the output wav file will have noticeable noise.

To check WAV format specification, go to [Microsoft WAVE specs](http://soundfile.sapp.org/doc/WaveFormat/)

***

Work In Progress 🚜

## Collaborators

* Chervonagura, Ioji
* Bond, Federico
* Gutierrez, Julian
* Itzcovich, Ivan
