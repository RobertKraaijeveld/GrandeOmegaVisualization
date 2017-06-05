require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

xy = [195,72,174,73,320,74,190,75,170,76]
#xy = [35,30,23,33,47,45,17,23,10,8,43,49,9,12,6,4,28,31]
puts DataProcesserCoupling.getDataProcesser.getCorrelationMeasures(xy)