#!/usr/bin/env bash

# Arbitrary but unique message tag
msgTag="system"

if [[ "$1" == "up" ]]; then
  light -A "$2"
else 
  light -U "$2"
fi

# Query amixer for the current volume and whether or not the speaker is muted
brightness="$(light)"
  # dunstify -t 1000 -a $msgTag -u low -i audio-volume-high-symbolic -h string:x-dunst-stack-tag:$msgTag \
  dunstify -t 1000 -a $msgTag -u low -h string:x-dunst-stack-tag:$msgTag \
  -h int:value:"$brightness" "Brightness: ${brightness}"

