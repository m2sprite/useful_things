#!/bin/bash
for f in *.c; do to_html "$f"; done
for f in *.h; do to_html "$f"; done
