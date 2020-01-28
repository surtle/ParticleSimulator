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

uniform vec3 AmbientColor=vec3(0.15, 0.2, 0.6);
uniform vec3 LightDirection=normalize(vec3(1,0,2));
uniform vec3 LightColor=vec3(0, 0, 1);
uniform vec3 DiffuseColor=vec3(0, 0, 0.5);

uniform vec3 LightDirection2=normalize(vec3(-1, 2, 1));
uniform vec3 LightColor2 = vec3(1, 0, 0);
uniform vec3 DiffuseColor2 = vec3(0.5, 0, 0);

uniform vec3 eye = vec3(0, 3, -5);

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

vec3 calcDirLight(vec3 lightDirection, vec3 normal, vec3 amb, vec3 dif, vec3 color) {
	vec3 lightDir = normalize(lightDirection); 
	vec3 specLight = vec3(1);

	float diff = max(dot(normal, lightDir), 0.0); 
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(eye - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = amb;
	vec3 diffuse = dif * diff * color;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * specLight;

	return (ambient + diffuse + specular);
}

void main() {
	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance=AmbientColor + LightColor * max(0,dot(LightDirection,fragNormal));

	vec3 irradiance2=AmbientColor + LightColor2 * max(0,dot(LightDirection2, fragNormal));

	// Diffuse reflectance
	vec3 reflectance=irradiance * DiffuseColor;

	vec3 reflectance2=irradiance * DiffuseColor2; 

	vec3 sums = calcDirLight(LightDirection, fragNormal, AmbientColor, DiffuseColor, LightColor) + calcDirLight(LightDirection2, fragNormal, AmbientColor, DiffuseColor2, LightColor2);

	gl_FragColor = vec4(sums, 1);

	// Gamma correction
	//gl_FragColor=vec4(sqrt(reflectance),1);
}

#endif

////////////////////////////////////////////////////////////////////////////////
