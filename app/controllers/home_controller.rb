require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'

class HomeController < ApplicationController
  include DataProcesserCoupling

  def index
    gradeAvgsAsJson = DataProcesserCoupling::getDataProcesser.getGradeAvgPerClassAsJSON
    #make this a method and return
    parsedGradeAvgs = JSON.parse(gradeAvgsAsJson)
    @avgGradePerClass = parsedGradeAvgs

    startedExcersisesAmount = DataProcesserCoupling::getDataProcesser.getAmountOfStartedExcersisesPerStudentAsJSON
    parsedExcersiseAmounts = JSON.parse(startedExcersisesAmount)
    @excersisesAmounts = parsedExcersiseAmounts

    #LE TEST
    series_a = @excersisesAmounts
    series_b = @avgGradePerClass
    @testStr1 = [
                {name: "Series A", data: series_a},
                {name: "Series B", data: series_b}
              ]

    gradesAndExcerisesClustersJSON = DataProcesserCoupling::getDataProcesser.getKMeansAsJSON
    gradesAndExcerisesClustersParsed = JSON.parse(gradesAndExcerisesClustersJSON)
    @gradesAndExcerisesClusters = gradesAndExcerisesClustersParsed

    #MAKE HASH INSTEAD OF STRING
    #[{:name=>"Series A", :data=>{"1"=>"315","2"=>"300"}}]

    allClusterHashes = []
    clusterCounter = 0

    @gradesAndExcerisesClusters.each do |cluster| 
      clusterCounter += 1

      clusterHash = Hash.new
      clusterHash[:name] = "Cluster " + clusterCounter.to_s

      vectorHash = Hash.new      
      cluster.each do |vector|
        vectorHash[vector[0]] = vector[1]  
      end

      clusterHash[:data] = vectorHash
      allClusterHashes.push(clusterHash) 
    end
    @hashTest = allClusterHashes

  end

end
