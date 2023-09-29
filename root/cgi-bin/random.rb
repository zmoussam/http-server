#!/usr/bin/env ruby

require 'cgi'

cgi = CGI.new

# Generate a random number between 1 and 100
random_number = rand(1..100)

# Set the content type to HTML
cgi.out("text/html") do
  "<html>
  <head>
    <title>Random Number Generator</title>
  </head>
  <body>
    <h1>Random Number:</h1>
    <p>#{random_number}</p>
  </body>
</html>"
end
