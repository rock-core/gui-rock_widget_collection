#!/usr/bin/env ruby

require 'vizkit'

gui = Vizkit.default_loader.Vectorfield3D

gui.addVector(0.0,0.0,0.0,1.0,0.0,0.0)
gui.addVector(0.0,0.0,0.0,1.0,1.0,0.0)
#gui.showScalarBar(false)
gui.setScalarBarTitle('Foo Bar flux (T)')
gui.setTitle("Test Vectorfield3D Visualization")
gui.showOrientationMarker(true)
gui.show
Vizkit.exec


