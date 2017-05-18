require_relative './dataprocesser' 

module DataProcesserCoupling
    @dataprocesser = Dataprocesser.new 

    def self.getDataProcesser
        @dataprocesser
    end
end

puts DataProcesserCoupling.getDataProcesser.getGradeAvgPerClass()