import time

print("HTTP/1.1 OK 200", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print("Content-length: 19", end="\r\n\r\n")
print("<h1>helloWorld</h1>", end=None);
