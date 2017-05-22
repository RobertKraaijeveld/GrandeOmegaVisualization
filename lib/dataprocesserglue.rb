require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end
#
cjishomo = [196.0, 100.0, 323.0, 75.0, 738.0, 75.0, 807.0, 100.0]
puts DataProcesserCoupling.getDataProcesser.getLinearRegression(cjishomo)