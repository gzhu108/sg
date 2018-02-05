#!/usr/bin/python
import os, sys, getopt, pathlib
from create_from_template import *


def create_item(infile, outfile, namespace, serviceclass, paths):
	rest_service = create_item_from_template(infile, namespace, serviceclass, paths)
	print("Generating", outfile)
	out_file = open(outfile, "w+")
	out_file.write(rest_service)
	out_file.close()


def create_group(infile, outfile, namespace, method, func):
	rest_reactor = create_group_from_template(infile, namespace, method, func)
	print("Generating", outfile)
	out_file = open(outfile, "w+")
	out_file.write(rest_reactor)
	out_file.close()


def create_service_header(tempdir, outdir, namespace, serviceclass, paths):
	infile = tempdir + "/rest.service.template.h"
	outfile = outdir + "/" + serviceclass + "Service.h"
	create_item(infile, outfile, namespace, serviceclass, paths)


def create_service_cpp(tempdir, outdir, namespace, serviceclass, paths):
	infile = tempdir + "/rest.service.template.cpp"
	outfile = outdir + "/" + serviceclass + "Service.cpp"
	create_item(infile, outfile, namespace, serviceclass, paths)


def create_reactor_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.reactor.template.h"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Reactor.h"
		create_group(infile, outfile, namespace, method, func)


def create_reactor_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.reactor.template.cpp"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Reactor.cpp"
		create_group(infile, outfile, namespace, method, func)


def create_message_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.message.template.h"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Message.h"
		create_group(infile, outfile, namespace, method, func)


def create_message_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.message.template.cpp"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Message.cpp"
		create_group(infile, outfile, namespace, method, func)


def create_response_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.response.template.h"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Response.h"
		create_group(infile, outfile, namespace, method, func)


def create_response_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.response.template.cpp"
	for method, path in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Response.cpp"
		create_group(infile, outfile, namespace, method, func)


def main(argv):
	namespace = "myserver";
	serviceclass = "MyServer"
	paths = [("GET", "/v1/version"), ("POST", "/v1/update")]

	tempdir = os.path.dirname(__file__)
	outdir = os.getcwd()
	infile = ""

	try:
		opts, args = getopt.getopt(argv[1:], "ht:o:i:", ["tempdir=", "outdir="])
	except getopt.GetoptError:
		print(__file__, "-t <tempdir> -o <outdir>")
		sys.exit(2)

	for opt, arg in opts:
		if opt in ("-t", "--tempdir"):
			tempdir = arg
		elif opt in ("-o", "--outdir"):
			outdir = arg
		elif opt in ("-i", "--infile"):
			infile = arg

	pathlib.Path(outdir).mkdir(parents=True, exist_ok=True) 
	create_service_header(tempdir, outdir, namespace, serviceclass, paths)
	create_service_cpp(tempdir, outdir, namespace, serviceclass, paths)
	create_reactor_header(tempdir, outdir, namespace, paths)
	create_reactor_cpp(tempdir, outdir, namespace, paths)
	create_message_header(tempdir, outdir, namespace, paths)
	create_message_cpp(tempdir, outdir, namespace, paths)
	create_response_header(tempdir, outdir, namespace, paths)
	create_response_cpp(tempdir, outdir, namespace, paths)


if __name__ == "__main__":
    main(sys.argv)
