# microgen.py
import sys, os
import argparse
import ruamel.yaml
import json


def getPropertyType(propertyValue):
    propertyType = ""
    propertyDefault = ""

    if "default" in propertyValue:
        propertyDefault = propertyValue["default"]

    if "string" == propertyValue["type"]:
        propertyType = "std::string"
        if "default" in propertyValue:
            propertyDefault = '"' + propertyValue["default"] + '"'
    elif "integer" == propertyValue["type"]:
        if "int64" == propertyValue["format"]:
            propertyType = "std::int64_t"
        else:
            propertyType = "std::int32_t"
    elif "number" == propertyValue["type"]:
        if "float" == propertyValue["format"]:
            propertyType = "float"
        else:
            propertyType = "double"
    elif "boolean" == propertyValue["type"]:
        propertyType = "bool"
    elif "array" == propertyValue["type"]:
        arrayType, propertyDefault = getPropertyType(propertyValue["items"])
        propertyType = "std::vector<" + arrayType + ">"

    return propertyType, propertyDefault


def replaceContent(line, serviceName, contentName, propertyName, propertyType, propertyDefault, propertyClass):
    namespace = serviceName.lower()
    outText = line \
        .replace("$namespace", namespace) \
        .replace("$contentname", contentName) \
        .replace("$propertyname", propertyName) \
        .replace("$propertytype", propertyType)

    if not propertyDefault:
        outText = outText.replace(", $propertydefault", "")
    else:
        outText = outText.replace("$propertydefault", propertyDefault)

    if (not propertyClass) and ("$propertyclass.h" in outText):
        return ""
    else:
        outText = outText.replace('$propertyclass', propertyClass)

    return outText + "\n"


def replaceContentPropertyLines(metadata, serviceName, contentName, propertyItems):
    lines = metadata.splitlines()
    started = False
    outText = ""

    for line in lines:
        propertyName = ""
        propertyType = ""
        propertyDefault=""
        propertyClass=""

        if started:
            if line.find("@repeatend") >= 0:
                started = False
            else:
                for propertyName, propertyValue in propertyItems:
                    if "$ref" in propertyValue:
                        propertyType = propertyValue["$ref"].replace("#/components/schemas/", "")
                        propertyDefault=""
                        propertyClass=propertyType
                    elif "type" in propertyValue:
                        propertyType, propertyDefault = getPropertyType(propertyValue)
                        propertyClass=""
                    outText += replaceContent(line, serviceName, contentName, propertyName, propertyType, propertyDefault, propertyClass)
        elif line.find("@repeatstart") >= 0:
            started = True
        else:
            outText += replaceContent(line, serviceName, contentName, propertyName, propertyType, propertyDefault, propertyClass)

    return outText


def replaceContentLines(metadata, serviceName, contentName, contentItem):
    lines = metadata.splitlines()
    started = False
    outText = ""

    for line in lines:
        propertyName = ""
        propertyType = ""
        propertyDefault=""
        propertyClass=""

        if started:
            if line.find("@repeatend") >= 0:
                started = False
            else:
                propertyType, propertyDefault = getPropertyType(contentItem["additionalProperties"])
                propertyType = "std::vector<std::pair<std::string, " + propertyType + ">>"
                outText += replaceContent(line, serviceName, contentName, "keyValuePairs", propertyType, propertyDefault, propertyClass)
        elif line.find("@repeatstart") >= 0:
            started = True
        else:
            outText += replaceContent(line, serviceName, contentName, propertyName, propertyType, propertyDefault, propertyClass)

    return outText


def replaceMethodPath(line, serviceName, method="", path="", requestContent=""):
    namespace = serviceName.lower()
    pathname = path.replace("/", "")
    outText = line \
        .replace("$pathname", pathname) \
        .replace("$path", path) \
        .replace("$method", method) \
        .replace("$namespace", namespace) \
        .replace("$service", serviceName) \
        .replace("$requestcontent", requestContent)

    return outText + "\n"


def replacePaths(metadata, serviceName, paths):
    lines = metadata.splitlines()
    started = False
    outText = ""
    pathDict = {}

    for line in lines:
        if started:
            if line.find("@repeatend") >= 0:
                started = False
                for value in pathDict.values():
                    outText += value
            else:
                for path, value in paths.items():
                    if "get" in value:
                        method = "GET"
                        pathDict[method + path] += replaceMethodPath(line, serviceName, method, path)
                    if "post" in value:
                        method = "POST"
                        pathDict[method + path] += replaceMethodPath(line, serviceName, method, path)
                    if "put" in value:
                        method = "PUT"
                        pathDict[method + path] += replaceMethodPath(line, serviceName, method, path)
                    if "delete" in value:
                        method = "DELETE"
                        pathDict[method + path] += replaceMethodPath(line, serviceName, method, path)
                    if "patch" in value:
                        method = "PATCH"
                        pathDict[method + path] += replaceMethodPath(line, serviceName, method, path)

        elif line.find("@repeatstart") >= 0:
            started = True
            pathDict.clear()
            for path, value in paths.items():
                if "get" in value:
                    method = "GET"
                    pathDict[method + path] = ""
                if "post" in value:
                    method = "POST"
                    pathDict[method + path] = ""
                if "put" in value:
                    method = "PUT"
                    pathDict[method + path] = ""
                if "delete" in value:
                    method = "DELETE"
                    pathDict[method + path] = ""
                if "patch" in value:
                    method = "PATCH"
                    pathDict[method + path] = ""
        else:
            outText += replaceMethodPath(line, serviceName)

    return outText


def replaceLines(metadata, serviceName, method, path, requestContent):
    lines = metadata.splitlines()
    started = False
    outText = ""

    for line in lines:
        if started:
            if line.find("@repeatend") >= 0:
                started = False
            else:
                outText += replaceMethodPath(line, serviceName, method, path, requestContent)
        elif line.find("@repeatstart") >= 0:
            started = True
        else:
            outText += replaceMethodPath(line, serviceName, method, path, requestContent)

    return outText


def generateContent(serviceName, jsonDoc):
    scriptPath = sys.path[0]

    contentHeaderTemplate = open(scriptPath + "/Content.h.metadata").read()
    contentSourceTemplate = open(scriptPath + "/Content.cpp.metadata").read()

    for contentName, value in jsonDoc["components"]["schemas"].items():
        propertyType = ""
        propertyDefault = ""
        if ("type" in value) and ("object" == value["type"]):
            for propertyName, propertyValue in value["properties"].items():
                if "$ref" in propertyValue:
                    propertyType = propertyValue["$ref"].replace("#/components/schemas/", "")
                elif "type" in propertyValue:
                    propertyType, propertyDefault = getPropertyType(propertyValue)
                contentHeader = replaceContentPropertyLines(contentHeaderTemplate, serviceName, contentName, value["properties"].items())
                headerFile = serviceName + "/base/" + contentName + ".h"
                saveFile(headerFile, contentHeader)
                contentSource = replaceContentPropertyLines(contentSourceTemplate, serviceName, contentName, value["properties"].items())
                sourceFile = serviceName + "/base/" + contentName + ".cpp"
                saveFile(sourceFile, contentSource)
        elif ("additionalProperties" in value) and ("type" in value["additionalProperties"]):
            contentHeader = replaceContentLines(contentHeaderTemplate, serviceName, contentName, value)
            headerFile = serviceName + "/base/" + contentName + ".h"
            saveFile(headerFile, contentHeader)
            contentSource = replaceContentLines(contentSourceTemplate, serviceName, contentName, value)
            sourceFile = serviceName + "/base/" + contentName + ".cpp"
            saveFile(sourceFile, contentSource)


def generateReactorBase(serviceName, jsonDoc):
    scriptPath = sys.path[0]

    reactorHeaderTemplate = open(scriptPath + "/ReactorBase.h.metadata").read()
    reactorSourceTemplate = open(scriptPath + "/ReactorBase.cpp.metadata").read()

    for path, value in jsonDoc["paths"].items():
        if "get" in value:
            method = "GET"
            requestContent = value["get"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
            requestContent = requestContent.replace("#/components/schemas/", "")

            reactorHeader = replaceLines(reactorHeaderTemplate, serviceName, method, path, requestContent)
            headerFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
            saveFile(headerFile, reactorHeader)
            reactorSource = replaceLines(reactorSourceTemplate, serviceName, method, path, requestContent)
            sourceFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
            saveFile(sourceFile, reactorSource)

        if "post" in value:
            method = "POST"
            requestContent = value["post"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
            requestContent = requestContent.replace("#/components/schemas/", "")

            reactorHeader = replaceLines(reactorHeaderTemplate, serviceName, method, path, requestContent)
            headerFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
            saveFile(headerFile, reactorHeader)
            reactorSource = replaceLines(reactorSourceTemplate, serviceName, method, path, requestContent)
            sourceFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
            saveFile(sourceFile, reactorSource)

        if "put" in value:
            method = "PUT"
            requestContent = value["put"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
            requestContent = requestContent.replace("#/components/schemas/", "")

            reactorHeader = replaceLines(reactorHeaderTemplate, serviceName, method, path, requestContent)
            headerFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
            saveFile(headerFile, reactorHeader)
            reactorSource = replaceLines(reactorSourceTemplate, serviceName, method, path, requestContent)
            sourceFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
            saveFile(sourceFile, reactorSource)

        if "delete" in value:
            method = "DELETE"
            requestContent = value["delete"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
            requestContent = requestContent.replace("#/components/schemas/", "")

            reactorHeader = replaceLines(reactorHeaderTemplate, serviceName, method, path, requestContent)
            headerFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
            saveFile(headerFile, reactorHeader)
            reactorSource = replaceLines(reactorSourceTemplate, serviceName, method, path, requestContent)
            sourceFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
            saveFile(sourceFile, reactorSource)

        if "patch" in value:
            method = "PATCH"
            requestContent = value["patch"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
            requestContent = requestContent.replace("#/components/schemas/", "")

            reactorHeader = replaceLines(reactorHeaderTemplate, serviceName, method, path, requestContent)
            headerFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
            saveFile(headerFile, reactorHeader)
            reactorSource = replaceLines(reactorSourceTemplate, serviceName, method, path, requestContent)
            sourceFile = serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
            saveFile(sourceFile, reactorSource)


def generateServiceBase(serviceName, jsonDoc):
    scriptPath = sys.path[0]

    serviceBaseHeaderTemplate = open(scriptPath + "/ServiceBase.h.metadata").read()
    serviceBaseHeader = replacePaths(serviceBaseHeaderTemplate, serviceName, jsonDoc["paths"])
    headerBaseFile = serviceName + "/base/" + serviceName + "ServiceBase.h"
    saveFile(headerBaseFile, serviceBaseHeader)

    serviceBaseSourceTemplate = open(scriptPath + "/ServiceBase.cpp.metadata").read()
    serviceBaseSource = replacePaths(serviceBaseSourceTemplate, serviceName, jsonDoc["paths"])
    sourceBaseFile = serviceName + "/base/" + serviceName + "ServiceBase.cpp"
    saveFile(sourceBaseFile, serviceBaseSource)


def generateService(serviceName, jsonDoc):
    scriptPath = sys.path[0]

    serviceHeaderTemplate = open(scriptPath + "/Service.h.metadata").read()
    serviceHeader = replacePaths(serviceHeaderTemplate, serviceName, jsonDoc["paths"])
    headerFile = serviceName + "/" + serviceName + "Service.h"
    saveFile(headerFile, serviceHeader)

    serviceSourceTemplate = open(scriptPath + "/Service.cpp.metadata").read()
    serviceSource = replacePaths(serviceSourceTemplate, serviceName, jsonDoc["paths"])
    sourceFile = serviceName + "/" + serviceName + "Service.cpp"
    saveFile(sourceFile, serviceSource)


def generateMain(serviceName, jsonDoc):
    scriptPath = sys.path[0]
    mainTemplate = open(scriptPath + "/main.cpp.metadata").read()
    mainSource = replacePaths(mainTemplate, serviceName, jsonDoc["paths"])
    mainFile = serviceName + "/main.cpp"
    saveFile(mainFile, mainSource)


def generateCMakeLists(serviceName, jsonDoc):
    scriptPath = sys.path[0]
    CMakeListsTemplate = open(scriptPath + "/CMakeLists.txt.metadata").read()
    CMakeListsSource = replacePaths(CMakeListsTemplate, serviceName, jsonDoc["paths"])
    CMakeListsFile = serviceName + "/CMakeLists.txt"
    saveFile(CMakeListsFile, CMakeListsSource)


def saveFile(filename, data):
    print(f"Generating {filename}")
    f = open(filename, "w")
    f.write(data)
    f.close()


def parseYaml(serviceName, fileName):
    try:
        if not os.path.exists(serviceName + "/base"):
            os.makedirs(serviceName + "/base")

        yaml = ruamel.yaml.YAML(typ='safe').load(open(fileName))

        jsonString = json.dumps(yaml, indent=2)
        open(serviceName + "/" + os.path.basename(fileName) + ".json", "w").write(jsonString)

        jsonDoc = json.loads(jsonString)

        generateCMakeLists(serviceName, jsonDoc)
        generateMain(serviceName, jsonDoc)
        generateService(serviceName, jsonDoc)
        generateServiceBase(serviceName, jsonDoc)
        generateReactorBase(serviceName, jsonDoc)
        generateContent(serviceName, jsonDoc)
        
    except yaml.YAMLError as e:
        print(f"Error reading yaml file {fileName}: {e}")
    except json.JSONDecodeError as e:
        print(f"Json Error: {e}")
    except:
        print(f"Error in yaml file: {fileName}")


def parseArg():
    parser = argparse.ArgumentParser(description='microgen.py')

    group2 = parser.add_mutually_exclusive_group()
    group2.add_argument("-s", help="specify service name")
    group2.add_argument("--service", help="specify service name")

    group3 = parser.add_mutually_exclusive_group(required=True)
    group3.add_argument("-f", help="specify yaml file")
    group3.add_argument("--fileName", help="specify yaml file")

    args = parser.parse_args()

    print(f"service: {args.s}")
    print(f"yaml file: {args.f}")
    return args.s, args.f


def main(argv):
    serviceName, fileName = parseArg()
    if not serviceName:
        serviceName = os.path.splitext(os.path.basename(fileName))[0]

    parseYaml(serviceName, fileName)


if __name__ == "__main__":
    main(sys.argv[1:])
