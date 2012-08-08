#!/usr/bin/env ruby

require 'vizkit'

plot = Vizkit.default_loader.Plot2d

timer = Qt::Timer.new 
timer.connect SIGNAL("timeout()") do
    @val ||= 0
    plot.update(4.0*Math::sin(@val.to_f/180*Math::PI),"sin")
    plot.update(4.0*Math::cos(@val.to_f/180*Math::PI),"cos")
    @val +=5
end
timer.start(100)
plot.show
Vizkit.exec


