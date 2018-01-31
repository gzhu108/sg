import sys
from create_funcs import *


def create_service_header(namespace, serviceclass, paths):
	rest_service_h_template_file = open("rest.service.template.h")
	rest_service_h = ""
	line = rest_service_h_template_file.readline()
	while line:
		if line.find("@decl_creator_begin") >= 0:
			rest_service_h += create_func(rest_service_h_template_file, "@decl_creator_end", serviceclass, paths)
		else:
			rest_service_h += line.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)
		line = rest_service_h_template_file.readline()

	rest_service_h_template_file.close()

	out_name = serviceclass + "Service.h"
	out_file = open(out_name, "w+")
	out_file.write(rest_service_h)
	out_file.close()


def create_service_cpp(namespace, serviceclass, paths):
	rest_service_cpp_template_file = open("rest.service.template.cpp")
	rest_service_cpp = ""
	line = rest_service_cpp_template_file.readline()
	while line:
		print("create_service_cpp line: ", line)
		if line.find("@incl_creator_begin") >= 0:
			rest_service_cpp += create_func(rest_service_cpp_template_file, "@incl_creator_end", serviceclass, paths)
		elif line.find("@init_creator_begin") >= 0:
			rest_service_cpp += create_func(rest_service_cpp_template_file, "@init_creator_end", serviceclass, paths)
		elif line.find("@impl_creator_begin") >= 0:
			rest_service_cpp += create_func(rest_service_cpp_template_file, "@impl_creator_end", serviceclass, paths)
		else:
			rest_service_cpp += line.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)
		line = rest_service_cpp_template_file.readline()
		print("create_service_cpp out: ", rest_service_cpp)

	rest_service_cpp_template_file.close()

	out_name = serviceclass + "Service.cpp"
	out_file = open(out_name, "w+")
	out_file.write(rest_service_cpp)
	out_file.close()


def main(argv):
	print("Starting", __file__)
	namespace = "mytest";
	serviceclass = "MyTest"
	paths = [("GET", "/v1/version"), ("POST", "/v1/update")]
	create_service_header(namespace, serviceclass, paths)
	create_service_cpp(namespace, serviceclass, paths)


if __name__ == "__main__":
    main(sys.argv)
