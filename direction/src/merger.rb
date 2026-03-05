#!/usr/bin/env ruby

require 'serialport'

BAUDRATE = 9600
TIMEOUT  = 1 # seconds

# --------------------------------
# Find first available serial device
# --------------------------------
def find_serial_device
  devices = Dir["/dev/ttyUSB*"] + Dir["/dev/ttyACM*"]
  devices.sort.each do |dev|
    begin
      sp = SerialPort.new(dev, BAUDRATE, 8, 1, SerialPort::NONE)
      sp.close
      return dev
    rescue
      next
    end
  end
  nil
end

device = "/dev/ttyUSB1"#find_serial_device
sp1 = SerialPort.new(device, BAUDRATE, 8, 1, SerialPort::NONE)
sp1.read_timeout = TIMEOUT * 1000 * 1
puts "Connected to #{device}"


device = "/dev/ttyUSB2"#find_serial_device
sp2 = SerialPort.new(device, BAUDRATE, 8, 1, SerialPort::NONE)
sp2.read_timeout = TIMEOUT * 1000 * 1
puts "Connected to #{device}"

output = SerialPort.new("/dev/pts/15", 9600, 8, 1, SerialPort::NONE)

puts "Merging data..."
while true do
    point1 = sp1.readline.chomp
    point2 = sp2.readline.chomp
    output.puts("#{point1},#{point2}")
end