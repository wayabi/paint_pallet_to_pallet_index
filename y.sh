#!/bin/sh

for a in `ls mask`
do
echo ${a}
./a.out \
	--path_mask ./mask/${a} \
	--path_out ./mask_out/${a}
done
