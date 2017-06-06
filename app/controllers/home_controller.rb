require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling

  def index
  end 

  #Receiving JSON
  def gradeavgs
    gradeAvgsPerClassJSON = DataProcesserCoupling::getDataProcesser.getGradeAvgPerClass
    render json: gradeAvgsPerClassJSON
  end

  def amountofstudentsperclass
    amountOfStudentsPerClassJSON = DataProcesserCoupling::getDataProcesser.getAmountOfStudentsPerClass
    render json: amountOfStudentsPerClassJSON
  end

  def kmeans
    gradesAndExcerisesClustersJSON = DataProcesserCoupling::getDataProcesser.getKMeans(getPercentage()) 
    render json: gradesAndExcerisesClustersJSON
  end 

  def successrate
    studentsGradesAndSuccesRateJSON = DataProcesserCoupling::getDataProcesser.getSuccesRate(getPercentage())
    render json: studentsGradesAndSuccesRateJSON
  end


  #Sending and receiving JSON
  def linearregression
    xyValuesArray = getXYValues

    regressionLineJSON = DataProcesserCoupling::getDataProcesser.getLinearRegression(xyValuesArray)
    render json: regressionLineJSON
  end

  def logregression
    xyValuesArray = getXYValues

    logRegressionLineJSON = DataProcesserCoupling::getDataProcesser.getLogarithmicLinearRegression(xyValuesArray)
    render json: logRegressionLineJSON
  end

  def correlation
    xyValuesArray = getXYValues

    correlationJSON = DataProcesserCoupling::getDataProcesser.getCorrelationMeasures(xyValuesArray)
    render json: correlationJSON 
  end


  #Param Fetching and misc.
  def getPercentage
    params.fetch(:percentage).to_i
  end

  def getXYValues
    xyValuesArray = JSON.parse(params.fetch(:xyvalues))

    #parsing each value to float
    xyValuesArray.collect do |value|
      value.to_f 
    end
  end

end
