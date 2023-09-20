content = ""
while True:
	try:
		line = input() + "\n"
	except EOFError:
		break
	content = content + line
contentLength = len(content) + 835
contentLength = str(contentLength)

print('HTTP/1.1 200 OK', end="\r\n")
print('Content-Type: text/html')
print(f'Content-Length: {contentLength}', end="\r\n")
print('', end="\r\n")
print('<!DOCTYPE html>')
print('<html>')
print('	<head>')
print('		<title> post_result </title>')
print('		<style>')
print('			html { color-scheme: light dark; }')
print('			body { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }')
print('			.button {	background-color: #4CAF50;')
print('			border: none;')
print('			color: white;')
print('			padding: 10px 20px;')
print('			text-align: center;')
print('			text-decoration: none;')
print('			display: inline-block;')
print('			font-size: 16px;')
print('			margin: 4px 2px;')
print('			cursor: pointer;')
print('			border-radius: 5px;')
print('		}')
print('		</style>')
print('	</head>')
print('	<body>')
print('		<center>')
print('			<h2 style="size: 50px"> Submitted Form Data </h2>')
print('		</center>')
print('		<h3 style="size: 30px">Your input was received as :</h3>')
print('		<hr><p></p><hr>')
print('		<b># The server has processed your input and returned this answer.</v>')
print(f'		<p>{content}</p>')
print('		<center>')
print('			<a href="./postMethod.html"><button class="button">Go Back</button><')
print('		</center>')
print('	</body>')
print('</html>')



# body = f'<!DOCTYPE html>\n<html>\n/t<html>\n/t/t<head>\n/t/t/t<title> post_result </title>\n/t/t/t<style>\n/t/t/t/thtml { color-scheme: light dark; }\n/t/t/t/tbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n/t/t/t/t.button { background-color: #4CAF50;\n/t/t/t/tborder: none;\n/t/t/t/tcolor: white;\n/t/t/t/tpadding: 10px 20px;\n/t/t/t/ttext-align: center;\n/t/t/t/ttext-decoration: none;\n/t/t/t/tdisplay: inline-block;\n
# /t/t/t/tfont-size: 16px;\n
# /t/t/t/tmargin: 4px 2px;\n
# /t/t/t/tcursor: pointer;\n
# /t/t/t/tborder-radius: 5px;\n
# /t/t/t}\n
# /t/t/t</style>\n
# /t/t</head>\n
# /t/t<body>\n
# /t/t/t<center>\n
# /t/t/t/t<h2 style="size: 50px"> Submitted Form Data </h2>\n
# /t/t/t</center>\n
# /t/t/t<h3 style="size: 30px">Your input was received as :</h3>\n
# /t/t/t<hr><p>hello</p><hr>\n
# /t/t/t<b># The server has processed your input and returned this answer.</v>\n
# /t/t/t<p></p>\n
# /t/t/t<center>\n
# /t/t/t/t<a href="./postMethod.html"><button class="button">Go Back</button></a>\n
# /t/t/t</center>\n
# /t/t</body>\n
# /t</html>\n
# </html>\n'


# print('<!DOCTYPE html>')
# print('<html>')
# print('/t<html>')
# print('/t/t<head>')
# print('/t/t/t<title> post_result </title>')
# print('/t/t/t<style>')
# print('/t/t/t/thtml { color-scheme: light dark; }')
# print('/t/t/t/tbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }')
# print('/t/t/t/t.button { background-color: #4CAF50;')
# print('/t/t/t/tborder: none;')
# print('/t/t/t/tcolor: white;')
# print('/t/t/t/tpadding: 10px 20px;')
# print('/t/t/t/ttext-align: center;')
# print('/t/t/t/ttext-decoration: none;')
# print('/t/t/t/tdisplay: inline-block;')
# print('/t/t/t/tfont-size: 16px;')
# print('/t/t/t/tmargin: 4px 2px;')
# print('/t/t/t/tcursor: pointer;')
# print('/t/t/t/tborder-radius: 5px;')
# print('/t/t/t}')
# print('/t/t/t</style>')
# print('/t/t</head>')
# print('/t/t<body>')
# print('/t/t/t<center>')
# print('/t/t/t/t<h2 style="size: 50px"> Submitted Form Data </h2>')
# print('/t/t/t</center>')
# print('/t/t/t<h3 style="size: 30px">Your input was received as :</h3>')
# print('/t/t/t<hr><p>hello</p><hr>')
# print('/t/t/t<b># The server has processed your input and returned this answer.</v>')
# print('/t/t/t<p></p>')
# print('/t/t/t<center>')
# print('/t/t/t/t<a href="./postMethod.html"><button class="button">Go Back</button></a>')
# print('/t/t/t</center>')
# print('/t/t</body>')
# print('/t</html>')
# print('</html>')
