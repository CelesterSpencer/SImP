#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

// std
#include <map>
#include <vector>
#include <string>

// lib
#include <glm/glm.hpp>
#include <glad/glad.h>

// project
#include "Shader.h"

class ShaderProgram
{

public:

    /**
	 * @brief Constructor
	 *
	 * @param computeshader path to the computeshader
	 *
	 */
    ShaderProgram(std::string computeshader);

	/**
	 * @brief Constructor
	 * 
	 * @param vertexshader path to the vertexshader
	 * @param fragmentshader path to the fragmentshader
	 * 
	 */
	ShaderProgram(std::string vertexshader, std::string fragmentshader);

	/**
	 * @brief Constructor
	 * 
	 * @param vertexshader path to the vertexshader
	 * @param fragmentshader path to the fragmentshader
	 * @param geometryshader path to the geometryshader
	 * 
	 */
	ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string geometryshader);

		/**
	 * @brief Constructor
	 * 
	 * @param vertexshader path to the vertexshader
	 * @param fragmentshader path to the fragmentshader
	 * @param geometryshader path to the geometryshader
	 * 
	 */
	ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string tessellationcontrollshader, std::string tessellationevaluationshader, std::string geometryshader);
	
	ShaderProgram(std::string vertexshader, std::string fragmentshader, std::string tessellationcontrollshader, std::string tessellationevaluationshader);


	/**
	 * @brief Destructor
	 * 
	 */
	~ShaderProgram();

	GLint getShaderProgramHandle(); //!< returns the program handle

	/**
	 * @brief Updates a boolean uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, bool value);
	/**
	 * @brief Updates an integer uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, int value);
	/**
	 * @brief Updates a float uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, float value);
	/**
	 * @brief Updates a double uniform variable
	 * 
	 * @param name  Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, double value);
	/**
	 * @brief Updates a 2D integer vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::ivec2& vector);
	/**
	 * @brief Updates a 3D integer vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::ivec3& vector);
	/**
	 * @brief Updates a 4D integer vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::ivec4& vector);
	/**
	 * @brief Updates a 2D float vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::vec2& vector);
	/**
	 * @brief Updates a 3D float vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::vec3& vector);
	/**
	 * @brief Updates a 4D float vector uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::vec4& vector);
	/**
	 * @brief Updates a 2x2 matrix uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::mat2& matrix);
	/**
	 * @brief Updates a 3x3 matrix uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::mat3& matrix);
	/**
	 * @brief Updates a 4x4 matrix uniform variable
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const glm::mat4& matrix);
	/**
	 * @brief Updates a list of 2D vector uniform variables
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector which stores the values to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const std::vector<int>& vector);
	/**
	 * @brief Updates a list of 2D vector uniform variables
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector which stores the values to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const std::vector<glm::vec2>& vector);
	/**
	 * @brief Updates a list of 3D vector uniform variables
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector which stores the values to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const std::vector<glm::vec3>& vector);
	/**
	 * @brief Updates a list of 4D vector uniform variables
	 * 
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector which stores the values to update the unform with
	 * 
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, const std::vector<glm::vec4>& vector);

	/**
	 * @brief binds a texture to a OpenGL texture unit and updates the corresponding uniform
	 * @param name 	Name of the (sampler) uniform variable in GLSL
	 * @param texUnit texture unit to bind the texture to (make sure it doesn't colide with the TextureMap size)
	 * @param textureHandle the texture to bind
	 * @return The shader program
	 */
	ShaderProgram* updateAndBindTexture(std::string name, int texUnit, GLuint textureHandle, GLenum textureType = GL_TEXTURE_2D);

	/**
	 * @brief Method to add an already existing texture to the shader, which will be bound automatically wenn use() is called
	 * 
	 * @param textureName name of the texture to add
	 * @param textureHandle texture handle
	 *
	 */
	void bindTextureOnUse(const std::string &textureName, GLuint textureHandle);

	/**
	 * @brief Method to enable the shader program
	 * 
	 */
	virtual void use();

	/**
	 * @brief Method to disable the shader program
	 * 
	 */
	void disable();
	
	/**
	* @brief Struct for possible GLSL bindings
	*/
	struct Info{
		GLenum type;
		GLuint location;
	};

	inline std::map<std::string, Info>* getUniformInfoMap(){return &m_uniformMap;} //!< returns the Texturemap
	inline std::map<std::string, Info>* getOutputInfoMap(){return &m_outputMap;} //!< returns the Texturemap
	inline std::map<std::string, Info>* getInputInfoMap(){return &m_inputMap;} //!< returns the Texturemap
	inline std::map<std::string, GLuint>* getTextureMap()	{return &m_textureMap;} //!< returns the Texturemap
	
	/**
	 * @brief Logs all active bound uniforms to the console
	 */
	void printUniformInfo();
	/**
	 * @brief Logs all active bound input buffers to the console
	 */
	void printInputInfo();
	/**
	 * @brief Logs all active bound output buffers to the console
	 */
	void printOutputInfo();


	void printInfo(std::map<std::string, Info>* map);

	/**
	 * @brief Prints informations of the shader programs compile and link status
	 *        to the console
	 */
	void printShaderProgramInfoLog();

	/**
	 * @brief Method to returns the bound location of a named uniform
	 * 
	 * @param uniform name of the uniform
	 * 
	 */
	 GLuint uniform(const std::string &uniform);

	/**
	 * @brief Method to returns the bound location of a named buffer
	 * 
	 * @param buffer name of the buffer
	 * 
	 */
	GLuint buffer(const std::string &buffer);

	/**
	 * @brief Method to returns the texture handle that is bound to the provided (sampler-)name when use() is called 
	 * 
	 * @param texture name of the texture
	 * 
	 */
	GLuint texture(const std::string &texture);

protected:

	/**
	 * @brief maps the shader properties of an interface (i.e. all Uniforms) 
	 * to their locations and names so they can be accessed from outside.
	 * @details Possible through glGetProgramInterfaceiv which is quite awesome.
	 * 
	 * @param interface GL_UNIFORM, GL_PROGRAM_INPUT or GL_PROGRAM_OUTPUT
	 * @param map target to write retrieved information to
	 */
	void mapShaderProperties(GLenum glinterface, std::map<std::string, Info>* map);

	/**
	 * @brief Method to attach a shader to the shader program
	 * 
	 * @param shader shader to attach
	 * 
	 */
	void attachShader(Shader shader);

	/**
	 * @brief Method to link the shader program and display the link status
	 * 
	 * @param shader shader to attach
	 * 
	 */
	void link();

	// Handle of the shader program
	GLuint m_shaderProgramHandle;

	// Number of attached shader
	int m_shaderCount;

	/**
	 * @brief Maintains all used and bound uniform locations
	 */
	std::map<std::string, Info> m_uniformMap;
	/**
	 * @brief Maintains all used input buffers
	 */
	std::map<std::string, Info> m_inputMap;
	/**
	 * @brief Maintains all used output buffers
	 */
	std::map<std::string, Info> m_outputMap;

	// Map of texture handles that will be bound to the associated (sampler-) name when use() is called
	std::map<std::string, GLuint> m_textureMap;

public:
	static std::string getTypeString(GLenum type);

	struct BlockUniformInfo 
	{
		ShaderProgram::Info info;
		GLint offset; // byte offset in the uniform block / buffer
		GLint arrayStride;
		GLint matrixStride;
	};
	struct UniformBlockInfo
	{
		GLint index; //!< index in the shader program
		GLint byteSize;
		GLint numActiveUniforms; //!< amount of active uniforms in this uniform block
		std::map<std::string, BlockUniformInfo> uniforms; //!< uniform locations in the shader
	};
	static std::map<std::string, UniformBlockInfo> getAllUniformBlockInfo(ShaderProgram& shaderProgram);
	static void printUniformBlockInfo(std::map<std::string, ShaderProgram::UniformBlockInfo>& map);

	static std::vector<float> createUniformBlockDataVector(ShaderProgram::UniformBlockInfo& uniformBlock); //!< creates a std::vector equal to the byte size of the uniform block
	static GLuint createUniformBlockBuffer(std::vector<float>& data, GLuint bindingPoint); //!< generates a buffer, fills it with data and binds it to bindingPoint

	static void updateValuesInBufferData(std::string uniformName, const float* values, int numValues, ShaderProgram::UniformBlockInfo& info, std::vector<float>& buffer);
	static void updateValueInBuffer(std::string uniformName, const float* values, int numValues, ShaderProgram::UniformBlockInfo& info, GLuint bufferHandle);
};

#endif // SHADER_PROGRAM_H