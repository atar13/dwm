#!/usr/bin/env bash
# changeVolume

# Arbitrary but unique message tag
msgTag="myvolume"

if [[ "$@" == "toggle" ]]; then
    pactl set-sink-mute $(pactl info | grep 'Default Sink' | cut -d' ' -f 3) "$@" > /dev/null
else
    pactl set-sink-volume $(pactl info | grep 'Default Sink' | cut -d' ' -f 3) "$@" > /dev/null
fi

# Query amixer for the current volume and whether or not the speaker is muted
volume="$(pactl list sinks | grep -A 7 "$(pactl info | grep 'Default Sink' | cut -d' ' -f 3)" | grep Volume | awk '{print $5}')"
mute="$(pactl list sinks | grep -A 7 "$(pactl info | grep 'Default Sink' | cut -d' ' -f 3)" | grep Mute | awk '{print $2}')"
if [[ $volume == 0 || "$mute" == "yes" ]]; then
    # Show the sound muted notification
    # dunstify -t 1000 -a "system" -u low -i audio-volume-muted-symbolic -h string:x-dunst-stack-tag:$msgTag "Volume muted" 
    dunstify -t 1000 -a "system" -u low -h string:x-dunst-stack-tag:$msgTag "Volume muted" 
else
    # Show the volume notification
    # dunstify -t 1000 -a "system" -u low -i audio-volume-high-symbolic -h string:x-dunst-stack-tag:$msgTag \
    dunstify -t 1000 -a "system" -u low -h string:x-dunst-stack-tag:$msgTag \
    -h int:value:"$volume" "Volume: ${volume}"
fi
