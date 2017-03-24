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

uniform float aspectRatio;

void main() {
    vec2 offset = vec2(0.5,0.5);
    vec2 ppos = (pos.xy - offset) * vec2(1, aspectRatio) + offset;
	passPosition = vec3(ppos.xy,pos.z);
	passUV = uv;
	gl_Position = vec4(pos.xy, pos.z, 1);
}
