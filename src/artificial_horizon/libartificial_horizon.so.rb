require 'eigen'

#prepares the c++ qt widget for the use in ruby with widget_grid
def __plugin_widget__.types
    return ['/wrappers/Orientation', '/wrappers/RigidBodyState', '/wrappers/Pose']
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
def __plugin_widget__.display(port, sample)
    if sample.respond_to?(:orientation) # pose and rigid body state
        sample = sample.orientation
    end

    if !sample.kind_of?(Eigen::Quaternion)
        # The base typelib plugin is not loaded, do the convertions by ourselves
        sample = Eigen::Quaternion.new(sample.re, *sample.im.to_a)
    end

    angles = sample.to_euler
    setPitchAngle(angles[1])
    setRollAngle(angles[2])
end

