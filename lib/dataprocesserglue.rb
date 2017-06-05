require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

xy = [1,2,3,4,5,7]
puts DataProcesserCoupling.getDataProcesser.getLogarithmicLinearRegression(xy);