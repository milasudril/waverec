waverec
=======

This is a simple CLI audio recorder using MuStudio

Dependencies
------------
* gabi
* waveio
* mustudio

Usage
-----

    waverec "ports[port_0;port_1;port_2] buffer_size[48000]"

This command will create three wav files called `port_0.wav`, `port_1.wav`, and
`port_2.wav`. Assuming the current sample rate is 48 kHz, it will use a one
second long buffer. Pressing CTRL+C will start recording. Pressing CTRL+C again
will stop recording.

