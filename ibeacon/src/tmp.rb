# Nathan Hinton

#B(x, P


def factorial(n)
  if n == 0
    1
  else
    (1..n).inject(:*)
  end
end


puts "Enter the value of P"
P = gets.chomp.to_f

puts "Enter the value of x"
x = gets.chomp.to_i

puts"calculating probability that x is at least 2, x: #{x}, P: #{P}"

result = 1
x.times do |idx|
  puts idx
  puts((factorial(x)/(factorial(x-idx) * factorial(idx)).to_f))
  result -= (factorial(x)/(factorial(x-idx) * factorial(idx)).to_f * P**(x-idx) * (1-P)**idx).to_f
end

puts result
