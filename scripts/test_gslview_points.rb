require 'vizkit'

Orocos.initialize
log = Orocos::Log::Replay.open ARGV

camera = log.camera
widget = Vizkit.default_loader.GstImageView

points_x = Array.new
points_y = Array.new
0.upto 100 do |i|
    points_x << 100
    points_y << i
end

camera.frame.connect_to widget
widget.show
widget.addPoints(points_x,points_y,Qt::Color.new(255,0,0),1,0)

Vizkit.control log
Vizkit.exec
