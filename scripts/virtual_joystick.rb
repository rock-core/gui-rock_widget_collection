#!/usr/bin/ruby

require 'vizkit'
require 'optparse'
include Orocos

maxSpeed = 0.4 #SeekurJr=1.5 m/s
maxJoyRotSpeed = 20 # degree/s
options = {}

optparse = OptionParser.new do |opts|
  opts.banner = "Usage: #{opts.program_name} --task TASK --port PORT [--host HOST]"
  opts.on("--task TASK", "Task to connect virtual joystick to") do |task|
    options[:task] = task
  end
  
  opts.on("--port PORT","Port to connect virtual joystick to (of type Motion2D)") do |p|
    options[:port] = p
  end

  options[:host] = 'localhost'
  opts.on("--host [HOST]","Host, where the corba nameserver is running") do |host|
    options[:host] = host
  end

  opts.on("-h","--help","Display this help screen") do
    puts opts
    exit
  end
end

optparse.parse!

if options[:task].nil? || options[:port].nil? 
  puts optparse
  exit
end

Orocos::CORBA.name_service.ip = options[:host]
Orocos.initialize
Orocos.load_typekit_for "/base/commands/Motion2D"

sample = Types::Base::Commands::Motion2D.new 

joystickGui = Vizkit.default_loader.VirtualJoystick
joystickGui.show

joystickGui.connect_to_task options[:task] do |task|
  c = lambda{|msg| puts msg}
  g = lambda{|a,b| sample.translation = a * maxSpeed
                   sample.rotation = -b * maxJoyRotSpeed * Math::PI/180
                   sample}
  connect SIGNAL("axisChanged(double,double)"), PORT(options[:port]), :getter => g, :callback => c 

  task.on_reachable {joystickGui.setEnabled(true)}
  task.on_unreachable {joystickGui.setEnabled(false)}
end

Vizkit.exec

