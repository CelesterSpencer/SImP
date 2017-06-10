#version 330

/*
* Simple fragmentshader that can modifie the alpha value of the fragcolor. 
*/

//!< in-variable
in vec2 passUV;

//!< uniforms
uniform sampler2D tex;
uniform float transparency;
uniform bool isGrayScale;

//!< out-variables
layout(location = 0) out vec4 fragColor;

void main() 
{
	vec4 texColor = texture(tex, passUV);

	//!< fragcolor gets transparency by uniform
	if(isGrayScale)
	{
	    fragColor = vec4(texColor.rrr, transparency);
	}
	else
	{
	    fragColor = vec4(texColor.rgb, texColor.a * transparency);
	}
}