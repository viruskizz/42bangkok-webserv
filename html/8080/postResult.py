content = ""
while True:
	try:
		line = input() + "\n"
	except EOFError:
		break
	content = content + line

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
print(f'		<hr><p style="color= red;">{content}</p><hr>')
print('		<b># The server has processed your input and returned this answer.</v>')
print('		<p></p>')
print('		<center>')
print('			<a href="./postMethod.html"><button class="button">Go Back</button>')
print('		</center>')
print('	</body>')
print('</html>')
