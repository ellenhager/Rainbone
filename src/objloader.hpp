#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "glm/glm.hpp"

// Load obj file without texture coordinates
bool loadObj(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	/*std::vector<glm::vec2> & out_uvs, */
	std::vector<glm::vec3> & out_normals
);
// Load obj file with texture coordinates
bool loadObj(
    const char * path, 
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
);

#endif