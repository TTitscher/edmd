#!/usr/bin/env python

import vtk
import numpy as np
import time
 
R_THETA = 20
R_PHI = 20

# create a rendering window and renderer
ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
 
# create a renderwindowinteractor
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

class Sphere:
    def __init__(self, appendData, position, radius, velocity):
        self._p = position
        self._r = radius
        self._v = velocity

        # create source
        self._source = vtk.vtkSphereSource()
        self._source.SetThetaResolution(R_THETA)
        self._source.SetPhiResolution(R_THETA)
        self.update(0.)
        appendData.AddInputConnection(self._source.GetOutputPort())


    def update(self, dt):
        self._p += self._v * dt
        self._source.SetCenter(self._p[0], self._p[1], self._p[2])
        self._source.SetRadius(self._r)
        if (self._p[0] > 10):
            self._v[0] = -10;
        if (self._p[0] < -10):
            self._v[0] = 10;

# enable user interface interactor
ren.SetBackground(0.4, 0.6, 0.8)
renWin.SetSize(500,500)
iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
iren.Initialize()

appendData = vtk.vtkAppendPolyData()

spheres = list()
for i in range(10):
    spheres.append(Sphere(appendData, np.array([0.,3.*i,0.]), 1, np.array([10.,0.,0.])))

# mapper
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(appendData.GetOutputPort())

# actor
actor = vtk.vtkActor()
actor.SetMapper(mapper)

# assign actor to the renderer
ren.AddActor(actor)


dt = 0.020
timings = {'Update': 0., 'Render' :0.} 

txt = vtk.vtkTextActor()
txt.GetTextProperty().SetFontFamilyToArial()
txt.GetTextProperty().SetFontSize(18)
txt.SetDisplayPosition(20,30)
ren.AddActor(txt)

def update_scene(*args):
    '''
    Update the sphere position
    '''
    t_start = time.time()

    for sphere in spheres:
        sphere.update(dt)   
    t_update = time.time()

    renWin.Render()
    t_render = time.time()

    timings['Update'] +=  t_update - t_start
    timings['Render'] +=  t_render - t_update


    txt.SetInput("Frame took: " + str(t_render - t_start) + "s")


iren.AddObserver('TimerEvent', update_scene)
iren.CreateRepeatingTimer(20)

iren.Start()
iren.GetRenderWindow().Finalize()

print timings
