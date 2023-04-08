require "rake"
require "rake/extensiontask"

Rake::ExtensionTask.new do |ext|
  ext.name = "circular_buffer_ivar"
  ext.ext_dir = "ext_ivar"
  ext.lib_dir = "."
end

Rake::ExtensionTask.new do |ext|
  ext.name = "circular_buffer_typeddata"
  ext.ext_dir = "ext_typeddata"
  ext.lib_dir = "."
end

task benchmark: [:"compile:circular_buffer_ivar", :"compile:circular_buffer_typeddata"] do
  sh("bundle exec #{FileUtils::RUBY} benchmark.rb")
end
