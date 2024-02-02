#!/bin/bash

# This is a Bash script to run ERSAP CLAS12 streamning data processing pipeline

# NERSC specific
mkdir $HOME/ersap
mkdir $HOME/ersap/log
mkdir $HOME/ersap/output
cd /ersap

echo "Setting up the environment"
source setup


# Get processing node IP address at NERSC and set PR_HOST to it 
PR_HOST=$( hostname -i | awk '{print $2}' )

# Check if we got an IP address
if [ -z "$PR_HOST" ]; then
    echo "No IP address for processing node found. Exiting... "
    exit 1
else
    echo "IP address for the processing node is: $PR_HOST"
fi

echo "PR_HOST is set to $PR_HOST"   

# Check to see if LB_HOST is set

if [ -z "$LB_HOST" ]; then
# If it is not set, set it to ESnet LB for NERSC 
	export LB_HOST=192.188.29.6
fi
echo "LB_HOST is set to $LB_HOST"

echo "Starting ET "
et_start_fifo -f /tmp/fifoEt -d -s 150000 -n 1 -e 1000 -a 239.200.0.99 &
sleep 5

echo "Starting reassembly engine"
packetBlasteeEtFifoClientNew  -b 4000000 -r 25000000 -cores 83 -f /tmp/fifoEt -a $PR_HOST -gaddr $LB_HOST &

