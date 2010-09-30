#prepares the c++ qt widget for the use in ruby with widget_grid
  
  def __plugin_widget__.types
    return []
  end

  def __plugin_widget__.default_options()
      options = Hash.new
      options[:min_width] = 140
      options[:min_height] = 140
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

  #diplay is called each time new data are available on the orocos output port
  #this functions translates the orocos data struct to the widget specific format
  def __plugin_widget__.display(port,frame)
    
  end
