require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling
  helper_method :completedexcersises  
  helper_method :gradeavgs  
  helper_method :kmeans
  helper_method :successrate  

  def index
  end

  def gradeavgs 
    gradeAvgsPerClassJSON = DataProcesserCoupling::getDataProcesser.getGradeAvgPerClass

    gradeAvgsPerClassParsed = JSON.parse(gradeAvgsPerClassJSON)
    render json: gradeAvgsPerClassParsed
  end

  def kmeans
    gradesAndExcerisesClustersJSON = DataProcesserCoupling::getDataProcesser.getKMeans(getPercentage()) 

    gradesAndExcerisesClustersParsed = JSON.parse(gradesAndExcerisesClustersJSON)
    render json: gradesAndExcerisesClustersParsed
  end 

  def successrate
    studentsGradesAndSuccesRateJSON = DataProcesserCoupling::getDataProcesser.getSuccesRate(getPercentage())
    
    studentsGradesAndSuccesRateParsed = JSON.parse(studentsGradesAndSuccesRateJSON)
    render json: studentsGradesAndSuccesRateParsed
  end

  def getPercentage
    params.fetch(:percentage).to_i
  end
end
