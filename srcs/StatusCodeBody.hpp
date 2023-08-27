#ifndef STATUSCODEBODY_HPP
# define STATUSCODEBODY_HPP

/*
* marco of bodyContent for status code */
# define BODY201 "<!DOCTYPE html>\n" 						\
					"<html>\n" 								\
					"\t<body>\n"						    \
						"\t\t<center>\n"					\
						"\t\t<h1>201 Created</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"				\
					    "\t\t</center>\n"					\
					"\t</body>\n</html>\n"

// # define BODY204 "<!DOCTYPE html>\n" 					\
// 					"<html>\n" 								\
// 					"\t<body>\n"						    \
// 						"\t\t<center>\n"					\
// 						"\t\t<h1>204 No Content/h1><hr>\n"	\
// 						"\t\t<p>webserv/</p>\n"				\
// 					    "\t\t</center>\n"					\
// 					"\t</body>\n</html>\n"

# define BODY204 ""

# define BODY400 "<!DOCTYPE html>\n" 							\
					"<html>\n" 									\
					"\t<body>\n"						    	\
						"\t\t<center>\n"						\
						"\t\t<h1>400 Bad Request</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"					\
					    "\t\t</center>\n"						\
					"\t</body>\n</html>\n"

# define BODY401 "<!DOCTYPE html>\n" 							\
					"<html>\n" 									\
					"\t<body>\n"						    	\
						"\t\t<center>\n"						\
						"\t\t<h1>401 Unauthorized</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"					\
					    "\t\t</center>\n"						\
					"\t</body>\n</html>\n"

# define BODY403 "<!DOCTYPE html>\n" 							\
					"<html>\n" 									\
					"\t<body>\n"						    	\
						"\t\t<center>\n"						\
						"\t\t<h1>403 Forbidden</h1><hr>\n"		\
						"\t\t<p>webserv/</p>\n"					\
					    "\t\t</center>\n"						\
					"\t</body>\n</html>\n"

# define BODY404 "<!DOCTYPE html>\n" 						\
					"<html>\n" 								\
					"\t<body>\n"						    \
						"\t\t<center>\n"					\
						"\t\t<h1>404 Not Found</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"				\
					    "\t\t</center>\n"					\
					"\t</body>\n</html>\n"

# define BODY405 "<!DOCTYPE html>\n" 								\
					"<html>\n" 										\
					"\t<body>\n"							    	\
						"\t\t<center>\n"							\
						"\t\t<h1>405 Method Not Allowed</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"						\
					    "\t\t</center>\n"							\
					"\t</body>\n</html>\n"

# define BODY408 "<!DOCTYPE html>\n" 								\
					"<html>\n" 										\
					"\t<body>\n"								    \
						"\t\t<center>\n"							\
						"\t\t<h1>408 Request Timeout</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"						\
					    "\t\t</center>\n"							\
					"\t</body>\n</html>\n"

# define BODY500 "<!DOCTYPE html>\n" 									\
					"<html>\n" 											\
					"\t<body>\n"								    	\
						"\t\t<center>\n"								\
						"\t\t<h1>500 Internal Server Error</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"							\
					    "\t\t</center>\n"								\
					"\t</body>\n</html>\n"

# define BODY502 "<!DOCTYPE html>\n" 							\
					"<html>\n" 									\
					"\t<body>\n"						    	\
						"\t\t<center>\n"						\
						"\t\t<h1>502 Bad Gateway</h1><hr>\n"	\
						"\t\t<p>webserv/</p>\n"					\
					    "\t\t</center>\n"						\
					"\t</body>\n</html>\n"

// 503 Service Unavailable"
// 504 Gateway Timeout

#endif