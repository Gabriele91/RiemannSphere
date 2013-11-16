import sys
from os.path import isfile
name=raw_input("Name files:")
nameupper=name.upper()

#make h file
if not isfile("RiemannSphere/"+name+".h"):
	ofile = open("RiemannSphere/"+name+".h","w")
	ofile.write("#ifndef "+nameupper+"_H\n")
	ofile.write("#define "+nameupper+"_H\n\n")
	ofile.write("#include <Easy3D.h>\n\n")
	ofile.write("namespace RiemannSphere {\n\n")
	ofile.write("\tclass "+name+" {\n\n")
	ofile.write("\tpublic:\n\n")
	ofile.write("\t\t"+name+"();\n\n")
	ofile.write("\t};\n\n")
	ofile.write("};\n\n")
	ofile.write("#endif")
	ofile.close()
else:
	print("RiemannSphere/"+name+".h, olready exist")
#make cppfile
if not isfile("RiemannSphere/"+name+".cpp"):
	ofile = open("RiemannSphere/"+name+".cpp","w")
	ofile.write("#include <stdafx.h>\n")
	ofile.write("#include <"+name+".h>\n\n")
	ofile.write("///////////////////////\n")
	ofile.write("using namespace Easy3D;\n")
	ofile.write("using namespace RiemannSphere;\n")
	ofile.write("///////////////////////\n\n")
	ofile.write(name+"::"+name+"(){\n}")
	ofile.close()
else:
	print("RiemannSphere/"+name+".cpp, olready exist")