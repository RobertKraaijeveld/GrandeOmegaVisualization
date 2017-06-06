require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

#xy = [199,5,114,25,105,5,105,5,125,5,114,5,119,25,115,0,120,5,114,0,160,25,156,0,139,25,176,5,124,5,110,5,149,5,115,5,170,5,184,25,434,75,324,75,320,100,80]
puts DataProcesserCoupling.getDataProcesser.getSuccesRate(100);