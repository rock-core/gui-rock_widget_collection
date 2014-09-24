#!/usr/bin/ruby
##############################################
# Small script for the virtual joystick plugin
# Author: leif.christensen@dfki.de
# Date:   09/19/2014 
##############################################

require 'vizkit'
require 'optparse'
include Orocos

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

  options[:maxspeed] = 1.0 # m/s
  opts.on("--maxspeed [MAXSPEED]", Float, "Maximum speed (m/s)") do |speed|
    options[:maxspeed] = speed
  end
  
  options[:maxrotspeed] = 0.5 #rad/s 
  opts.on("--maxrotspeed [MAXROTSPEED]", Float, "Maximum rotational velocity (rad/s)") do |rot|
    options[:maxrotspeed] = rot
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
  g = lambda{|a,b| sample.translation = a * options[:maxspeed]
                   sample.rotation = -b * options[:maxrotspeed] 
                   sample}
  connect SIGNAL("axisChanged(double,double)"), PORT(options[:port]), :getter => g 

  task.on_reachable {joystickGui.setEnabled(true)}
  task.on_unreachable {joystickGui.setEnabled(false)}
end

Vizkit.exec

