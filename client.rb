require 'socket'

s = TCPSocket.new 'localhost', 8080

s.write("#{ARGV[0]}")
puts "the value passed is #{ARGV[0]}"

s.each_line do |line|
	puts line
end

s.close