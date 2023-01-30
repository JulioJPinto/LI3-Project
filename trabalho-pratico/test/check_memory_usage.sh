#!/bin/bash

# Set the threshold for peak memory usage
THRESHOLD=$1

# Get the peak memory usage of the command

if [ "$(uname)" == "Darwin" ]; then
  PEAK_MEM=$(/usr/bin/time -l "${@:2}" 2>&1 | awk '/maximum resident set size/{print $NF}')
else
  PEAK_MEM=$(/usr/bin/time -v "${@:2}" 2>&1 | grep 'Maximum resident set size' | awk '{print $NF}')
fi

PEAK_MEM=$((PEAK_MEM / 1024))

# Compare the peak memory usage to the threshold
if [ $PEAK_MEM -gt $THRESHOLD ]; then
  echo "Error: Peak memory usage ($PEAK_MEM MB) exceeded threshold ($THRESHOLD MB)"
  exit 1
else
  exit 0
fi