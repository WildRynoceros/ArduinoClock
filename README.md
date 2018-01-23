# RTC_DS1307
## Synopsis
This is my version of a DS1307 and accompanying time library for Arduino. It is able to do timekeeping, but it's also able to use the control register and the square-wave oscillator. The time library that accompanies it is designed to work within the DS1307's time limitations so the DS1307 is put into an undefined state.

## Motivation
Other DS1307 libaries I've found to be either not working or not as fully featured as I hoped it would be. Additionally, I didn't want to rely on needing to read from the DS1307 each second, since the operation was expensive for the Arduino. So, I wanted to have the DS1307 be a timekeeper that I only read from out of necessity (for example, if the sketch restarts), and have the Arduino update the time internally in step with the DS1307 using the DS1307's oscillator. This trades needing to do I2C communication each second for memory, freeing up the Arduino to do other operations.

## Installation
Just download this folder and install it into your Arduino's libraries folder.

## Reference
Read more about it on the [wiki](https://github.com/WildRynoceros/RTC_DS1307/wiki).

## Contributors
This library's core functionality was from JeeLabs original code from [here](http://news.jeelabs.org/code/), but the extended functionality and the RTC_Time class was created by me, WildRynoceros.

## Future Updates
First, I'm hoping to do some verification of the code as well as initial cleanup.

After ensuring the core functions and the accompanying time library fully work, assuming accessing this memory would not do anything too jarring, I hope to write code that enables access to the rest of the DS1307's 56x8 RAM.
