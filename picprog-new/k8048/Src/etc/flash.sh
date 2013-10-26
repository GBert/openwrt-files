#!/bin/bash

while test 1; do
	kio output a 0
	sleep 0.1
	kio output a 1
	sleep 0.1
done

exit 0
