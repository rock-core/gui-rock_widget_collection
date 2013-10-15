#!/usr/bin/env ruby

require 'vizkit'
require 'vizkittypelib'

Orocos.initialize
Orocos.load_typekit "base"

gui = Vizkit.default_loader.Vectorfield3D
gui.extend(Vizkit::QtTypelibExtension)

# addVector method 1
gui.addVector(0.0,0.0,0.0,1.0,0.0,0.0)

# addVector method 2
pos = Types::Base::Position.new(2,2,0)
mag = Types::Base::Vector3d.new(0.4, 0.4, 0.2)
gui.addVector(pos,mag)

#gui.showScalarBar(false)
gui.setScalarBarTitle('Foo Bar flux (T)')
gui.setTitle("Test Vectorfield3D Visualization")
gui.showOrientationMarker(true)
gui.show
Vizkit.exec


