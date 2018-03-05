#!/usr/bin/python
import os, sys, getopt, pathlib
from create_from_template import *
import pyraml.parser


def create_item(infile, outfile, namespace, serviceclass, paths):
	rest_item = create_item_from_template(infile, namespace, serviceclass, paths)
	print("Generating", outfile)
	out_file = open(outfile, "w+")
	out_file.write(rest_item)
	out_file.close()


def create_group(infile, outfile, namespace, method, path, properties):
	rest_group = create_group_from_template(infile, namespace, method, path, properties)
	print("Generating", outfile)
	out_file = open(outfile, "w+")
	out_file.write(rest_group)
	out_file.close()


def create_service_header(tempdir, outdir, namespace, serviceclass, paths):
	infile = tempdir + "/rest.service.template.h"
	outfile = outdir + "/" + serviceclass + "ServiceBase.h"
	create_item(infile, outfile, namespace, serviceclass, paths)


def create_service_cpp(tempdir, outdir, namespace, serviceclass, paths):
	infile = tempdir + "/rest.service.template.cpp"
	outfile = outdir + "/" + serviceclass + "ServiceBase.cpp"
	create_item(infile, outfile, namespace, serviceclass, paths)


def create_reactor_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.reactor.template.h"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "ReactorBase.h"
		create_group(infile, outfile, namespace, method, path, [])


def create_reactor_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.reactor.template.cpp"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "ReactorBase.cpp"
		create_group(infile, outfile, namespace, method, path, [])


def create_message_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.message.template.h"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Message.h"
		create_group(infile, outfile, namespace, method, path, reqprop)


def create_message_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.message.template.cpp"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Message.cpp"
		create_group(infile, outfile, namespace, method, path, reqprop)


def create_response_header(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.response.template.h"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Response.h"
		create_group(infile, outfile, namespace, method, path, resprop)


def create_response_cpp(tempdir, outdir, namespace, paths):
	infile = tempdir + "/rest.response.template.cpp"
	for method, path, reqprop, resprop in paths:
		func = create_func_from_path(path)
		outfile = outdir + "/" + method + func + "Response.cpp"
		create_group(infile, outfile, namespace, method, path, resprop)


def main(argv):
	namespace = "myserver";
	serviceclass = "MyServer"
	paths = \
		[ \
			('GET', '/v1/version', [], [('Version', 'std::string', '"1.0.0.0"')]), \
			('POST', '/v1/update', [('Name', 'std::string', '""'), ('Age', 'std::string', '"18"')], []) \
		]

	tempdir = os.path.dirname(__file__)
	outdir = os.getcwd()
	infile = 'schema.raml'

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

	# load the raml file.
	#p = pyraml.parser.load(infile)
	#print(p)

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
