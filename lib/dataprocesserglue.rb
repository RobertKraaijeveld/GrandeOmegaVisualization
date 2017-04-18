require_relative './dataprocesser' 


module DataProcesserCoupling    
    def DataProcesserCoupling::getGradeAvgPerClassAsJSON
        Dataprocesser.new.getGradeAvgPerClassAsJSON()
    end
end

puts DataProcesserCoupling::getGradeAvgPerClassAsJSON