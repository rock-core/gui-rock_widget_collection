#!/usr/bin/env ruby

require 'orocos/log'
#require 'testgui.rb'
require 'vizkit'

@line_ctr = 0;
@line_direction = 1;
@line_color = Qt::Color.new(Qt::green)
@line_width = 1

@frame_width = 600
@frame_height = 300

def rotate
    @view.rotate(90)
end

def add_line
    @view.addLine(Qt::LineF.new(@line_ctr, 0, @line_ctr, @frame_height-1), @line_color, @line_width, false)
    if(@line_ctr == @frame_height)
        @line_direction = -1
    elsif(@line_ctr == 0)
        @line_direction = 1
    end
    @line_ctr = @line_ctr + @line_direction
end

def add_circle
    max_radius = 100
    @view.addCircle(Qt::PointF.new(rand(@frame_width), rand(@frame_height)), rand(max_radius), true)
end

def clear_overlays
    @view.clearOverlays(true);
end

# ---------------------------------------

if ARGV.length != 1
    puts "Please supply a logfile as argument."
    exit
end


@view = Vizkit.default_loader.GstImageView
#@oldview = Vizkit.default_loader.ImageView
#@oldview.Aspect_Ratio=true
#@oldview.show
#@property_control = Vizkit.default_loader.property_control
#@property_control.show

@view.progress_indicator_timeout = 2500
@view.use_smooth_transformation = true

#@view.extend Vizkit::QtTypelibExtension
@view.show

#@view.addCircle(Qt::PointF.new(0,0), 50, true);

#@view.addLine(Qt::LineF.new(200, 0, 200, 300))
#@view.addLine(Qt::LineF.new(250, 0, 250, 300))

@testgui = Vizkit.load(File.join(File.dirname(__FILE__),'testgui.ui'))
@testgui.show

@testgui.rotate_button.connect(SIGNAL('clicked()')) {rotate}
@testgui.line_button.connect(SIGNAL('clicked()')) {add_line}
@testgui.circle_button.connect(SIGNAL('clicked()')) {add_circle}
@testgui.clear_overlays_button.connect(SIGNAL('clicked()')) {clear_overlays}

@view.addLine(Qt::LineF.new(0, 0, 300, 200), Qt::Color.new(Qt::yellow), 5.0, true)
#puts :TOPRIGHT
#@view.addText("AVALON im Studiobad", :TOPRIGHT, Qt::Color.new(Qt::green), true)

log = Orocos::Log::Replay.open(ARGV[0])

log.front_camera.frame.connect_to @view
#log.front_camera.frame.connect_to @oldview

Vizkit.control log

Vizkit.exec
