
import cgi
form = cgi.FieldStorage()


if 'm' in form and 'n' in form:
    try:
        multiplicand1 = int(form['m'].value)
        multiplicand2 = int(form['n'].value)
        product = multiplicand1 * multiplicand2
        print("<html><body>")
        print("<h1>Multiplication Result</h1>")
        print(multiplicand1)
        print("*")
        print(multiplicand2)
        print("=")
        print(product)
        print("</body></html>")
    except ValueError:
        print("<html><body>")
        print("<h1>Error</h1>")
        print("<p>Invalid input. Please enter valid numbers.</p>")
        print("</body></html>")
else:
    print("<html><body>")
    print("<h1>Error</h1>")
    print("<p>Missing parameters. Please provide both multiplicands.</p>")
    print("</body></html>")

