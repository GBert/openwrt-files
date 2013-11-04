#!/bin/bash
#
# Verify test
#
# run from k8048/Src/
#

function verify {
	while test 1; do
		RC1=`${CMD} verify ${HEX}`
		RC2=`echo -n ${RC1} | grep 'Fail: 0'`
		if test "${RC2}" == ""; then
			echo "${RC1}" > verify.err
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
	;;
pic16f627)
	;&
pic16f628a)
	;&
pic16f648a)
	;&
pic16f886)
	CMD="k14"
	;;
pic16f1825)
	CMD="k14 mchp"
	;;
pic18f1320)
	;&
pic18f252)
	;&
pic18f2431)
	CMD="k16"
	;;
pic18f25k50)
	;&
pic18f26k80)
	;&
pic18lf27j53)
	CMD="k16 mchp"
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
