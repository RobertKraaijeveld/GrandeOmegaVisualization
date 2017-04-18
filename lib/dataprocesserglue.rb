require_relative './dataprocesser' 

#TODO: just get a ref to a dataprocesser and call its' methods instead
module DataProcesserCoupling    
    def DataProcesserCoupling::getGradeAvgPerClassAsJSON
        Dataprocesser.new.getGradeAvgPerClassAsJSON()
    end

    def DataProcesserCoupling::getAmountOfStartedExcersisesPerStudentAsJSON
        Dataprocesser.new.getAmountOfStartedExcersisesPerStudentAsJSON()
    end

    def DataProcesserCoupling::getKMeansAsJSON
        Dataprocesser.new.getKMeansAsJSON()
    end
end

puts DataProcesserCoupling::getAmountOfStartedExcersisesPerStudentAsJSON
puts DataProcesserCoupling::getGradeAvgPerClassAsJSON
puts DataProcesserCoupling::getKMeansAsJSON