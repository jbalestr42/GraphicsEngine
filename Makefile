.PHONY: all clean fclean re

NAME =	graphics.app
CC =	g++
FLAGS =	-g -Wall -Wextra -Werror -std=c++11

ifeq ($(shell uname), Linux)
	LIB =	-lGL -lGLEW -lm -lXrandr -lXi -lXcursor -lXinerama -lX11 -lXxf86vm -lpthread -ldl
else # update for Darwin
	LIB =	-L $(DEVIL)/lib -L $(GLFW)/lib -L $(GLEW)/lib -L $(ASSIMP)/lib -lIL -lglfw3 -lGLEW -lassimp -lm -lpthread -ldl -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

ILIB =	`pkg-config --cflags assimp` \
		`pkg-config --cflags glfw3` \
		`pkg-config --cflags IL`
LIB +=	`pkg-config --libs assimp` \
		`pkg-config --libs glfw3` \
		`pkg-config --libs IL`
SDIR =	sources/
ODIR =	objects/
IDIR =	includes/
SRC =	main.cpp \
		Matrix.cpp \
		Transformable.cpp \
		Quaternion.cpp \
		Windows.cpp \
		Mesh.cpp \
		Model.cpp \
		ResourceManager.cpp \
		Vertex.cpp \
		Shader.cpp \
		Vector2.cpp \
		Vector3.cpp \
		Vector4.cpp \
		Keyboard.cpp \
		Mouse.cpp \
		Texture.cpp \
		Light.cpp \
		DirectionalLight.cpp \
		Camera.cpp \
		LightManager.cpp \
		Color.cpp

INC =	Matrix.hpp \
		Transformable.hpp \
		IDrawable.hpp \
		Quaternion.hpp \
		Windows.hpp \
		Mesh.hpp \
		Model.hpp \
		ResourceManager.hpp \
		Vertex.hpp \
		Shader.hpp \
		Vector2.hpp \
		Vector3.hpp \
		Vector4.hpp \
		Math.hpp \
		Keyboard.hpp \
		Mouse.hpp \
		Texture.hpp \
		Light.hpp \
		DirectionalLight.hpp \
		Camera.hpp \
		IView.hpp \
		LightManager.hpp \
		Color.hpp

OBJ =	$(SRC:.cpp=.o)
I =		$(addprefix $(IDIR), $(INC))
S =		$(addprefix $(SDIR), $(SRC))
O =		$(addprefix $(ODIR), $(OBJ))

all: $(NAME)

$(NAME): $(O)
	@echo "-> Creating $(NAME)..."
	@$(CC) $(FLAGS) -o $@ $^ -I $(IDIR) $(ILIB) $(LIB)
	@echo "-> Done !"

$(ODIR)%.o: $(SDIR)%.cpp $(I)
	@echo "-> Compiling $<..."
	@$(CC) $(FLAGS) -c $< -o $@ -I $(IDIR) $(ILIB)

$(O): | ./objects
./objects:
	@mkdir $(ODIR)

clean:
	@echo "-> Cleaning object files..."
	@rm -rf $(ODIR)

fclean: clean
	@echo "-> Cleaning $(NAME)..."
	@rm -f $(NAME)

re: fclean all
