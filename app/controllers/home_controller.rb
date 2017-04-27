require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling
  helper_method :kmeans

  def index
    gradeAvgsAsJson = DataProcesserCoupling::getDataProcesser.getGradeAvgPerClassAsJSON
    #make this a method and return
    parsedGradeAvgs = JSON.parse(gradeAvgsAsJson)
    @avgGradePerClass = parsedGradeAvgs

    startedExcersisesAmount = DataProcesserCoupling::getDataProcesser.getAmountOfStartedExcersisesPerStudentAsJSON
    parsedExcersiseAmounts = JSON.parse(startedExcersisesAmount)
    @excersisesAmounts = parsedExcersiseAmounts
  end

  def kmeans
    upperPercentageOfGradesToBeSelected = params.fetch(:percentage).to_i
    gradesAndExcerisesClustersJSON = DataProcesserCoupling::getDataProcesser.getKMeansAsJSON(upperPercentageOfGradesToBeSelected) 

    gradesAndExcerisesClustersParsed = JSON.parse(gradesAndExcerisesClustersJSON)
    render json: gradesAndExcerisesClustersParsed
  end 

end
