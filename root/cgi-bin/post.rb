#!/usr/bin/env ruby

require 'cgi'

cgi = CGI.new

# Function to generate a random number within a range
def generate_random_number(min, max)
  rand(min..max)
end

# Process the form data if it's a POST request
if cgi.request_method == "POST"
  min_range = cgi.params['min_range'][0].to_i
  max_range = cgi.params['max_range'][0].to_i
  random_number = generate_random_number(min_range, max_range)

  # Set the content type to HTML
  cgi.out("text/html") do
    "<html>
    <head>
      <title>Random Number Generator</title>
    </head>
    <body>
      <h1>Random Number:</h1>
      <p>#{random_number}</p>
      <a href=\"#{cgi.script_name}\">Generate Another Random Number</a>
    </body>
  </html>"
  end
else
  # Set the content type to HTML
  cgi.out("text/html") do
    "<html>
    <head>
      <title>Random Number Generator</title>
    </head>
    <body>
      <h1>Generate Random Number</h1>
      <form method=\"post\" action=\"#{cgi.script_name}\">
        <label for=\"min_range\">Minimum Range:</label>
        <input type=\"number\" id=\"min_range\" name=\"min_range\" required><br>
        <label for=\"max_range\">Maximum Range:</label>
        <input type=\"number\" id=\"max_range\" name=\"max_range\" required><br>
        <input type=\"submit\" value=\"Generate\">
      </form>
    </body>
  </html>"
  end
end
