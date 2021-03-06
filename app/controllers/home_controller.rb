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

  def weekdaycompletionsvsgradesclassification
    weekdayCompletionsVsGradesClassificationJSON = DataProcesserCoupling::getDataProcesser.getWeekdayCompletionsVsGradesClassification(getPercentage())
    render json: weekdayCompletionsVsGradesClassificationJSON
  end

  def weekendcompletionsvsgradesclassification
    weekendCompletionsVsGradesClassificationJSON = DataProcesserCoupling::getDataProcesser.getWeekendCompletionsVsGradesClassification(getPercentage())
    render json: weekendCompletionsVsGradesClassificationJSON
  end

  def daycompletionsvsgradesclassification
    dayCompletionsVsGradesClassificationJSON = DataProcesserCoupling::getDataProcesser.getDayCompletionsVsGradesClassification(getPercentage())
    render json: dayCompletionsVsGradesClassificationJSON
  end

  def nightcompletionsvsgradesclassification
    nightCompletionsVsGradesClassificationJSON = DataProcesserCoupling::getDataProcesser.getNightCompletionsVsGradesClassification(getPercentage())
    render json: nightCompletionsVsGradesClassificationJSON
  end

  def attemptsvsfailures
    attemptsVsFailuresJSON = DataProcesserCoupling.getDataProcesser.getAttemptsVsFailures()
    render json: attemptsVsFailuresJSON
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

  def filteroutliers
    xyValuesArray = getXYValues

    filteredData = DataProcesserCoupling::getDataProcesser.filterOutliers(xyValuesArray)
    render json: filteredData 
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
