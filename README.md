# Steganography on WAV files

## How to use

To compile, run
```bash
$ make
```

To clean, run
```bash
$ make clean
```

To run tests, run
```bash
$ make test
```

#### To embed

It supports hiding a plaintext file or a ciphertext file.

##### Plaintext file

The program receives the **--embed / -e** option and the following parameters:

* file to hide with **--in / -i**
* steganographic method with **--steg / -s**. Possible values: &lt;LSB1|LSB4|LSBE&gt;
* carrier file with **--p / -p**
* output wav file with **--out / -o** (this file will be the carrier that contains the file to hide)

For example:

```bash
$ ./stegowav --in samples/avatar.png --p samples/fun.wav --out samples/dup.wav --steg LSB4 --embed
```
or, what is the same:
```bash
$ ./stegowav -i samples/avatar.png -p samples/fun.wav -o samples/dup.wav -s LSB4 -e
```

##### Ciphertext file

The program receives the **--embed / -e** option and the following parameters:

* file to hide with **--in / -i**
* steganographic method with **--steg / -s**. Possible values: &lt;LSB1|LSB4|LSBE&gt;
* carrier file with **--p / -p**
* output wav file with **--out / -o** (this file will be the carrier that contains the file to hide)
* encryption password with **--pass**
* [OPTIONAL] encryption algorithm with **-a**. Possible values:  &lt;aes128|aes192|aes256|des&gt; (default set to aes128)
* [OPTIONAL] ecnryption cipher mode with **-m**. Possible values: &lt;ecb|cfb|ofb|cbc&gt; (default set to cbc)

For example:

```bash
$ ./stegowav  --in samples/avatar.png --p samples/fun.wav --out samples/dup.wav --steg LSB4 --embed -a "aes192" -m cfb --pass "thePassword"
```

*Use of quotes are optional*

#### To extract

##### Plaintext file

The program receives the **--extract / -x** option and the following parameters:

* steganographic method with **--steg / -s**. Possible values: &lt;LSB1|LSB4|LSBE&gt;
* carrier file with **--p / -p**
* output file with **--out / -o** (No need for extension, it is added automatically)

For example:

```bash
$  ./stegowav --p samples/dup.wav --out out/output --steg LSB1 --extract
```

or, what is the same:

```bash
$  ./stegowav -p samples/dup.wav -o out/output -s LSB1 -x
```

##### Ciphertext file

The program receives the **--extract / -x** option and the following parameters:

* steganographic method with **--steg / -s**. Possible values: &lt;LSB1|LSB4|LSBE&gt;
* carrier file with **--p / -p**
* output file with **--out / -o** (No need for extension, it is added automatically)
* encryption password with **--pass**
* encryption algorithm with **-a**. Possible values:  &lt;aes128|aes192|aes256|des&gt;
* ecnryption cipher mode with **-m**. Possible values: &lt;ecb|cfb|ofb|cbc&gt;

```bash
$ ./stegowav --p samples/dup.wav --out samples/output --steg LSB4 --extract -a "aes192" -m cfb --pass "thePassword"
```

*Use of quotes are optional*

#### Options

For more details, run:

```bash
$ ./stegowav --help
```

#### Important

To check if the input file that was embed and the output file that was extracted are equal, you can run:

```bash
cmp --silent file1 file2 || echo "files are different"
```

To check WAV format specification, go to [Microsoft WAVE specs](http://soundfile.sapp.org/doc/WaveFormat/)

***

## Collaborators

* Chervonagura, Ioji
* Bond, Federico
* Gutierrez, Julian
* Itzcovich, Ivan
