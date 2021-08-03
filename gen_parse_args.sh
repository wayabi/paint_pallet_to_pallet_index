#!/bin/sh

gen_arg_parse_h \
	path_color,string,man \
	path_mask,string,man \
	path_output,string, \
	size_color_box,int,man \
	> ./src/arg_parse.h
