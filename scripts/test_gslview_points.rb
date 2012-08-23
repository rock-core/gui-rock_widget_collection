require 'vizkit'

Orocos.initialize
log = Orocos::Log::Replay.open ARGV

camera = log.camera
widget = Vizkit.default_loader.ImageView

points_x = Array.new
points_y = Array.new
0.upto 100 do |i|
    points_x << 100
    points_y << i
end

camera.frame.connect_to widget
widget.addLine(Qt::LineF.new(10, 10, 300, 200), Qt::Color.new(Qt::yellow), 5.0, true)
widget.addPoints(points_x,points_y,Qt::Color.new(255,0,0),1,true)
widget.show

Vizkit.control log
Vizkit.exec
