#!BPY

"""
Name: 'C code (.c)...'
Blender: 244
Group: 'Import'
Tip: 'Import from C code.'
"""

import bpy
import Blender
import math
from Blender import Mesh,Object,Material,Texture,Image,Draw

# script main function
def ImportFromC(file_name):
	file = open(file_name, "r")

	scene = bpy.data.scenes.active
	meshes = []
	for ob in scene.objects:
		obtype = ob.type
		if obtype == "Mesh":
			meshes.append(ob)

	lines = [l_split for l in file.readlines() for l_split in (' '.join(l.split()),) if l_split]

	# erase all of the comments (/* */)
	i = 0
	insideComment = 0
	while i < len(lines):
		stopErase = 0
		
		commentStart = lines[i].find("/*")
		if insideComment == 1:
			commentStart = 0
			
		if lines[i].find("//") != -1:
			if lines[i].find("//") < commentStart:
				stopErase = 1
		if stopErase != 1:
			if commentStart != -1:
				insideComment = 1
	
		lineEnd = len(lines[i])
		commentEnd = lines[i].find("*/")
		
		if commentEnd != -1:
			if insideComment == 1:
				commentEnd = lines[i].find("*/")+2
				insideComment = 0
		else:
			if insideComment == 1:
				commentEnd = lineEnd
		
		if commentStart != -1:
			if lines[i].find("//") != -1:
				if lines[i].find("//") < commentStart:
					stopErase = 1
				if lines[i].find("//")+2 < commentEnd:
					stopErase = 1
			if stopErase != 1:
				lines[i] = lines[i].replace(lines[i][commentStart:commentEnd], "")
		
		commentStart = lines[i].find("/*")
		if commentStart == -1:
			i = i + 1
		if stopErase == 1:
			i = i + 1

	# erase all of the comments (//)
	i = 0
	while i < len(lines):
		commentStart = lines[i].find("//")
		commentEnd = len(lines[i])
		if commentStart != -1:
			lines[i] = lines[i].replace(lines[i][commentStart:commentEnd], "")
		i = i + 1
	
	# erase all spaces
	i = 0
	while i < len(lines):
		lines[i] = lines[i].replace(" ", "")
		i = i + 1
	
	# move the data from "lines" to "code" ignoring returns
	i = 0
	code = []
	while i < len(lines):
		lineEnd = len(lines[i])
		j = 0
		while j < lineEnd:
			code[len(code):] = lines[i][j]
			j = j + 1
		i = i + 1
	
	
	# find the first listed vertex array size
	i = 0
	while i < len(code):
		if code[0+i:12+i] == ["_", "P", "O", "I", "N", "T", "D", "A", "T", "S", "Z", "["]:
			codePos = i + 12
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1

	# read the vertex array sizes from the code
	endOfNum = 0
	numCnt = 0
	pointDatSz = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		pointDatSz[len(pointDatSz):] = [int(num)]
	
	#number of meshes
	meshCnt = len(pointDatSz)
	
	# find the first listed face array size
	i = 0
	while i < len(code):
		if code[0+i:11+i] == ["_", "L", "I", "N", "E", "D", "A", "T", "S", "Z", "["]:
			codePos = i + 11
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1

	# read the face array sizes from the code
	endOfNum = 0
	numCnt = 0
	lineDatSz = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		lineDatSz[len(lineDatSz):] = [int(num)]
	
	# find the first listed color array size
	i = 0
	while i < len(code):
		if code[0+i:12+i] == ["_", "C", "O", "L", "O", "R", "D", "A", "T", "S", "Z", "["]:
			codePos = i + 11
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1

	# read the color array sizes from the code
	endOfNum = 0
	numCnt = 0
	colorDatSz = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		colorDatSz[len(colorDatSz):] = [int(num)]
	
	
	# find the first listed vertex
	i = 0
	while i < len(code):
		if code[0+i:7+i] == ["_", "P", "O", "I", "N", "T", "["]:
			codePos = i + 7
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1
	
	# read the verticies from the code
	endOfNum = 0
	vert = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		vert[len(vert):] = [float(num)]
	

	# find the first listed line data number
	i = 0
	while i < len(code):
		if code[0+i:6+i] == ["_", "L", "I", "N", "E", "["]:
			codePos = i + 6
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1
	
	# read the line data from the code
	endOfNum = 0
	line = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		line[len(line):] = [int(num)]
	
	
	# find the first listed color
	i = 0
	while i < len(code):
		if code[0+i:7+i] == ["_", "C", "O", "L", "O", "R", "["]:
			codePos = i + 7
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1
	
	# read the colors from the code
	endOfNum = 0
	color = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		color[len(color):] = [int(float.fromhex(num))]
	
	
	meshInc = 0
	while meshInc < meshCnt:
		mesh = Mesh.New()
		
		# import verticies to blender
		if meshInc == 0:
			i = 0
		else:
			i = pointDatSz[meshInc-1]
		while i < pointDatSz[meshInc]:
			mesh.verts.extend(vert[i],vert[i+1],vert[i+2])
			i = i + 3

		# import faces to blender
		if meshInc == 0:
			i = 0
			polyBgn = 0
		else:
			i = lineDatSz[meshInc-1]
			polyBgn = pointDatSz[meshInc-1] / 3
			
		while i < lineDatSz[meshInc]:
			if line[i] == 4:
				mesh.faces.extend(line[i+1]-polyBgn, line[i+2]-polyBgn, line[i+3]-polyBgn, line[i+4]-polyBgn)
				i = i + 5
			else:
				if line[i] == 3:
					mesh.faces.extend(line[i+1]-polyBgn, line[i+2]-polyBgn, line[i+3]-polyBgn)
					i = i + 4
		
		
		# import a list of colors into blender
		mat_list = []
		if meshInc == 0:
			i = 0
		else:
			i = colorDatSz[meshInc-1]
		colorPal = [color[i]]
		while i < colorDatSz[meshInc]:
			
			j = 0
			addColor = 1
			while j < len(colorPal):
				if colorPal[j] == color[i]:
					addColor = 0
				j = j + 1
			
			if addColor == 1:
				colorPal[len(colorPal):] = [color[i]]
			
			i = i + 1
		
		colorPalSz = len(colorPal)
		i = 0
		while i < colorPalSz:
			mat = bpy.data.materials.new()
			mat.rgbCol = [float(colorPal[i] & 0x0000FF) / 255,\
				float((colorPal[i] & 0x00FF00) >> 8) / 255,\
				float((colorPal[i] & 0xFF0000) >> 16) / 255]
			mat_list.append(mat)
			mesh.materials = mat_list[:16]
			i = i + 1
			
			if i == 16:
				colorPalSz = 16
				print "Only 16 colors is allowed."
			
			
		# link the faces with the colors
		if meshInc == 0:
			colorArrayPos = 0
		else:
			colorArrayPos = colorDatSz[meshInc-1]
		i = 0
		while i < len(mesh.faces):
			j = 0
			while j < len(colorPal):
				if color[colorArrayPos+i] == colorPal[j]:
					colorNum = j
				j = j + 1
			mesh.faces[i].mat = colorNum
			i = i + 1
		
		scene.objects.new(mesh)
		meshInc = meshInc + 1
	
	
	scene = Blender.Scene.GetCurrent()
	meshes = []
	for ob in scene.objects:
		obtype = ob.type
		if obtype == "Mesh":
			meshes.append(ob)

	# find the first listed transformation data value
	i = 0
	while i < len(code):
		if code[0+i:7+i] == ["_", "T", "R", "A", "N", "S", "["]:
			codePos = i + 7
			i = len(code)
		i = i + 1
	
	i = codePos
	while i < len(code):
		if code[0+i:3+i] == ["]", "=", "{"]:
			codePos = i + 3
			i = len(code)
		i = i + 1

	# read the transformation data from the code
	endOfNum = 0
	numCnt = 0
	trans = []
	while endOfNum != 2:
		i = 0
		num = ""
		endOfNum = 0
		while endOfNum == 0:
			if code[codePos+i] == "," or code[codePos+i] == "}":
				endOfNum = 1
			else:
				num = num + code[codePos+i]
			i = i + 1
			if code[codePos+i] == "}":
				endOfNum = 2
		
		if endOfNum != 2:
			codePos = codePos+i
		
		trans[len(trans):] = [float(num)]
	
	#Read the transformation data from file
	i = 0
	matrix = ob.getMatrix()
	while i < meshCnt:
		meshes[meshCnt-1-i].rot = [trans[i*9+0], trans[i*9+1], trans[i*9+2]]
		meshes[meshCnt-1-i].loc = [trans[i*9+3], trans[i*9+4], trans[i*9+5]]
		meshes[meshCnt-1-i].size = [trans[i*9+6], trans[i*9+7], trans[i*9+8]]
		i = i + 1
	
	
def my_callback(filename):
	ImportFromC(filename)

if __name__ == '__main__':
	Blender.Window.FileSelector(my_callback, "Import C code", "*.c")
