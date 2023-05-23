ifndef MODE
PHP_MODE := cli
else
PHP_MODE := ${MODE}
endif

ifeq ($(PHP_MODE), cli)
else ifeq ($(PHP_MODE), fpm)
else ifeq ($(PHP_MODE), apache2)
else
override PHP_MODE := cli
endif

PHP_VERSION		= $(shell /usr/bin/php --version | head -n 1 | cut -d " " -f 2 | cut -c 1-3)
PHP_API_VER		= $(shell /usr/bin/php -i | /bin/grep 'PHP API' | cut -d " " -f 4)

NAME			= openai

INI_DIR			= /etc/php/${PHP_VERSION}/${PHP_MODE}/conf.d

EXTENSION_DIR	= $(shell php-config --extension-dir)

EXTENSION		= ${NAME}.so
INI				= ${NAME}.ini
PRIORITY		= 30

COMPILER		= g++
LINKER			= g++

COMPILER_FLAGS	= -I ../php-cpp/include -I /usr/include/php/20210902/Zend -I /usr/local/include -I /usr/include/php/20210902/TSRM -I/usr/include/x86_64-linux-gnu -Wall -c -O2 -std=c++11 -fpic -o

LINKER_FLAGS	= -shared
LINKER_DEPENDENCIES	= -lphpcpp -lcurl

RM				= rm -f
CP				= cp -f
MKDIR			= mkdir -p

SOURCES			= $(wildcard *.cpp)
OBJECTS			= $(SOURCES:%.cpp=%.o)

all:			${OBJECTS} ${EXTENSION}

${EXTENSION}:	${OBJECTS}
				${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
				${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

install:
				${CP} ${EXTENSION} ${EXTENSION_DIR}
				${CP} ${INI} ${INI_DIR}/${PRIORITY}-${INI}

clean:
				${RM} ${EXTENSION} ${OBJECTS}
				${RM} /etc/php/${PHP_VERSION}/cli/conf.d/${PRIORITY}-${INI}
				${RM} ${EXTENSION_DIR}/${NAME}.so