# microgen.py
import sys, os, shutil, glob
import argparse
import ruamel.yaml
import json


class microgen:
    def __init__(self, serviceName, jsonDoc):
        self.serviceName = serviceName
        self.namespace = serviceName.lower()
        self.jsonDoc = jsonDoc


    def getPropertyType(self, propertyValue):
        propertyType = ""
        propertyDefault = ""

        if "string" == propertyValue["type"]:
            propertyType = "std::string"
            if "default" in propertyValue:
                propertyDefault = '"' + propertyValue["default"] + '"'
        elif "integer" == propertyValue["type"]:
            if "int64" == propertyValue["format"]:
                propertyType = "std::int64_t"
            else:
                propertyType = "std::int32_t"
            if "default" in propertyValue:
                propertyDefault = propertyValue["default"]
            else:
                propertyDefault = "0"
        elif "number" == propertyValue["type"]:
            if "float" == propertyValue["format"]:
                propertyType = "float"
            else:
                propertyType = "double"
            if "default" in propertyValue:
                propertyDefault = propertyValue["default"]
            else:
                propertyDefault = "0.0"
        elif "boolean" == propertyValue["type"]:
            propertyType = "bool"
            if "default" in propertyValue:
                propertyDefault = propertyValue["default"]
            else:
                propertyDefault = "false"
        elif "array" == propertyValue["type"]:
            arrayType, propertyDefault = self.getPropertyType(propertyValue["items"])
            propertyType = "std::vector<" + arrayType + ">"

        return propertyType, propertyDefault


    def replaceContent(self, line, contentName, propertyName, propertyType, propertyDefault, propertyClass):
        outText = line \
            .replace("$namespace", self.namespace) \
            .replace("$contentname", contentName) \
            .replace("$propertyname", propertyName) \
            .replace("$propertytype", propertyType)

        if not propertyDefault:
            outText = outText.replace(" {$propertydefault}", "")
        else:
            outText = outText.replace("$propertydefault", propertyDefault)

        if (not propertyClass) and ("$propertyclass.h" in outText):
            return ""
        else:
            outText = outText.replace('$propertyclass', propertyClass)

        return outText + "\n"


    def replaceContentPropertyLines(self, metadata, contentName, propertyItems):
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
                        propertyName = propertyName[0].upper() + propertyName[1:]
                        if "$ref" in propertyValue:
                            propertyType = propertyValue["$ref"].replace("#/components/schemas/", "")
                            propertyDefault=""
                            propertyClass=propertyType
                        elif "type" in propertyValue:
                            propertyType, propertyDefault = self.getPropertyType(propertyValue)
                            propertyClass=""
                        outText += self.replaceContent(line, contentName, propertyName, propertyType, propertyDefault, propertyClass)
            elif line.find("@repeatstart") >= 0:
                started = True
            else:
                outText += self.replaceContent(line, contentName, propertyName, propertyType, propertyDefault, propertyClass)

        return outText


    def replaceContentLines(self, metadata, contentName, contentItem):
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
                    propertyType, propertyDefault = self.getPropertyType(contentItem["additionalProperties"])
                    propertyType = "std::vector<std::pair<std::string, " + propertyType + ">>"
                    outText += self.replaceContent(line, contentName, "KeyValuePairs", propertyType, propertyDefault, propertyClass)
            elif line.find("@repeatstart") >= 0:
                started = True
            else:
                outText += self.replaceContent(line, contentName, propertyName, propertyType, propertyDefault, propertyClass)

        return outText


    def replaceMethodPath(self, line, method="", path="", requestContent=""):
        pathname = path.replace("/", "")
        outText = line \
            .replace("$pathname", pathname) \
            .replace("$path", path) \
            .replace("$method", method) \
            .replace("$namespace", self.namespace) \
            .replace("$service", self.serviceName) \
            .replace("$requestcontent", requestContent)

        return outText + "\n"


    def replacePaths(self, metadata, paths):
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
                            pathDict[method + path] += self.replaceMethodPath(line, method, path)
                        if "post" in value:
                            method = "POST"
                            pathDict[method + path] += self.replaceMethodPath(line, method, path)
                        if "put" in value:
                            method = "PUT"
                            pathDict[method + path] += self.replaceMethodPath(line, method, path)
                        if "delete" in value:
                            method = "DELETE"
                            pathDict[method + path] += self.replaceMethodPath(line, method, path)
                        if "patch" in value:
                            method = "PATCH"
                            pathDict[method + path] += self.replaceMethodPath(line, method, path)

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
                outText += self.replaceMethodPath(line)

        return outText


    def replaceLines(self, metadata, method, path, requestContent):
        lines = metadata.splitlines()
        started = False
        outText = ""

        for line in lines:
            if started:
                if line.find("@repeatend") >= 0:
                    started = False
                else:
                    outText += self.replaceMethodPath(line, method, path, requestContent)
            elif line.find("@repeatstart") >= 0:
                started = True
            else:
                outText += self.replaceMethodPath(line, method, path, requestContent)

        return outText


    def generateContent(self):
        scriptPath = sys.path[0]

        contentHeaderTemplate = open(scriptPath + "/Content.h.metadata").read()
        contentSourceTemplate = open(scriptPath + "/Content.cpp.metadata").read()

        for contentName, value in self.jsonDoc["components"]["schemas"].items():
            if ("type" in value) and ("object" == value["type"]):
                for propertyName, propertyValue in value["properties"].items():
                    contentHeader = self.replaceContentPropertyLines(contentHeaderTemplate, contentName, value["properties"].items())
                    headerFile = self.serviceName + "/base/" + contentName + ".h"
                    self.saveFile(headerFile, contentHeader)
                    contentSource = self.replaceContentPropertyLines(contentSourceTemplate, contentName, value["properties"].items())
                    sourceFile = self.serviceName + "/base/" + contentName + ".cpp"
                    self.saveFile(sourceFile, contentSource)
            elif ("additionalProperties" in value) and ("type" in value["additionalProperties"]):
                contentHeader = self.replaceContentLines(contentHeaderTemplate, contentName, value)
                headerFile = self.serviceName + "/base/" + contentName + ".h"
                self.saveFile(headerFile, contentHeader)
                contentSource = self.replaceContentLines(contentSourceTemplate, contentName, value)
                sourceFile = self.serviceName + "/base/" + contentName + ".cpp"
                self.saveFile(sourceFile, contentSource)


    def generateReactorBase(self):
        scriptPath = sys.path[0]

        reactorHeaderTemplate = open(scriptPath + "/ReactorBase.h.metadata").read()
        reactorSourceTemplate = open(scriptPath + "/ReactorBase.cpp.metadata").read()

        for path, value in self.jsonDoc["paths"].items():
            if "get" in value:
                method = "GET"
                requestContent = ""
                if "requestBody" in value["get"]: 
                    requestContent = value["get"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
                    requestContent = requestContent.replace("#/components/schemas/", "")

                reactorHeader = self.replaceLines(reactorHeaderTemplate, method, path, requestContent)
                headerFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
                self.saveFile(headerFile, reactorHeader)
                reactorSource = self.replaceLines(reactorSourceTemplate, method, path, requestContent)
                sourceFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
                self.saveFile(sourceFile, reactorSource)

            if "post" in value:
                method = "POST"
                requestContent = ""
                if "requestBody" in value["post"]:
                    requestContent = value["post"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
                    requestContent = requestContent.replace("#/components/schemas/", "")

                reactorHeader = self.replaceLines(reactorHeaderTemplate, method, path, requestContent)
                headerFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
                self.saveFile(headerFile, reactorHeader)
                reactorSource = self.replaceLines(reactorSourceTemplate, method, path, requestContent)
                sourceFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
                self.saveFile(sourceFile, reactorSource)

            if "put" in value:
                method = "PUT"
                requestContent = ""
                if "requestBody" in value["put"]:
                    requestContent = value["put"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
                    requestContent = requestContent.replace("#/components/schemas/", "")

                reactorHeader = self.replaceLines(reactorHeaderTemplate, method, path, requestContent)
                headerFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
                self.saveFile(headerFile, reactorHeader)
                reactorSource = self.replaceLines(reactorSourceTemplate, method, path, requestContent)
                sourceFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
                self.saveFile(sourceFile, reactorSource)

            if "delete" in value:
                method = "DELETE"
                requestContent = ""
                if "requestBody" in value["delete"]:
                    requestContent = value["delete"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
                    requestContent = requestContent.replace("#/components/schemas/", "")

                reactorHeader = self.replaceLines(reactorHeaderTemplate, method, path, requestContent)
                headerFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
                self.saveFile(headerFile, reactorHeader)
                reactorSource = self.replaceLines(reactorSourceTemplate, method, path, requestContent)
                sourceFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
                self.saveFile(sourceFile, reactorSource)

            if "patch" in value:
                method = "PATCH"
                requestContent = ""
                if "requestBody" in value["patch"]:
                    requestContent = value["patch"]["requestBody"]["content"]["application/json"]["schema"]["$ref"]
                    requestContent = requestContent.replace("#/components/schemas/", "")

                reactorHeader = self.replaceLines(reactorHeaderTemplate, method, path, requestContent)
                headerFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.h"
                self.saveFile(headerFile, reactorHeader)
                reactorSource = self.replaceLines(reactorSourceTemplate, method, path, requestContent)
                sourceFile = self.serviceName + "/base/" + method + path.replace("/", "") + "ReactorBase.cpp"
                self.saveFile(sourceFile, reactorSource)


    def generateServiceBase(self):
        scriptPath = sys.path[0]

        serviceBaseHeaderTemplate = open(scriptPath + "/ServiceBase.h.metadata").read()
        serviceBaseHeader = self.replacePaths(serviceBaseHeaderTemplate, self.jsonDoc["paths"])
        headerBaseFile = self.serviceName + "/base/" + self.serviceName + "ServiceBase.h"
        self.saveFile(headerBaseFile, serviceBaseHeader)

        serviceBaseSourceTemplate = open(scriptPath + "/ServiceBase.cpp.metadata").read()
        serviceBaseSource = self.replacePaths(serviceBaseSourceTemplate, self.jsonDoc["paths"])
        sourceBaseFile = self.serviceName + "/base/" + self.serviceName + "ServiceBase.cpp"
        self.saveFile(sourceBaseFile, serviceBaseSource)


    def generateService(self):
        scriptPath = sys.path[0]

        serviceHeaderTemplate = open(scriptPath + "/Service.h.metadata").read()
        serviceHeader = self.replacePaths(serviceHeaderTemplate, self.jsonDoc["paths"])
        headerFile = self.serviceName + "/" + self.serviceName + "Service.h"
        self.saveFile(headerFile, serviceHeader)

        serviceSourceTemplate = open(scriptPath + "/Service.cpp.metadata").read()
        serviceSource = self.replacePaths(serviceSourceTemplate, self.jsonDoc["paths"])
        sourceFile = self.serviceName + "/" + self.serviceName + "Service.cpp"
        self.saveFile(sourceFile, serviceSource)


    def generateMain(self):
        scriptPath = sys.path[0]
        mainTemplate = open(scriptPath + "/main.cpp.metadata").read()
        mainSource = self.replacePaths(mainTemplate, self.jsonDoc["paths"])
        mainFile = self.serviceName + "/main.cpp"
        self.saveFile(mainFile, mainSource)


    def generateCMakeLists(self):
        scriptPath = sys.path[0]
        CMakeListsTemplate = open(scriptPath + "/CMakeLists.txt.metadata").read()
        CMakeListsSource = self.replacePaths(CMakeListsTemplate, self.jsonDoc["paths"])
        CMakeListsFile = self.serviceName + "/CMakeLists.txt"
        self.saveFile(CMakeListsFile, CMakeListsSource)


    def saveFile(self, filename, data):
        print(f"Generating {filename}")
        f = open(filename, "w")
        f.write(data)
        f.close()


def parseYaml(serviceName, fileName):
    try:
        yaml = ruamel.yaml.YAML(typ='safe').load(open(fileName))
        jsonString = json.dumps(yaml, indent=2)

        if not os.path.exists(serviceName + "/base"):
            os.makedirs(serviceName + "/base")

        shutil.copy2(fileName, serviceName)
        #open(serviceName + "/" + os.path.basename(fileName) + ".json", "w").write(jsonString)

        jsonDoc = json.loads(jsonString)

        mg = microgen(serviceName, jsonDoc)
        mg.generateCMakeLists()
        mg.generateMain()
        mg.generateService()
        mg.generateServiceBase()
        mg.generateReactorBase()
        mg.generateContent()
        
    except yaml.YAMLError as e:
        print(f"Error reading yaml file {fileName}: {e}")
    except json.JSONDecodeError as e:
        print(f"Json Error: {e}")
    except:
        print(f"Error in yaml file: {fileName}")


def copyMicroreactor(serviceName):
    includePath = os.path.join(os.getcwd(), serviceName + "/include/microreactor")
    libPath = os.path.join(os.getcwd(), serviceName + "/lib")

    shutil.rmtree(includePath, ignore_errors=True)
    shutil.rmtree(libPath, ignore_errors=True)
    os.makedirs(includePath)
    os.makedirs(libPath)

    sourcePath = os.path.realpath(os.path.dirname(__file__) + "/../src")
    libraryPath = os.path.realpath(os.path.dirname(__file__) + "/../../build/microreactor/Debug")

    includeFiles = glob.iglob(os.path.join(sourcePath, "*.h"))
    for f in includeFiles:
        if os.path.isfile(f):
            shutil.copy2(f, includePath)

    libFiles = glob.iglob(os.path.join(libraryPath, "*.lib"))
    for f in libFiles:
        if os.path.isfile(f):
            shutil.copy2(f, libPath)

    aFiles = glob.iglob(os.path.join(libraryPath, "*.a"))
    for f in aFiles:
        if os.path.isfile(f):
            shutil.copy2(f, libPath)


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
    copyMicroreactor(serviceName)


if __name__ == "__main__":
    main(sys.argv[1:])
