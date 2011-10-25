require 'vizkit'
require 'orocos/log'


require 'octave'
engine = Octave::Engine.new

log = Orocos::Log::Replay.open(ARGV)
widget = Vizkit.default_loader.SonarDisplay

log.sonar_rear.BaseScan.connect_to do |data,_|
  a = engine.mytest(to_octave(data))
  from_octave(data,a)
  widget.addSonarBeam(data.bearing.rad+Math::PI,data.beam.size-8,data.beam.to_byte_array[8..-1])
  data
end

widget.show

Vizkit.control log
Vizkit.exec
