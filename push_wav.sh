#!/bin/sh
espeak -ven+f2 "$1" -w /tmp/espeak.wav
while true
do
	gst-launch-0.10 filesrc location=/tmp/espeak.wav ! wavparse ! audioconvert ! audioresample ! mulawenc ! rtppcmupay ! udpsink host=224.1.1.1 auto-multicast=true port=3000
	echo `cat /tmp/espeak_state.txt`
	if [ `cat /tmp/espeak_state.txt` = "stop" ];then
		break	
	fi
        sleep 3
done
