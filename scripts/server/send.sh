#!/bin/bash

error_exit() {
    [ -z "${1}" ] && CAUSE="Unknown error"
    [ -z "${2}" ] && EXITCODE=1
    echo "Something went wrong. Reason: ${1}."
    exit ${EXITCODE}
}

[ -z "${1}" ] && error_exit "Please specify a request URL to send"
RAND_TMP_SUFFIX=$(tr -dc A-Za-z0-9 </dev/urandom | head -c 10)
BASEPATH="/tmp/audio-jack-web-${RAND_TMP_SUFFIX}"
mkdir -p "${BASEPATH}"
BASE_URL="${1}"
SOURCE=$(wget "${BASE_URL}" -q -O - | xz | base64)

echo "${SOURCE}" | tr -d '\n' | while read -r -n 450 SPLIT_SOURCE; do
    echo "POCSAG1200:1:\"${SPLIT_SOURCE}\"" | ./pagerenc | ffmpeg -f s16le -ar 22050 -ac 1 -i - -ar 44100 "${BASEPATH}/output.wav" && play "${BASEPATH}/output.wav" || error_exit
    rm -f "${BASEPATH}/output.wav"
done

rm -rf "${BASEPATH}"
