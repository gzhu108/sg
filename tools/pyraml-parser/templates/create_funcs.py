import sys


def create_func(infile, endtag, serviceclass, paths):
	text = ""
	line = infile.readline()
	while line:
		print("create_func: ", line)
		if line.find(endtag) >= 0:
			outtext = ""
			for method, path in paths:
				func = path.replace("/", "")
				outtext += text.replace("$method", method) \
					.replace("$path", path) \
					.replace("$func", func) \
					.replace("$serviceclass", serviceclass)
			return outtext
		else:
			text += line
		line = infile.readline()
