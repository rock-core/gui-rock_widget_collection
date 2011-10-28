require 'vizkit'
require 'orocos/log'
Orocos.initialize


require 'octave'
engine = Octave::Engine.new

log = Orocos::Log::Replay.open(ARGV)
sonar_data = log.find_port("/base/samples/SonarBeam")
widget = Vizkit.default_loader.SonarDisplay

sonar_data.connect_to do |data,_|
  a = engine.mytest(to_octave(data))
  from_octave(data,a)
  widget.addSonarBeam(data.bearing.rad+Math::PI,data.beam.size-8,data.beam.to_byte_array[8..-1])
  data
end

widget.show

#load video stream
Vizkit.display log.bottom_camera.frame

Vizkit.control log
Vizkit.exec
