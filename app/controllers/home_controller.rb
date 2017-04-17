require_dependency 'dataprocesserglue'

class HomeController < ApplicationController
  include DataProcesserCoupling

  def index
    @title = DataProcesserCoupling::test
  end
end
