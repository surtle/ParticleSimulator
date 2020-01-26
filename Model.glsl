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

uniform vec3 AmbientColor=vec3(0.2);
uniform vec3 LightDirection=normalize(vec3(1,0,2));
uniform vec3 LightColor=vec3(0, 0, 1);
uniform vec3 DiffuseColor=vec3(0, 0, 0.5);

uniform vec3 LightDirection2=normalize(vec3(-1, 2, 1));
uniform vec3 LightColor2 = vec3(1, 0, 0);
uniform vec3 DiffuseColor2 = vec3(0.5, 0, 0);

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

vec3 calcDirLight(vec3 lightDirection, vec3 normal, vec3 amb, vec3 dif, vec3 color) {
	vec3 lightDir = normalize(lightDirection); 

	float diff = max(dot(normal, lightDir), 0.0); 

	vec3 ambient = amb;
	vec3 diffuse = dif * diff * color;

	return (ambient + diffuse);
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
