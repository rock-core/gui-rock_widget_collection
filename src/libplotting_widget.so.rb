#prepares the c++ qt widget for the use in ruby with widget_grid

require 'Qt4'

WidgetFactory::extend_cplusplus_widget do 
  def types
    return []
  end

  def default_options()
    options = Hash.new
    options[:min_width] = 200
    options[:min_height] = 200
    options[:draw_grid] = true
    options[:autoscrolling] = true
    options[:enable_slider_x] = false
    options[:enable_slider_y] = false
    options[:axis_title_x] = "Test"
    options[:axis_title_y] = "Test"
    options[:x_lower_bound] = 0
    options[:x_upper_bound] = 1000
    options[:y_lower_bound] = 0
    options[:y_upper_bound] = 100
    options[:x_value_method] = nil
    options[:y_value_method] = nil
    return options
  end

  def default_display_widget
    types
  end

  def config(options= Hash.new)
    puts "Configuring Plotting widget"
    @time_base = -1
    @options ||= default_options()
    @options = @options.merge(options)
    setDrawGrid(@options[:draw_grid])
    setAxisBoundaries(0, @options[:x_lower_bound], @options[:x_upper_bound])
    setAxisBoundaries(1, @options[:y_lower_bound], @options[:y_upper_bound])
    setAutoscrolling(@options[:autoscrolling])
    @options[:y_value_method] = "temperatureFrontLeft"
    @options[:x_value_method] = "timestamp"
    return @options
  end

  #diplay is called each time new data are available on the orocos output port
  #this functions translates the orocos data struct to the widget specific format
  def display(port,frame)
    all_methods =  frame.methods
    puts @options[:y_value_method]
    y_value = 0
    x_value = 0
    for i in 0..all_methods.length
      method = all_methods[i]
      if(method != nil)
        if(method.eql?(@options[:y_value_method]))
          call_method = frame.method(method)
          y_value = call_method.call
        elsif(method.eql?(@options[:x_value_method]))
          call_method = frame.method(method)
          x_value = call_method.call
        end
      end
    end
    if(@time_base == -1)
      @time_base = x_value
    end
    addData((x_value - @time_base)/100, y_value, 20)
  end
end
