#version 330

/*
* This shader renders something on a screenfilling quad. 
* The renderpass needs a quad as VAO.
*/

//!< in-variables
in vec3 pos;
in vec2 uv;

//!< out-variables
out vec2 passUV;

uniform float widthScale;
uniform float heightScale;

void main() {
    vec2 fixedPos = pos.xy * vec2(widthScale, heightScale);
	gl_Position = vec4(fixedPos.xy, pos.z, 1);
	passUV = uv;
}
