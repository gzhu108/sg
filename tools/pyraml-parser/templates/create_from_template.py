def create_func_from_path(path):
	return path.replace("/", "")


def create_item_group(infile, serviceclass, paths):
	outtext = ""
	text = ""
	line = infile.readline()
	while line:
		if line.find("@group_end") >= 0:
			for method, path, reqprop, resprop in paths:
				func = create_func_from_path(path)
				outtext += text \
					.replace("$method", method) \
					.replace("$path", path) \
					.replace("$func", func) \
					.replace("$serviceclass", serviceclass)
			break
		else:
			text += line
		line = infile.readline()
	return outtext


def create_property_group(infile, properties):
	outtext = ""
	text = ""
	line = infile.readline()
	while line:
		if line.find("@property_end") >= 0:
			for propname, proptype, propvalue in properties:
				outtext += text \
					.replace("$propname", propname) \
					.replace("$proptype", proptype) \
					.replace("$propvalue", propvalue)
			break
		else:
			text += line
		line = infile.readline()
	return outtext


def create_item_from_template(templatefilename, namespace, serviceclass, paths):
	infile = open(templatefilename)
	outtext = ""
	line = infile.readline()
	while line:
		if line.find("@group_begin") >= 0:
			outtext += create_item_group(infile, serviceclass, paths)
		else:
			outtext += line \
				.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)
		line = infile.readline()
	infile.close()
	return outtext


def create_group_from_template(templatefilename, namespace, method, path, properties):
	infile = open(templatefilename)
	outtext = ""
	func = create_func_from_path(path)
	line = infile.readline()
	while line:
		if line.find("@property_begin") >= 0:
			outtext += create_property_group(infile, properties)
		else:
			outtext += line \
				.replace("$namespace", namespace) \
				.replace("$method", method) \
				.replace("$path", path) \
				.replace("$func", func)
		line = infile.readline()
	infile.close()
	return outtext
