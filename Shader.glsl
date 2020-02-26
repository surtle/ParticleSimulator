////////////////////////////////////////
// Shader.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 ModelMtx = mat4(1);
uniform mat4 ModelViewProjMtx = mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position = ModelViewProjMtx * vec4(Position, 1);

	fragPosition = vec3(ModelMtx * vec4(Position, 1));
	fragNormal = vec3(ModelMtx * vec4(Normal, 0));
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 AmbientColor = vec3(0.4, 0.1, 0.4);
uniform vec3 LightDirection = normalize(vec3(0, -1, -4));
uniform vec3 LightColor = vec3(1, 0, 1);
uniform vec3 DiffuseColor = vec3(0, 0, 0.7);

uniform vec3 LightDirection2 = normalize(vec3(-1, 3, 1));
uniform vec3 LightColor2 = vec3(1, 0, 0);
uniform vec3 DiffuseColor2 = vec3(0.2, 0, 0);

uniform vec3 eye = vec3(0, 0, 10);

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

void main() {

	gl_FragColor = vec4(0.2);

	// Gamma correction
	//gl_FragColor=vec4(sqrt(reflectance),1);
}

#endif

////////////////////////////////////////////////////////////////////////////////
