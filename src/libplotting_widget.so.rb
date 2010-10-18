#prepares the c++ qt widget for the use in ruby with widget_grid

require 'Qt4'

WidgetFactory::extend_cplusplus_widget do 
  def types
    return []
  end

  def default_options()
    options = Hash.new
    options[:width_min] = 200
    options[:height_max] = 200
    options[:grid] = true
    options[:autoscrolling] = true
    options[:slider_x] = false
    options[:slider_y] = false
    options[:title_x] = "Test"
    options[:title_y] = "Test"
    options[:x_min] = 0
    options[:x_max] = 20
    options[:y_min] = 0
    options[:y_max] = 30
    options[:x] = nil
    options[:y] = nil
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
    setDrawGrid(@options[:grid])
    setAxisBoundaries(0, @options[:x_min], @options[:x_max])
    setAxisBoundaries(1, @options[:y_min], @options[:y_max])
    setAutoscrolling(@options[:autoscrolling])
  #  @options[:y] = :temperatureFrontLeft
  #  @options[:x] = :timestamp
    return @options
  end

  def value(data, arg)
    puts arg.class
    #check if arg is a method or a proc
    if arg.is_a?(Method) || arg.is_a?(Proc)
      if arg.arity == 1
        return arg.call data 
      else
        return arg.call
      end
    else
      return data.send(arg) if arg.is_a?(Symbol) && data.respond_to?(arg)
    end
    puts 'Cannot get plotting value. Set @options[x], @options[y] to proc, method or symbol.'
  end

  #diplay is called each time new data are available on the orocos output port
  #this functions translates the orocos data struct to the widget specific format
  def display(port,data)
    x_value = value(data,@options[:x])
    y_value = value(data,@options[:y])
    if(@time_base == -1)
      @time_base = x_value
    end
    addData((x_value - @time_base), y_value, 10)
  end
end
