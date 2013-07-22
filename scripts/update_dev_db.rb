
require 'rubygems'
require 'active_record'

ActiveRecord::Base.establish_connection(
  :adapter  => "mysql",
  :host     => "localhost",
  :database => "go_maritimes"
)

class DbUpdate < ActiveRecord::Base

  DbUpdate.connection.select_all("select * from agencies") do |agency|
    puts agency.agency_name
  end

end
