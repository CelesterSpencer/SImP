#version 330

/*
* This shader renders something on a screenfilling quad. 
* The renderpass needs a quad as VAO.
*/

//!< in-variables
in vec3 pos;
in vec2 uv;

//!< out-variables
out vec3 passPosition;
out vec2 passUV;

void main() {
	passPosition = pos;
	passUV = uv;
	gl_Position = vec4(pos.xy, pos.z, 1);
}
