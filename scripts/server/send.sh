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
SEND_SOURCE=$(echo "${SOURCE}" | tr -d '\n')

iterations_total=1
while read -r -n 512 SPLIT_SOURCE; do
    iterations_total=$((iterations_total+1))
done <<< "${SEND_SOURCE}"
echo "${iterations_total} iterations."
echo "POCSAG2400:1:\"${iterations_total}\"" | ./pagerenc | ffmpeg -hide_banner -loglevel error -f s16le -ar 22050 -ac 1 -i - -ar 44100 "${BASEPATH}/iterations.wav" && play "${BASEPATH}/iterations.wav" || error_exit "Failed sending total iterations to client" 2

current_iteration=1
while read -r -n 512 SPLIT_SOURCE; do
    echo "SEND: ${current_iteration} of ${iterations_total}"
    echo "POCSAG2400:1:\"${SPLIT_SOURCE}\"" | ./pagerenc | ffmpeg -hide_banner -loglevel error -f s16le -ar 22050 -ac 1 -i - -ar 44100 "${BASEPATH}/output.wav" && play "${BASEPATH}/output.wav" || error_exit "Failed playing audio" 2
    rm -f "${BASEPATH}/output.wav"
    current_iteration=$((current_iteration+1))
done <<< "${SEND_SOURCE}"

echo "POCSAG2400:1:\"==END==\"" | ./pagerenc | ffmpeg -hide_banner -loglevel error -f s16le -ar 22050 -ac 1 -i - -ar 44100 "${BASEPATH}/end.wav" && play "${BASEPATH}/end.wav" || error_exit "Failed sending END signal to client" 2

rm -rf "${BASEPATH}"