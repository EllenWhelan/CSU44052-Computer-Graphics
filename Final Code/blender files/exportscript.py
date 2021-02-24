# exports each selected object into its own file

import bpy
import os

# export to blend file location
basedir = os.path.dirname(bpy.data.filepath)

if not basedir:
    raise Exception("Blend file is not saved")

view_layer = bpy.context.view_layer

obj_active = view_layer.objects.active
selection = bpy.context.selected_objects

bpy.ops.object.select_all(action='DESELECT')
x = 0
for obj in bpy.context.scene.objects:

    obj.select_set(True)

    # some exporters only use the active object
    view_layer.objects.active = obj

    name = bpy.path.clean_name(obj.name)
    fn = os.path.join(basedir, name)
    if obj.type == 'MESH' and obj.name != "Plane":
        bpy.ops.export_scene.obj(filepath=f"{x}.obj", use_triangles=True, use_selection=True)
        f = open(f"{x}.matrix", "w" )

        f.write(f"{obj.location.x} {obj.location.y} {obj.location.z}")
        f.close()
        x+=1

    
    # Can be used for multiple formats
    # bpy.ops.export_scene.x3d(filepath=fn + ".x3d", use_selection=True)

    obj.select_set(False)

    print("written:", fn)
    

view_layer.objects.active = obj_active

for obj in selection:
    obj.select_set(True)