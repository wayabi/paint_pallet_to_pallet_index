#!/bin/sh

gen_arg_parse_h \
	path_img_label,string,man \
	path_pallet_csv,string,man \
	path_img_output,string,man \
	> ./src/arg_parse.h
