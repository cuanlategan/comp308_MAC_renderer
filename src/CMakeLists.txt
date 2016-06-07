#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_geometry.hpp"
	"cgra_math.hpp"
	"opengl.hpp"
	"simple_shader.hpp"
	"simple_image.hpp"
	"geometry.hpp"
	"define.h"
	"cuan_define.h"
	"grass.h"
	"wave_generator.h"
	"field.h"
	"grid.h"
	"RiverHandler.h"
	"RiverPoint.h"
	"vEdge.h"
	"vVertexPoint.h"
	"vTriangle.h"
	"imageLoader.hpp"
	"CImg.h"
	"riverData.hpp")


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"geometry.cpp"
	"grass.cpp"
	"field.cpp"
	"grid.cpp"
	"RiverHandler.cpp"
	"RiverPoint.cpp"
	"vEdge.cpp"
	"VoronoiHandler.cpp"
	"vTriangle.cpp"
	"vVertexPoint.cpp"
	"riverData.cpp")

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb)