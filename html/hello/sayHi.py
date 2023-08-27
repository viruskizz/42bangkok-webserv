name = input()
content = "<h1>hello " + name + ".</h1>";


print("HTTP/1.1 OK 200", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print(f"Content-length: {len(content)}", end="\r\n\r\n")

print(content, end=None);