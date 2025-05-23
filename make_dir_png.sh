#!/bin/bash
for f in *.ppm; do ffmpeg -i "$f" "${f%.ppm}.png"; done
