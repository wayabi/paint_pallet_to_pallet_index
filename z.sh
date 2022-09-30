#!/bin/sh

dir_src="./data"
dir_dst="./out"

for a in `ls ${dir_src}`
do
	./a.out \
	--path_img_label ${dir_src}/${a} \
	--path_pallet_csv ./pallet.csv \
	--path_img_out ${dir_dst}/${a}
done
