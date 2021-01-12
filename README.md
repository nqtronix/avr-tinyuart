<h1 align="center" style="font-weight: bold; margin-top: 20px; margin-bottom: 20px;">avr-tinyuart</h1>

<h4 align="center">A software uart designed for tiny AVR MCUs.</h4>

<p align="center">
	<a href="#changelog"><img src="https://img.shields.io/github/release-pre/nqtronix/avr-tinyuart.svg" alt="release: NA"></a>
    <a href="#about"><img src="https://img.shields.io/badge/language-C_(GCC_5.4.0)-blue.svg" alt="language: C GCC (5.4.0)"></a>
    <a href="#about"><img src="https://img.shields.io/badge/platform-AVR8-blue.svg" alt="platform: AVR8"></a>
	<a href="#about"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a>
	<a href="https://github.com/nqtronix/avr-tinyuart/issues"><img src="https://img.shields.io/github/issues/nqtronix/avr-tinyuart.svg" alt="issues: NA"></a>
	<a href="#license"><img src="https://img.shields.io/github/license/nqtronix/avr-tinyuart.svg" alt="license: NA"></a>
</p>

<p align="center">
  <a href="#getting-started">Getting Started</a> •
  <a href="#under-the-hood">Under the Hood</a> •
  <a href="#support">Need Help?</a> •
  <a href="#about">About</a> •
  <a href="#credits-and-references">Credits</a>
</p>



## Introduction
It can be surprisingly difficult to debug the smallest MCUs of the AVR family - the basic programming interfaces and limited IO make it hard to get real-time information about what your code does.

**avr-tinyuart** provides a simple yet highly optimized uart tx function. It can be used to print any information to a terminal or to correlate the data with analog signals on an oscilloscope. The *tiny* footprint makes it suitable for almost any project.

<br>

## Key Features
 
 - **software only:** no UART, timer or interrupt required
 - **lightweight:** uint8_t tx function compiles to as little as 34 bytes flash
 - **flexible:** defines set baud rate, IO and options
 - **accurate:** bit delays accurate to ±0.5 cycles

<br>

## Limitations

 - The algorithm uses the PIN register to toggle the TX output efficiently. This feature is not available on some older AVRs.
 - The maximum bit-delay is  about 768 cycles. This limits the *maximum* cpu clock for a given baud rate: 7372800Hz for 9600baud; 14745600Hz for 19200baud
 - There is no receive funtion as it would be significantly more complex and was not needed at the time of writing.

<br>

## Usage Example
**avr-tinyuart** is trivial to use. Just call `tinyuart_send_uint8()` whenever you want so send some data.

```c
int main(void)
{
	// interrupts must be disbaled before calling the function
	cli();
	
	// send ASCII
	tinyuart_send_uint8('H');
	tinyuart_send_uint8('i');
	tinyuart_send_uint8('\r');
	tinyuart_send_uint8('\n');
	
	// send numbers
	tinyuart_send_uint8(0);
	tinyuart_send_uint8(0xff);
	
	// re-enable interrups (if needed by your code)
	sei();
	
    while (1);
}
```

<br>

## Getting Started
This section is written especially for everyone who is **not familiar** with the used tools. If you run into problems, please [ask for clarification](#get-help).<br>

### Step 0: Software
 - [**Atmel Studio 7.0**][tool-atmel-studio-7-0] (Build 1931) [free]<br>
   The installer contains all tools you need to open, edit, compile, simulate and flash this code. If you favor another development tool, feel free to use it instead. (But please understand that I can not provide any support).

### Step 1: Download avr-tinyuart
 - Clone this repository or hit [Download][git-download] and extract the .zip file.

### Step 2: Browse the project
 - **Open the project in Atmel Studio:**<br>
   Either double click `tinyuart.atsln` or open Atmel Studio and select "File -> Open -> Project/Solution..."

### Step 3: Run the demo
 - **Select your MCU & Programming tool:**<br>
   Press `ALT + F7`; select your MCU under `Device` and `Change Device...` and your tool under `Tool` and pick it from the drop down menu

 - **Adjust settings:**<br>
   Open `tinyuart.h` and adjust the settings as needed. By default it's configured for an ATTINY13A running at 9.6MHz and 115200 baud.

 - **Compile & Program:**<br>
   Press `CTRL + ALT + F5` to compile a new version for your settings and flash it afterwards.

<br>

## Under the Hood

In theory any software UART implementation is quite simple, just output the correct bit at the right time. To archive this, the following tricks are noteworthy:

1. The code uses a toggle pattern instead of the data, this way high and low outputs can be generated with ony one instruction. This makes the timing independent of the data being send.
2. The carry bit is used as a 9th bit to hold the stop-bit which is later accessed with the `ror` instruction. This ensures the timing of the stop-bit is identical and also saves a few bytes code.
3. The delays are calculated by the C preprocessor. It does not support floating point, so to round a value `(n*2+1)/2` is used.


<br>

## Support

### FAQ

- **Q: I've followed the getting started steps but the terminal shows only garbage! Why?**<br>
A: Because UART does not have a dedicated clock line, it requires a fairly accurate clock of both sender and receiver. The total acceptable error is ≈ ±4.5%, but the RC oscillator within most AVRs is only specified within ±10%. It can be calibrated, but it's easier to tweak `TINYUART_F_CPU` and let the code deal with the difference. UART to USB convertes typically use a crystal and thus have a low tolerance (±20ppm = ±0.002%); the delay code has an error of ±0.5 cycles or <0.6% with default settings.
  

### Get Help

**Something doesn't work as expected?** No worries! Just open up a new issue in the [GitHub issue tracker][git-issues]. Please provide all information to reproduce your problem. If you don't have a GitHub account (and can't be bothered to create one,) you can [contact](#contact) me directly.

<br>

### Contribute

**Spotted an error?** [Open an issue][git-issues] or submit a pull request.

There is no CONTRIBUTING.md yet, sorry. Contributions will inherit the [license](#license) of this project. If you have any questions, just ask.

<br>

## About
### Status
**This project is currently classified as** <a href="https://github.com/nqtronix/git-template/blob/master/badges.md#project-status"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a><br>
_The developers intend to keep the code in working condition by updating dependencies, fixing bugs and solving issues._

As my testing needs increase I will likely add the functionality I need.

<br>

### Changelog
This project uses [**Semantic Versioning 2.0.0**][semver.org]. During initial development (0.x.x versions) any _major_ increase is substituted with a _minor_ increase (0.1.0->0.2.0 instead of 0.1.0->1.0.0).

The message of each commit contains detailed information about the changes made. The list below is a summary about all significant improvements.

 - **0.1.2 (latest)** <br>
	- update readme
 - **0.1.1** <br>
	- combine `_init()` and `_send_uint8_t()` functions
 - **0.1.0** <br>
	- initial release

<br>

### Contact

If you haven't done so already, please check out [Get Help](#get-help) for the fastest possible help on your issue. Alternatively you can find my public email address on my [profile][git-profile].

<br>

## Credits and References

### Projects Used
- [**git-template**][git-repo-git-template] - _A simple and clean git repository template._<br>

<br>


## License
This project is proudly licensed under the [MIT license][git-license].

The MIT license was chosen to give you the freedom to use this project in any way you want, while protecting all contributors from legal claims. Good code works, great code works for everyone. If this code has become a part of one of your projects, a link back to us would be highly appreciated. Thanks!

<!-- Links -->

[git-readme]:README.md
[git-license]:LICENSE.md
[git-profile]:https://github.com/nqtronix
[git-issues]:https://github.com/nqtronix/unittrace/issues
[git-download]:https://github.com/nqtronix/unittrace/archive/master.zip

[git-repo-git-template]:https://github.com/nqtronix/git-template

[semver.org]:semver.org

[tool-atmel-studio-7-0]:https://www.microchip.com/mplab/avr-support/atmel-studio-7

