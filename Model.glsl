////////////////////////////////////////
// Model.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 ModelMtx=mat4(1);
uniform mat4 ModelViewProjMtx=mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position=ModelViewProjMtx * vec4(Position,1);

	fragPosition=vec3(ModelMtx * vec4(Position,1));
	fragNormal=vec3(ModelMtx * vec4(Normal,0));
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 AmbientColor=vec3(0.19, 0.23, 0.57);
uniform vec3 LightDirection=normalize(vec3(0,-1,-4));
uniform vec3 LightColor=vec3(0.61, 0.44, 0.89);
uniform vec3 DiffuseColor=vec3(0.83, 0.51, 0.74);

uniform vec3 eye = vec3(0, 0, 10);

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

vec3 calcDirLight(vec3 lightDirection, vec3 normal, vec3 amb, vec3 dif, vec3 color) {
	
	vec3 ambient = amb * 0.2;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightDirection);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * color;

	float specStrength = 0.8;
	vec3 viewDir = normalize(eye - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specStrength * spec * color;

	vec3 result = (ambient + diffuse + specular);
	return result;
}

void main() {
	// Compute irradiance (sum of ambient & direct lighting)

	// Diffuse reflectance

	vec3 sums = calcDirLight(LightDirection, fragNormal, AmbientColor, DiffuseColor, LightColor);

	gl_FragColor = vec4(sums, 1);

	// Gamma correction
	//gl_FragColor=vec4(sqrt(reflectance),1);
}

#endif

////////////////////////////////////////////////////////////////////////////////
