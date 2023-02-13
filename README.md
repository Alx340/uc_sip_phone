# uc_sip_phone

Simple microcontroller based IP phone.
This is the educational project of the IP phone on the microcontroller.

# What is it?

This is one of my first projects when I started learning to program microcontrollers using a C.

The project is not completed, but works.
Initially, it was conceived as a training programming microcontrollers and training for developing different stacks of the internet protocols.
I tried to deal with various stacks with data transmission protocols ported to microcontrollers.
Further, the work was continued as "Proof of concept ", that it is really possible to make an IP-phone on a cheap microcontroller with 2 KB RAM memory.
It ended indeed a working prototype of the device.

Of course there is minimal functionality - to accept and make calls to the IP address.
Since the device has no jitter buffers, some artifacts slip.
However, they are compensated if you make calls to the PC. PC's jitter buffer compensates for jitter of device packets.

Perhaps someone will come in handy in the understanding of how the protocols work: TCP/IP, UDP, SIP, SDP, RTP, etc.


