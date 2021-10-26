#!/bin/sh

gen_arg_parse_h \
	path_color,string, \
	path_mask,string,man \
	path_output,string, \
	> ./src/arg_parse.h
