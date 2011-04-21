#!/usr/bin/env ruby

require 'vizkit'
# change the path to a correct one
multi = Vizkit.load("/home/blueck/temp/test/tryit.ui")
#multi.multiViewWidget.setDesignerMode(false)

multi.show
Vizkit.exec
