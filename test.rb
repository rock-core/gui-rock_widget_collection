require 'vizkit'
require 'orocos/log'


log = Orocos::Log::Replay.open(ARGV)
widget = Vizkit.default_loader.SonarDisplay

log.sonar_rear.BaseScan.connect_to do |data,_|
    widget.addSonarBeam(data.bearing.rad+Math::PI,data.beam.size-8,data.beam.to_byte_array[8..-1])
end

widget.show

Vizkit.control log
Vizkit.exec
