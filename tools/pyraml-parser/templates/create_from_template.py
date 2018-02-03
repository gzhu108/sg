def create_func_from_path(path):
	return path.replace("/", "")


def create_group(infile, serviceclass, paths):
	outtext = ""
	text = ""
	line = infile.readline()
	while line:
		if line.find("@group_end") >= 0:
			for method, path in paths:
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


def create_item_from_template(templatefilename, namespace, serviceclass, paths):
	infile = open(templatefilename)
	outtext = ""
	line = infile.readline()
	while line:
		if line.find("@group_begin") >= 0:
			outtext += create_group(infile, serviceclass, paths)
		else:
			outtext += line \
				.replace("$namespace", namespace) \
				.replace("$serviceclass", serviceclass)
		line = infile.readline()
	infile.close()
	return outtext


def create_group_from_template(templatefilename, namespace, method, func):
	infile = open(templatefilename)
	outtext = ""
	line = infile.readline()
	while line:
		outtext += line \
			.replace("$namespace", namespace) \
			.replace("$method", method) \
			.replace("$func", func)
		line = infile.readline()
	infile.close()
	return outtext
