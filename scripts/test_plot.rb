#!/usr/bin/env ruby

require 'vizkit'

plot = Vizkit.default_loader.PlotWidget

x = [1.0,2.0,3.0,4.0,5.0]
y = [1.0,2.0,3.0,4.0,5.0]


plot.addPoints(x,y,1,0,1)
plot.show
Vizkit.exec


