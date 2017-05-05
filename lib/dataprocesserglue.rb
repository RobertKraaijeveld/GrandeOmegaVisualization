require_relative './dataprocesser' 

#TODO: just get a ref to a dataprocesser and call its' methods instead
module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

# getSuccesRate
puts DataProcesserCoupling.getDataProcesser.getSuccesRate(100)