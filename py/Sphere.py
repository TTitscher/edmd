#!/usr/bin/env python

import vtk
import numpy as np
import time

class Sphere:
    def __init__(self, position, radius, velocity):
        self._p = position
        self._r = radius
        self._v = velocity

    def update(self, dt):
        self._p += self._v * dt
        self._r += dt
        if (self._p[0] > 10):
            self._v[0] = -10;
        if (self._p[0] < -10):
            self._v[0] = 10;

def ReferenceSphere(resolution = 20):
    sphere = vtk.vtkSphereSource()
    sphere.SetThetaResolution(resolution)
    sphere.SetPhiResolution(resolution/2)
    return sphere

def DefineVisuData(spheres):
    positions = vtk.vtkPoints()
    radii = vtk.vtkFloatArray()
    velocities = vtk.vtkPoints()
    for sphere in spheres:
        positions.InsertNextPoint(sphere._p)
        radii.InsertNextValue(sphere._r)
        velocities.InsertNextPoint(sphere._v)
    return positions, radii, velocities

def UpdateVisualization(spheres, visuData):
    positions, radii, velocities = visuData 
    for i in range(len(spheres)):
        positions.SetPoint(i, spheres[i]._p)
        radii.SetValue(i, spheres[i]._r)
        velocities.SetPoint(i, spheres[i]._v)


spheres = list()
for i in range(100):
    spheres.append(Sphere(np.array([0.,3.*i,0.]), 1, np.array([10.,0.,0.])))

visuData = DefineVisuData(spheres)
positions, radii, velocities = visuData 
polydata = vtk.vtkPolyData()
polydata.SetPoints(positions)
polydata.GetPointData().SetScalars(radii)

glyph = vtk.vtkPolyData()
sphereSource = ReferenceSphere()

glyph3Dmapper = vtk.vtkGlyph3DMapper()
glyph3Dmapper.SetSourceConnection(sphereSource.GetOutputPort())
glyph3Dmapper.SetInputData(polydata)
glyph3Dmapper.Update()

actor = vtk.vtkActor()
actor.SetMapper(glyph3Dmapper)

# create a rendering window and renderer
renderer = vtk.vtkRenderer()
renderer.SetBackground(0.4, 0.6, 0.8)
renderer.AddActor(actor)

renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindow.SetSize(500,500)
 
# create a renderwindowinteractor
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)

# enable user interface interactor
renderWindowInteractor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
renderWindowInteractor.Initialize()




# actor

# assign actor to the renderer
# renderer.AddActor(actor)


dt = 0.020
timings = {'Update': 0., 'Render' :0.} 

txt = vtk.vtkTextActor()
txt.GetTextProperty().SetFontFamilyToArial()
txt.GetTextProperty().SetFontSize(18)
txt.SetDisplayPosition(20,30)
renderer.AddActor(txt)

def update_scene(*args):
    '''
    Update the sphere position
    '''
    t_start = time.time()

    for sphere in spheres:
        sphere.update(dt)
    UpdateVisualization(spheres, visuData)
    positions.Modified()
    t_update = time.time()

    renderWindow.Render()
    t_render = time.time()

    timings['Update'] +=  t_update - t_start
    timings['Render'] +=  t_render - t_update


    txt.SetInput("Frame took: " + str(t_render - t_start) + "s")


renderWindowInteractor.AddObserver('TimerEvent', update_scene)
renderWindowInteractor.CreateRepeatingTimer(20)

renderWindowInteractor.Start()
renderWindowInteractor.GetRenderWindow().Finalize()

print timings
