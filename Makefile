NAME = webserv

SRCS =	srcs/main.cpp \
		srcs/Webserver/Connection/on_connect.cpp \
		srcs/Webserver/Poll_loop/error.cpp \
		srcs/Webserver/Poll_loop/run.cpp \
		srcs/Webserver/Poll_loop/clean.cpp \
		srcs/Webserver/Poll_loop/pollout.cpp \
		srcs/Webserver/Init/init_config.cpp \
		srcs/Webserver/Init/init_sockets.cpp \
		srcs/Webserver/Requests/new_request.cpp \
		srcs/Webserver/Requests/peek.cpp \
		srcs/Webserver/Requests/handle_request.cpp \
		srcs/Webserver/Requests/route_data.cpp \
		srcs/Webserver/Requests/read_chunks.cpp \
		srcs/Webserver/Parsing/parsing.cpp \
		srcs/HTTPRequest/constructor.cpp \
		srcs/HTTPRequest/cgi.cpp \
		srcs/HTTPRequest/DELETE/action.cpp \
		srcs/HTTPRequest/GET/action.cpp \
		srcs/HTTPRequest/GET/list_directory_page.cpp \
		srcs/HTTPRequest/POST/constructor.cpp \
		srcs/HTTPRequest/POST/action.cpp \
		srcs/HTTPRequest/POST/getters.cpp \
		srcs/HTTPRequest/getters.cpp \
		srcs/HTTPRequest/get_requested_ressource.cpp \
		srcs/HTTPRequest/parse_url.cpp \
		srcs/HTTPRequest/server_block.cpp \
		srcs/tools/parse_request.cpp \
		srcs/tools/tools.cpp \
		srcs/CGI/cgi.cpp \
		srcs/CGI/getters.cpp \
		srcs/HTTPResponse/cgi.cpp \
		srcs/HTTPResponse/handle_cgi_return.cpp \
		srcs/HTTPResponse/HTTP_status_codes.cpp \
		srcs/HTTPResponse/constructor.cpp \
		srcs/HTTPResponse/error_pages.cpp \
		srcs/HTTPResponse/getters.cpp \
		srcs/HTTPResponse/reply.cpp

TEST_SRCS = srcs/Client/client.cpp \
			srcs/Client/connect.cpp \
			srcs/Client/generate_request.cpp \
			srcs/Client/get.cpp \
			srcs/Client/init.cpp \
			srcs/Client/post.cpp \
			srcs/Client/run.cpp

INCDIR = inc

CC = clang++

CPPSTD = -std=c++98 

CXXFLAGS = -Wall -Wextra -Werror -MMD -MP

DEBUG = -fstandalone-debug -g3

MEM = -fsanitize=address

DEPS = ${OBJS:.o=.d}

OBJDIR = objs

OBJS = $(addprefix ${OBJDIR}/, ${SRCS:.cpp=.o})

TEST_OBJS = $(addprefix ${OBJDIR}/, ${TEST_SRCS:.cpp=.o})

MKDIR_P = mkdir -p

${OBJDIR}/%.o:%.cpp
	@${MKDIR_P} ${@D}
	${CC} ${CXXFLAGS} ${MEM} ${DEBUG} -I./${INCDIR} ${CPPSTD} -c $< -o $@

all : ${NAME}

${NAME} : ${OBJS}
	@${CC} ${CXXFLAGS} -I./${INCDIR} ${MEM} ${DEBUG} ${CPPSTD} ${OBJS} -o $@

re : fclean all

CLIENT = test_client

client : ${CLIENT}

run : all
	./${NAME}

${CLIENT} : ${TEST_OBJS}
	@${CC} ${CXXFLAGS} ${MEM} ${TEST_OBJS} -o $@

test_get: client
	@./${CLIENT} 1 0 

clean :
	rm -rf ${OBJDIR}

fclean : clean
	rm -rf ${NAME} ${CLIENT}

.PHONY : all clean fclean re

-include ${DEPS}