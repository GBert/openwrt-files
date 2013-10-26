#!/bin/bash
#
# Verify test
#
# run from k8048/Src/
#

function verify {
	while test 1; do
		RC=`${CMD} verify "${HEX}"`
		if test "${RC}" != "Total: ${NUM} Pass: ${NUM} Fail: 0"; then
			echo "${RC}" > verify.err
			echo "Error: verify failed, see verify.err"
			exit 1
		fi
	done
}

if test $# != 1; then
        echo "Try: verify.sh picmicro"
        exit 1
fi
CHIP=${1}

case "${CHIP}" in
pic12f519)
	CMD="k12 select pic12f519"
	NUM="349"
	;;
pic16f648a)
	CMD="k14"
	NUM="513"
	;;
pic18f252)
	CMD="k16"
	NUM="1297"
	;;
pic18f2431)
	CMD="k16"
	NUM="1238"
	;;
pic18f26k80)
	CMD="k16 mchp"
	NUM="1240"
	;;
pic18lf27j53)
	CMD="k16 mchp"
	NUM="1080"
	;;
*)
	echo "Error: picmicro not supported"
	exit 1
	;;
esac

HEX="./asm/test/${CHIP}.hex"
if test ! -f "${HEX}"; then
	echo "Error: hex file not available"
	exit 1
fi

verify

exit 0 # not reached
