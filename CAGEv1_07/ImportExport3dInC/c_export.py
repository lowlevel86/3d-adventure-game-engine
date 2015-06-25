#!BPY

"""
Name: 'C code (.c)...'
Blender: 244
Group: 'Export'
Tip: 'Export to C code.'
"""

import Blender
import math


def WriteVertices(file, ob):
	global meshCnt
	file.write("//data array %i\n" % (meshCnt))
	mesh = ob.getData()
	for vert in mesh.verts:
		file.write("%.6f, %.6f, %.6f,\n" % (vert[0], vert[1], vert[2]))

def WriteFaces(file, ob):
	global meshCnt
	global vertCnt
	file.write("//data array %i\n" % (meshCnt))
	mesh = ob.getData()
	for face in mesh.faces:
		face_v = face.v
		if len(face_v) == 3:
			file.write("3, %i, %i, %i,\n" % (face_v[0].index + vertCnt,
				face_v[1].index + vertCnt, face_v[2].index + vertCnt))
		elif len(face_v) == 4:
			file.write("4, %i, %i, %i, %i,\n" % (face_v[0].index + vertCnt,
				face_v[1].index + vertCnt, face_v[2].index + vertCnt, face_v[3].index + vertCnt))
	vertCnt += len(mesh.verts)

def WriteColors(file, ob):
	global meshCnt
	file.write("//data array %i\n" % (meshCnt))
	mesh = ob.getData()
	if mesh.materials:
		colorPalette = []
		for mat in mesh.materials:
			hexColor = int(round(mat.rgbCol[2]*255)) << 16
			hexColor += int(round(mat.rgbCol[1]*255)) << 8
			hexColor += int(round(mat.rgbCol[0]*255))
			colorPalette[len(colorPalette):] = [hexColor]
		for face in mesh.faces:
			file.write("0x%x,\n" % (colorPalette[face.mat]))
	else:
		for face in mesh.faces:
			file.write("0x0,\n")


# script main function
def ExportToC(file_name):
	file = open(file_name, "w")
	
	scene = Blender.Scene.GetCurrent()
	meshes = []
	revMeshes = []
	for ob in scene.objects:
		obtype = ob.type
		if obtype == "Mesh":
			revMeshes.append(ob)
	meshes = revMeshes[::-1] 
	
	global meshCnt
	global vertCnt
	
	contentName = Blender.sys.splitext(Blender.sys.basename(file_name))[0]
	
	#Print the global transformation data to file
	file.write("//global transformation data\n")
	file.write("float %s_GLOBAL[9]={\n" % (contentName))
	file.write("0.000000, 0.000000, 0.000000, //rotation\n")
	file.write("0.000000, 0.000000, 0.000000, //location\n")
	file.write("1.000000, 1.000000, 1.000000, //size\n")
	file.write("};\n\n")
	
	#Print the transformation data to file
	meshCnt = 0
	file.write("//transformation data\n")
	file.write("float %s_TRANS[%i]={\n" % (contentName, len(meshes) * 9))
	for mesh in meshes:
		file.write("//data block %i\n" % (meshCnt))
		file.write("%.6f, %.6f, %.6f,\n" % (mesh.rot[0], mesh.rot[1], mesh.rot[2]))
		file.write("%.6f, %.6f, %.6f,\n" % (mesh.loc[0], mesh.loc[1], mesh.loc[2]))
		file.write("%.6f, %.6f, %.6f,\n" % (mesh.size[0], mesh.size[1], mesh.size[2]))
		meshCnt += 1
	file.write("};\n\n")
	
	#Get the number of vertices
	vertCnt = 0
	for mesh in meshes:
		vertCnt += len(mesh.getData().verts)
	
	#Print the point data to file
	meshCnt = 0
	file.write("//point data\n")
	file.write("float %s_POINT[%i]={\n" % (contentName, vertCnt * 3))
	for mesh in meshes:
		WriteVertices(file, mesh)
		meshCnt += 1
	file.write("};\n\n")
	
	#Get the number of line data symbols
	ldsymCnt = 0
	for mesh in meshes:
		for face in mesh.getData().faces:
			if len(face.v) == 3:
				ldsymCnt += 4
			elif len(face.v) == 4:
				ldsymCnt += 5
	
	#Print the line data to file
	meshCnt = 0
	vertCnt = 0
	file.write("//line data\n")
	file.write("int %s_LINE[%i]={\n" % (contentName, ldsymCnt))
	for mesh in meshes:
		WriteFaces(file, mesh)
		meshCnt += 1
	file.write("};\n\n")
	
	#Get the number of faces to find the color data count
	faceCnt = 0
	for mesh in meshes:
		for face in mesh.getData().faces:
			faceCnt += 1
		
	#Print the color data to file
	meshCnt = 0
	file.write("//color data\n")
	file.write("int %s_COLOR[%i]={\n" % (contentName, faceCnt))
	for mesh in meshes:
		WriteColors(file, mesh)
		meshCnt += 1
	file.write("};\n\n")
	
	
	#Print the point data array sizes to file
	vertCnt = 0
	file.write("//point data array sizes\n")
	file.write("int %s_POINTDATSZ[%i]={\n" % (contentName, len(meshes)))
	for mesh in meshes:
		vertCnt += len(mesh.getData().verts)
		file.write("%i,\n" % (vertCnt * 3))
	file.write("};\n\n")
	
	#Print the line data array sizes to file
	ldsymCnt = 0
	file.write("//line data array sizes\n")
	file.write("int %s_LINEDATSZ[%i]={\n" % (contentName, len(meshes)))
	for mesh in meshes:
		for face in mesh.getData().faces:
			if len(face.v) == 3:
				ldsymCnt += 4
			elif len(face.v) == 4:
				ldsymCnt += 5
		file.write("%i,\n" % (ldsymCnt))
	file.write("};\n\n")
	
	#Print the color data array sizes to file
	faceCnt = 0
	file.write("//color data array sizes\n")
	file.write("int %s_COLORDATSZ[%i]={\n" % (contentName, len(meshes)))
	for mesh in meshes:
		for face in mesh.getData().faces:
			faceCnt += 1
		file.write("%i,\n" % (faceCnt))
	file.write("};\n\n")
	
	#Print the number of meshes
	file.write("//number of meshes\n")
	file.write("int %s_MESHCNT = %i;\n" % (contentName, len(meshes)))
	
	file.close()
	
	
	#Write the header file
	headerName = Blender.sys.dirname(file_name)
	headerName += '/'
	headerName += Blender.sys.splitext(Blender.sys.basename(file_name))[0]
	headerName += '.h'
	file = open(headerName, "w")
	
	file.write("extern float %s_GLOBAL[9];\n" % (contentName))
	file.write("extern float %s_TRANS[%i];\n" % (contentName, len(meshes) * 9))
	file.write("extern float %s_POINT[%i];\n" % (contentName, vertCnt * 3))
	file.write("extern int %s_LINE[%i];\n" % (contentName, ldsymCnt))
	file.write("extern int %s_COLOR[%i];\n" % (contentName, faceCnt))
	file.write("extern int %s_POINTDATSZ[%i];\n" % (contentName, len(meshes)))
	file.write("extern int %s_LINEDATSZ[%i];\n" % (contentName, len(meshes)))
	file.write("extern int %s_COLORDATSZ[%i];\n\n" % (contentName, len(meshes)))

	file.write("//unchanging global transformation data\n")
	file.write("float %s_STATICGLOBAL[9];\n\n" % (contentName))

	file.write("//unchanging transformation data\n")
	file.write("float %s_STATICTRANS[%i];\n\n" % (contentName, len(meshes) * 9))

	file.write("//unchanging point data\n")
	file.write("float %s_STATICPOINT[%i];\n\n" % (contentName, vertCnt * 3))

	file.write("extern int %s_MESHCNT;\n" % (contentName))
	
	file.close()
	
	
def FileSelectorCB(file_name):
	if not file_name.lower().endswith('.c'):
		file_name += '.c'
	ExportToC(file_name)

if __name__ == '__main__':
	Blender.Window.FileSelector(FileSelectorCB, "Export to C", Blender.sys.makename(ext='.c'))
