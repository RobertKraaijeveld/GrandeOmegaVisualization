require_dependency 'dataprocesserglue'
require 'rubygems'
require 'json'
require 'pp'

class HomeController < ApplicationController
  include DataProcesserCoupling

  def index
    gradeAvgsAsJson = DataProcesserCoupling::getGradeAvgPerClassAsJSON
  end
end
