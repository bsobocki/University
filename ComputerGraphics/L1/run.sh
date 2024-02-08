PROGRAM="xmastree"

PROJECT_DIR="$(pwd)"

COMPILER="g++"
FLAGS="-I. -lGLEW -lGL -lglfw"

FILES="show_xmastree.cpp common/shader.cpp "

BIN_DIR="bin"
SRC_DIR="src"
PROG_PATH="$PROJECT_DIR/$BIN_DIR/$PROGRAM"

RUN="$PROJECT_DIR/$BIN_DIR/$PROGRAM"
COMPILE="$COMPILER $FILES -o $PROG_PATH $FLAGS"

if [ ! -d $BIN_DIR ]; then 
   mkdir $BIN_DIR
fi 

cd $SRC_DIR
$COMPILE
$RUN