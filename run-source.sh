#!/bin/bash

# This is a Bash script to run ERSAP CLAS12 streamning data processing pipeline

# NERSC specific
mkdir $HOME/ersap
mkdir $HOME/ersap/log
mkdir $HOME/ersap/output
cd /ersap

echo "Setting up the environment"
source setup

# Check if the source file env variable is set
if [ -z "$CLAS_FILE" ]; then
	echo "Using default CLAS12 data file"
	CLAS_FILE=data/clas_005038.1231.hipo
else
	echo "Using $CLAS_FILE data file"
fi

# Check if we got the LB IP address
if [ -z "$LB_IP" ]; then
    echo "Setting LB_IP address LB located at the ESnet: 192.188.29.16"
else
    echo "IP address for the LB is: $LB_IP"
fi

# Check to see if LB_HOST is set
if [ -z "$LB_HOST" ]; then
	echo "Setting LB_HOST located at the ESnet: 192.188.29.6"
	export LB_HOST=192.188.29.6
else
    echo "IP address for the LB_HOST is: $LB_HOST"
fi

# Check to see if MTU is set
if [ -z "$MTU" ]; then
        echo "Setting MTU to 9000"
        export MTU=9000
else
    echo "MTU is set to $MTU"
fi

# Check to see if BUFDELAY is set
if [ -z "$BUFDELAY" ]; then
        echo "Setting BUFDELAY to 2000"
        export BUFDELAY=2000
else
    echo "BUFDELAY is set to $BUFDELAY"
fi

echo "Starting streaming CLAS12 data"
clasBlaster -f $CLAS_FILE -host $LB_IP -cp_addr $LB_HOST -sock 1 -mtu $MTU -s 25000000 -bufdelay -d $BUFDELAY 
