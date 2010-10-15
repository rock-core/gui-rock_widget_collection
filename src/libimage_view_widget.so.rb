#prepares the c++ qt widget for the use in ruby with widget_grid
  
  def __plugin_widget__.types
    return ['/base/samples/frame/Frame','/base/samples/frame/FramePair']
  end

  def __plugin_widget__.default_options()
      options = Hash.new
      options[:do_init] = true
      options[:min_width] = 640
      options[:min_height] = 480
      options[:aspect_ratio] = true
      options[:fixed_size] = false
      options[:time_overlay] = true
      options[:fps_overlay] = true
      options[:display_first] = true
      return options
  end

  def __plugin_widget__.default_display_widget
    types
  end

  def __plugin_widget__.config(options= Hash.new)
    @options ||= default_options()
    @options = @options.merge(options)
    return @options
  end

  def __plugin_widget__.save(path)
	saveImage2(path)
  end

  def __plugin_widget__.save_frame(frame,path)
        saveImage3(frame.frame_mode.to_s,frame.pixel_size,frame.size.width,frame.size.height,frame.image.to_byte_array[8..-1],path)
  end

  #diplay is called each time new data are available on the orocos output port
  #this functions translates the orocos data struct to the widget specific format
  def __plugin_widget__.display(port,frame)
    #check if type is frame_pair
    if frame.respond_to?(:first)
      frame = @options[:display_first] == true ? frame.first : frame.second
    end

    if(@options[:do_init]==true)
      width = [frame.size.width,@options[:min_width]].max
      height = [frame.size.height,@options[:min_height]].max
      setMinimumSize(width,height)
      setAspectRatio(@options[:aspect_ratio]);
      setFixedSize(@options[:fixed_size]);	
      
 #     @time_overlay_object = addText(width*@options[:scale_factor]-150,height*@options[:scale_factor]-5,0,"time")
 #     @time_overlay_object.setColor(Qt::Color.new(255,255,0))
 #     @time_overlay_object.setBackgroundColor(Qt::Color.new(0,0,0,40))
 #     @fps_overlay_object = addText(5,height*@options[:scale_factor]-5,0,"   ")
 #     @fps_overlay_object.setColor(Qt::Color.new(255,255,0))
 #     @fps_overlay_object.setBackgroundColor(Qt::Color.new(0,0,0,40))

      @options[:do_init] = false
    end
    if @options[:time_overlay1] == true
      if frame.time.instance_of?(Time)
        time = frame.time
      else
        time = Time.at(frame.time.seconds,frame.time.microseconds)
      end
      @time_overlay_object.setText(time.strftime("%b %d %Y %H:%M:%S"))
    end
    if @options[:fps_overlay1]
      stat = ''
      stat_valid = ''
      stat_invalid = ''
      frame.attributes.to_a.each do |x|
        stat =x.data_.to_s if x.name_ == 'StatFps'
        stat_valid =x.data_.to_s if x.name_ == 'StatValidFps'
        stat_invalid =x.data_.to_s if x.name_ == 'StatInValidFps'
      end
      @fps_overlay_object.setText(" stat fps: #{stat},  valid #{stat_valid}, invalid #{stat_invalid}")
    end
    addRawImage(frame.frame_mode.to_s,frame.pixel_size,frame.size.width,frame.size.height,frame.image.to_byte_array[8..-1])
  end
