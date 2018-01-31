import sys


def create_service_header(namespace, serviceclass, paths):
	rest_service_h_template_file = open("rest.service.template.h")

	decl_creator_found = False
	
	decl_creator = ""
	rest_service_h = ""

	for line in rest_service_h_template_file:
		print(line)
		if line.find("@decl_creator_begin") >= 0:
			decl_creator_found = True
		elif decl_creator_found:
			if line.find("@decl_creator_end") >= 0:
				decl_creator_found = False
				for method, path in paths:
					func = path.replace("/", "")
					creator = decl_creator.replace("$method", method) \
						.replace("$func", func)
					rest_service_h += creator
			else:
				decl_creator += line
		else:
			rest_service_h += line.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)

	rest_service_h_template_file.close()
	print(rest_service_h)

	out_name = serviceclass + "Service.h"
	out_file = open(out_name, "w+")
	out_file.write(rest_service_h)
	out_file.close()


def create_service_cpp(namespace, serviceclass, paths):
	rest_service_cpp_template_file = open("rest.service.template.cpp")

	incl_creator_found = False
	init_creator_found = False
	impl_creator_found = False

	incl_creator = ""
	init_creator = ""
	impl_creator = ""
	rest_service_cpp = ""

	for line in rest_service_cpp_template_file:
		print(line)

		if line.find("@incl_creator_begin") >= 0:
			incl_creator_found = True
		elif incl_creator_found:
			if line.find("@incl_creator_end") >= 0:
				incl_creator_found = False
				for method, path in paths:
					func = path.replace("/", "")
					creator = incl_creator.replace("$method", method) \
						.replace("$func", func)
					rest_service_cpp += creator
			else:
				incl_creator += line

		elif line.find("@init_creator_begin") >= 0:
			init_creator_found = True
		elif init_creator_found:
			if line.find("@init_creator_end") >= 0:
				init_creator_found = False
				for method, path in paths:
					func = path.replace("/", "")
					creator = init_creator.replace("$method", method) \
						.replace("$path", path) \
						.replace("$func", func) \
						.replace("$serviceclass", serviceclass)
					rest_service_cpp += creator
			else:
				init_creator += line

		elif line.find("@impl_creator_begin") >= 0:
			impl_creator_found = True
		elif impl_creator_found:
			if line.find("@impl_creator_end") >= 0:
				impl_creator_found = False
				for method, path in paths:
					func = path.replace("/", "")
					creator = impl_creator.replace("$method", method) \
						.replace("$path", path) \
						.replace("$func", func) \
						.replace("$serviceclass", serviceclass)
					rest_service_cpp += creator
			else:
				impl_creator += line
		
		else:
			rest_service_cpp += line.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)

	rest_service_cpp_template_file.close()
	print(rest_service_cpp)

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
