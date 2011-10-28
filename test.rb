require 'vizkit'
require 'orocos/log'


log = Orocos::Log::Replay.open(ARGV)
sonar_data = log.port_for("base::samples::SonarBeam")
widget = Vizkit.default_loader.SonarDisplay

def test(data,port)
    puts port
end

sonar_data.connect_to method(:test) do |data,_|
    widget.addSonarBeam(data.bearing.rad+Math::PI,data.beam.size-8,data.beam.to_byte_array[8..-1])
end

widget.show

Vizkit.control log
Vizkit.exec
