#include "ShaderProgram.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#define SHADERS_PATH "shader"

using namespace std;

ShaderProgram::ShaderProgram(std::string computeshader)
{
    // Initially, we have zero shaders attached to the program
    m_shaderCount = 0;

    // Generate a unique Id / handle for the shader program
    // Note: We MUST have a valid rendering context before generating
    // the m_shaderProgramHandle or it causes a segfault!
    m_shaderProgramHandle = glCreateProgram();

    //Set up fragment shader
    Shader computeShader(GL_COMPUTE_SHADER);
    computeShader.loadFromFile(SHADERS_PATH + computeshader);
    computeShader.compile();

    // Set up shader program
    attachShader(computeShader);
    link();

    mapShaderProperties(GL_UNIFORM, &m_uniformMap);
    //mapShaderProperties(GL_PROGRAM_INPUT, &m_inputMap);
    //mapShaderProperties(GL_PROGRAM_OUTPUT, &m_outputMap);
    // readUniforms();
}

ShaderProgram::ShaderProgram(std::string vertexshader, std::string fragmentshader) 
{
    // Initially, we have zero shaders attached to the program
	m_shaderCount = 0;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the m_shaderProgramHandle or it causes a segfault!
	m_shaderProgramHandle = glCreateProgram();

    //Set up fragment shader
    Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile(SHADERS_PATH + vertexshader);
	vertexShader.compile();

 	//Set up fragment shader
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile(SHADERS_PATH + fragmentshader);
	fragmentShader.compile();

	//readOutputs(fragmentShader);

	// Set up shader program
	attachShader(vertexShader);
	attachShader(fragmentShader);
	link();

    mapShaderProperties(GL_UNIFORM, &m_uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &m_inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &m_outputMap);

	// readUniforms();
}


ShaderProgram::ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string geometryshader) 
{
    
    // Initially, we have zero shaders attached to the program
	m_shaderCount = 0;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the m_shaderProgramHandle or it causes a segfault!
	m_shaderProgramHandle = glCreateProgram();

    Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile(SHADERS_PATH + vertexshader);
	vertexShader.compile();

	//Set up fragment shader
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile(SHADERS_PATH + fragmentshader);
	fragmentShader.compile();

	//readOutputs(fragmentShader);

	Shader geometryShader(GL_GEOMETRY_SHADER);
	geometryShader.loadFromFile(SHADERS_PATH + geometryshader);
	geometryShader.compile();

	// Set up shader program
	attachShader(vertexShader);
	attachShader(fragmentShader);
	attachShader(geometryShader);
    link();

    mapShaderProperties(GL_UNIFORM, &m_uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &m_inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &m_outputMap);
	//readUniforms();
}

//ShaderProgram::ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string tessellationcontrollshader, std::string tessellationevaluationshader) 
ShaderProgram::ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string tessellationcontrollshader, std::string tessellationevaluationshader, std::string geometryshader) 
{
    // Initially, we have zero shaders attached to the program
	m_shaderCount = 0;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the m_shaderProgramHandle or it causes a segfault!
	m_shaderProgramHandle = glCreateProgram();

    Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile(SHADERS_PATH + vertexshader);
	vertexShader.compile();

	Shader tessControllShader(GL_TESS_CONTROL_SHADER);
	tessControllShader.loadFromFile(SHADERS_PATH + tessellationcontrollshader );
	tessControllShader.compile();
	
	Shader tessEvalShader(GL_TESS_EVALUATION_SHADER);
	tessEvalShader.loadFromFile(SHADERS_PATH + tessellationevaluationshader );
	tessEvalShader.compile();

	Shader geometryShader(GL_GEOMETRY_SHADER);
	geometryShader.loadFromFile(SHADERS_PATH + geometryshader);
	geometryShader.compile();

	//Set up fragment shader
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile(SHADERS_PATH + fragmentshader);
	fragmentShader.compile();

	//readOutputs(fragmentShader);

	// Set up shader program
	attachShader(vertexShader);
	attachShader(tessControllShader);
	attachShader(tessEvalShader);
	attachShader(geometryShader);
	attachShader(fragmentShader);

    link();

    mapShaderProperties(GL_UNIFORM, &m_uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &m_inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &m_outputMap);
	//readUniforms();
}

ShaderProgram::ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string tessellationcontrollshader, std::string tessellationevaluationshader) 
{
    // Initially, we have zero shaders attached to the program
	m_shaderCount = 0;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the m_shaderProgramHandle or it causes a segfault!
	m_shaderProgramHandle = glCreateProgram();

    Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile(SHADERS_PATH + vertexshader);
	vertexShader.compile();

	Shader tessControllShader(GL_TESS_CONTROL_SHADER);
	tessControllShader.loadFromFile(SHADERS_PATH + tessellationcontrollshader );
	tessControllShader.compile();
	
	Shader tessEvalShader(GL_TESS_EVALUATION_SHADER);
	tessEvalShader.loadFromFile(SHADERS_PATH + tessellationevaluationshader );
	tessEvalShader.compile();

	//Set up fragment shader
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile(SHADERS_PATH + fragmentshader);
	fragmentShader.compile();

	//readOutputs(fragmentShader);

	// Set up shader program
	attachShader(vertexShader);
	attachShader(tessControllShader);
	attachShader(tessEvalShader);
	attachShader(fragmentShader);
    link();

    mapShaderProperties(GL_UNIFORM, &m_uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &m_inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &m_outputMap);
	//readUniforms();
}



ShaderProgram::~ShaderProgram()
{
	// Delete the shader program from the graphics card memory to
	// free all the resources it's been using
	glDeleteProgram(m_shaderProgramHandle);
}

GLint ShaderProgram::getShaderProgramHandle()
{
	return m_shaderProgramHandle;
}

void ShaderProgram::attachShader(Shader shader)
{	
	// Increment the number of shaders we have associated with the program
	m_shaderCount++;
	// Attach the shader to the program
	// Note: We identify the shader by its unique Id value
	glAttachShader( m_shaderProgramHandle, shader.getId());

}

void ShaderProgram::link()
{
	// If we have at least two shaders (like a vertex shader and a fragment shader)...
	if (m_shaderCount >= 1)
	{
		// Perform the linking process
		glLinkProgram(m_shaderProgramHandle);
		// Check the status
		GLint linkStatus;
		glGetProgramiv(m_shaderProgramHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
            std::cerr << "ERROR: Shader program linking failed." << std::endl;
			printShaderProgramInfoLog();
			glfwTerminate();
		}
		else
		{
            #ifdef DEBUG
            std::cout << "Shader program linking OK." << std::endl;
            #endif
        }
	}
	else
	{
        std::cerr << "Can't link shaders - you need at least 1, but attached shader count is only: " <<  m_shaderCount << std::endl;
		glfwTerminate();
	}
}

void ShaderProgram::printShaderProgramInfoLog() {
    GLint logLength;
    glGetProgramiv(m_shaderProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
    if(logLength > 0){
        char* log = (char*) malloc(logLength);
        GLsizei written;
        glGetProgramInfoLog(m_shaderProgramHandle, logLength, &written, log);
        std::cout << std::string( log ) << std::endl;
        free(log);
    }
}

void ShaderProgram::bindTextureOnUse(const std::string &textureName, GLuint textureHandle)
{	
	m_textureMap[textureName] = textureHandle;
}

ShaderProgram* ShaderProgram::updateAndBindTexture(std::string name, int texUnit, GLuint textureHandle, GLenum textureType)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(textureType, textureHandle);
	update(name, texUnit);
	return this;
}

GLuint ShaderProgram::uniform(const std::string &uniform)
{
	// Note: You could do this method with the single line:
	//
	// 		return m_uniformMap[uniform];
	//
	// But we're not doing that. Explanation in the attribute() method above
	// Create an iterator to look through our uniform map and try to find the named uniform
	std::map<std::string, Info>::iterator it = m_uniformMap.find(uniform);
	// Found it? Great - pass it back! Didn't find it? Alert user and halt.
	if ( it != m_uniformMap.end() )
	{
		return m_uniformMap[uniform].location;
	}
	else
	{
        std::cerr << "ERROR: Could not find uniform in shader program: " << uniform << std::endl;
		return -1;
	}
}

GLuint ShaderProgram::buffer(const std::string &buffer)
{
	// Note: You could do this method with the single line:
	//
	// 		return m_uniformMap[uniform];
	//
	// But we're not doing that. Explanation in the attribute() method above
	// Create an iterator to look through our uniform map and try to find the named uniform
	std::map<std::string, Info>::iterator it = m_outputMap.find(buffer);
	// Found it? Great - pass it back! Didn't find it? Alert user and halt.
	if ( it != m_outputMap.end() )
	{
		return m_outputMap[buffer].location;
	}
	else
	{
        std::cerr << "ERROR: Could not find buffer in shader program: " << buffer << std::endl;
		return 0;
	}
}

GLuint ShaderProgram::texture(const std::string &texture)
{
	// Note: You could do this method with the single line:
	//
	// 		return m_uniformMap[uniform];
	//
	// But we're not doing that. Explanation in the attribute() method above
	// Create an iterator to look through our uniform map and try to find the named uniform
	std::map<std::string, GLuint>::iterator it = m_textureMap.find(texture);
	// Found it? Great - pass it back! Didn't find it? Alert user and halt.
	if ( it != m_textureMap.end() )
	{
		return m_textureMap[texture];
	}
	else
	{
        std::cerr << "ERROR: Could not find texture in shader program: " << texture << std::endl;
		return 0;
	}
}

ShaderProgram* ShaderProgram::update(std::string name, bool value) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform1i(u, value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, int value) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform1i(u, value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, float value) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform1f(u, value);

	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, double value) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform1f(u, value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::ivec2& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform2iv(u, 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::ivec3& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform3iv(u, 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name,const glm::ivec4& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform4iv(u, 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::vec2& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform2fv(u, 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::vec3& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform3fv(uniform(name), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::vec4& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform4fv(u, 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::mat2& matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniformMatrix2fv(u, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::mat3& matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const glm::mat4& matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const std::vector<int>& vector)
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
		glUniform1iv(u, sizeof(vector), &vector[0]);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const std::vector<glm::vec2>& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform2fv(u, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const std::vector<glm::vec3>& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform3fv(u, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, const std::vector<glm::vec4>& vector) 
{
	glUseProgram(m_shaderProgramHandle);
	auto u = uniform(name);
	if ( u != (GLuint) -1)
	glUniform4fv(u, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}

void ShaderProgram::use()
{	
	int i = 0;

	for(auto texture : m_textureMap)
	{	
		//std::cout << "name: " << texture.first << ", active texture:" << i << ", uniform: " << uniform(texture.first) << ", handle: " << texture.second << std::endl;

		update( texture.first, i );
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, texture.second);
		i++;
	}

	glUseProgram(m_shaderProgramHandle);
}

void ShaderProgram::disable()
{
	glUseProgram(0);
}

void ShaderProgram::mapShaderProperties(GLenum interface, std::map<std::string, Info>* map) {
	GLint numAttrib = 0;
	glGetProgramInterfaceiv(m_shaderProgramHandle, interface, GL_ACTIVE_RESOURCES, &numAttrib);

	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	properties.push_back(GL_LOCATION);
	//properties.push_back(GL_BLOCK_INDEX);
	std::vector<GLint> values(properties.size());

	for(int attrib = 0; attrib < numAttrib; ++attrib)
	{
		glGetProgramResourceiv(m_shaderProgramHandle, interface, attrib, properties.size(),
		&properties[0], values.size(), NULL, &values[0]);

		//if (values[4] != -1)
		//	continue; // skip

		Info info;
		info.type = values[1];
		info.location = values[3];

		std::vector<GLchar> nameData(256);
		nameData.resize(values[0]); //The length of the name.
		glGetProgramResourceName(m_shaderProgramHandle, interface, attrib, nameData.size(), NULL, &nameData[0]);
		std::string name = std::string((char*)&nameData[0], nameData.size() - 1);
		name = std::string(name.c_str());

		(*map)[name] = info;
	}
}

void ShaderProgram::printUniformInfo() {
    std::cerr << "Shader Program " + to_string( m_shaderProgramHandle) << " Uniform info:" << std::endl;
    printInfo(&m_uniformMap);
}
void ShaderProgram::printInputInfo() {
    std::cout << "Shader Program " << to_string( m_shaderProgramHandle) << " Input info:" << std::endl;
    printInfo(&m_inputMap);
}

void ShaderProgram::printOutputInfo() {
    std::cout << "Shader Program " << to_string( m_shaderProgramHandle) << " Output info:" << std::endl;
    printInfo(&m_outputMap);
}

void ShaderProgram::printInfo(std::map<std::string, Info>* map) {
	for(auto e : *map) {
        std::cout << "name: " << e.first << std::endl;
        std::cout << "type: " << getTypeString(e.second.type) << std::endl;
        std::cout << "location: " << to_string(e.second.location) << std::endl;
	}
}

std::string ShaderProgram::getTypeString(GLenum type) {
	switch (type) {
	case 35670: 
		return "bool";
	case 5124: 
		return "int";
	case 5125: 
		return "unsigned int";
	case 5126: 
		return "float";
	case 35667: 
		return "ivec2";
	case 35668: 
		return "ivec3";
	case 35669: 
		return "ivec4";
	case 35664: 
		return "vec2";
	case 35665: 
		return "vec3";
	case 35666: 
		return "vec4";
	case 35674: 
		return "mat2";
	case 35675: 
		return "mat3";
	case 35676: 
		return "mat4";
	case 35677: 
		return "sampler1D";
	case 35678: 
		return "sampler2D";
	case 35679: 
		return "sampler3D";
	}
	return "unknown";
	//std::to_string(type);
}

std::map<std::string, ShaderProgram::UniformBlockInfo> ShaderProgram::getAllUniformBlockInfo(ShaderProgram& shaderProgram)
{
	std::map<std::string, UniformBlockInfo> result;
	GLint numBlocks = 0;
	glGetProgramInterfaceiv(shaderProgram.getShaderProgramHandle(), GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
	
	std::vector<GLenum> blockProperties;
	blockProperties.push_back(GL_NUM_ACTIVE_VARIABLES);
	blockProperties.push_back(GL_NAME_LENGTH);
	blockProperties.push_back(GL_BUFFER_DATA_SIZE);

	std::vector<GLenum> uniformProperties;
	uniformProperties.push_back(GL_NAME_LENGTH);
	uniformProperties.push_back(GL_TYPE);
	uniformProperties.push_back(GL_OFFSET);
	//uniformProperties.push_back(GL_SIZE); // doesnt exist, for some reason
	uniformProperties.push_back(GL_ARRAY_STRIDE);
	uniformProperties.push_back(GL_MATRIX_STRIDE);

	for ( int blockIdx = 0; blockIdx < numBlocks; blockIdx++)
	{
		std::vector<GLint> blockValues(blockProperties.size());
		glGetProgramResourceiv(shaderProgram.getShaderProgramHandle(), GL_UNIFORM_BLOCK, blockIdx, blockProperties.size(), &blockProperties[0], blockValues.size(), NULL, &blockValues[0]);
		GLint numActiveUniforms = blockValues[0]; // query amount of uniforms in this block

		std::vector<GLchar> nameData(256);
		nameData.resize(blockValues[1]); //The length of the name.
		glGetProgramResourceName(shaderProgram.getShaderProgramHandle(), GL_UNIFORM_BLOCK, blockIdx, nameData.size(), NULL, &nameData[0]);
		std::string blockName = std::string((char*)&nameData[0], nameData.size() - 1);
		blockName = std::string(blockName.c_str());
		
		std::vector<GLint> uniformHandles(numActiveUniforms); // allocate room for uniform indices
		const GLenum blockVariableProperties[1] = {GL_ACTIVE_VARIABLES};
		glGetProgramResourceiv(shaderProgram.getShaderProgramHandle(), GL_UNIFORM_BLOCK, blockIdx, 1, blockVariableProperties, numActiveUniforms, NULL, &uniformHandles[0] );

		result[blockName].index = blockIdx;
		result[blockName].numActiveUniforms = numActiveUniforms;
		result[blockName].byteSize = blockValues[2];

		for (auto attribIdx : uniformHandles)
		{
			std::vector<GLint> uniformValues(uniformProperties.size());

			// retrieve uniform properties
			glGetProgramResourceiv(shaderProgram.getShaderProgramHandle(), GL_UNIFORM, attribIdx, uniformProperties.size(),
			&uniformProperties[0], uniformValues.size(), NULL, &uniformValues[0]);

			std::vector<GLchar> nameData(256);
			nameData.resize(uniformValues[0]); //The length of the name.
			glGetProgramResourceName(shaderProgram.getShaderProgramHandle(), GL_UNIFORM, attribIdx, nameData.size(), NULL, &nameData[0]);
			std::string uniformName = std::string((char*)&nameData[0], nameData.size() - 1);
			uniformName = std::string(uniformName.c_str());

			//DEBUGLOG->log("uniform: " + blockName + "." + uniformName); DEBUGLOG->indent();
			//DEBUGLOG->log("type   : " + shaderProgram.getTypeString(uniformValues[1]));
			//DEBUGLOG->log("offset : ", uniformValues[2]);
			//DEBUGLOG->log("arr-str: ",uniformValues[3]);
			//DEBUGLOG->log("mat-str: ",uniformValues[4]);

			GLuint idx[1] = {(GLuint) attribIdx};
			GLint uniformSize;
			glGetActiveUniformsiv(shaderProgram.getShaderProgramHandle(), 1, idx, GL_UNIFORM_SIZE, &uniformSize );
			//DEBUGLOG->log("size   :", uniformSize);
			
			result[blockName].uniforms[uniformName].info.location= attribIdx;
			result[blockName].uniforms[uniformName].info.type = uniformValues[1];
			result[blockName].uniforms[uniformName].offset = uniformValues[2];
			result[blockName].uniforms[uniformName].arrayStride = uniformValues[3];
			result[blockName].uniforms[uniformName].matrixStride = uniformValues[4];

			//DEBUGLOG->outdent();
		}
	}
	return result;
}

void ShaderProgram::printUniformBlockInfo(std::map<std::string, ShaderProgram::UniformBlockInfo>& map)
{
	for ( auto b : map)
	{
        std::cout << "block name: " << b.first << std::endl;
        std::cout << "index    : " << b.second.index << std::endl;
        std::cout << "byte size: " << b.second.byteSize << std::endl;
        std::cout << "#uniforms: " << b.second.numActiveUniforms << std::endl;

        for (auto u : b.second.uniforms)
        {
            std::cout << "uniform name: " << u.first << std::endl;
            std::cout << "offset  : " << u.second.offset << std::endl;
            std::cout << "location: " << u.second.info.location << std::endl;
            std::cout << "type    : "  << ShaderProgram::getTypeString(u.second.info.type) << std::endl;
            std::cout << "matrixSt: " << u.second.matrixStride << std::endl;
            std::cout << "arraySt : " << u.second.arrayStride << std::endl;
        }

	}
}

std::vector<float> ShaderProgram::createUniformBlockDataVector(ShaderProgram::UniformBlockInfo& uniformBlock)
{	
	return std::vector<float>(uniformBlock.byteSize / sizeof(float), 0.0f);
}

GLuint ShaderProgram::createUniformBlockBuffer(std::vector<float>& data, GLuint bindingPoint)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferData(GL_UNIFORM_BUFFER, data.size() * sizeof(float), &data[0], GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);

	return buffer;
}

void ShaderProgram::updateValuesInBufferData(std::string uniformName, const float* values, int numValues, ShaderProgram::UniformBlockInfo& info, std::vector<float>& buffer)
{
	auto u = info.uniforms.find(uniformName);
	if ( u == info.uniforms.end()) { return; }
	if (numValues >= buffer.size()) {return;}

	int valStartIdx = u->second.offset / 4;
	if (buffer.size() < valStartIdx + numValues){return;} 

	for (int i = 0; i < numValues; i++)
	{
		buffer[valStartIdx + i] = values[i];
	}
}

void ShaderProgram::updateValueInBuffer(std::string uniformName, const float* values, int numValues, ShaderProgram::UniformBlockInfo& info, GLuint bufferHandle)
{
	auto u = info.uniforms.find(uniformName);
	if ( u == info.uniforms.end()) { return; }
	if ( info.byteSize < u->second.offset + (numValues * sizeof(float))){return;}

	glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, u->second.offset, numValues * sizeof(float), values);
}
