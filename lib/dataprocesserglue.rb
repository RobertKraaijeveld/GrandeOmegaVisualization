require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

x = [1.0, 2.0, 3.0, 5.0]
puts DataProcesserCoupling.getDataProcesser.getLinearRegression(x)