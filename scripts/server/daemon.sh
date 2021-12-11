#!/bin/bash

../../multimon-ng/multimon-ng -a MORSE_CW -q | while read -d ' ' REQUEST; do
	if [ "${REQUEST}" != "++END++" ]; then
		REQUEST_URL=$(echo "${REQUEST}" | base32 -d)
		./send.sh "${REQUEST_URL}"
	fi
done
