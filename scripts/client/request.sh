#!/bin/bash

error_exit() {
    [ -z "${1}" ] && CAUSE="Unknown error"
    [ -z "${2}" ] && EXITCODE=1
    echo "Something went wrong. Reason: ${1}."
    exit ${EXITCODE}
}

[ -z "${1}" ] && error_exit "Please specify a request URL"
RAND_TMP_SUFFIX=$(tr -dc A-Za-z0-9 </dev/urandom | head -c 10)
BASEPATH="/tmp/audio-jack-web-${RAND_TMP_SUFFIX}"
mkdir -p "${BASEPATH}"
BASE_URL="${1}"
ENCODED_OUTPUT=$(echo "${BASE_URL}" | base32 | tr -d "\n")
echo "request.sh: Sending string '${ENCODED_OUTPUT}'"
echo "${ENCODED_OUTPUT}" | ./cwwav -w 35 --output "${BASEPATH}/request.wav" || error_exit "Failed converting base32 to wav file"
play "${BASEPATH}/request.wav" || error_exit "Failed playing audio. Check if sox is installed"
rm -rf "${BASEPATH}"

exit 0