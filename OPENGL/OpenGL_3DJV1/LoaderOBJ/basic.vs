
attribute vec4 a_position;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

// Direction VERS la source lumineuse exprimee dans le repere WORLD
const vec3 L = vec3(1.0, 1.0, 0.0);

const vec4 c_objectColor = vec4(0.0, 1.0, 1.0, 1.0);

varying vec4 v_vertexColor;
varying vec3 v_normal;

#define VERTEX_LIGHTING 0

void main(void)
{
	// pseudo normale exprimee dans le repere LOCAL
	vec3 pseudoNormal = normalize(a_position.xyz);
	// solution 1
	//vec3 N = (u_worldMatrix * vec4(pseudoNormal, 0.0)).xyz;
	// solution 2
	vec3 N = mat3(u_worldMatrix) * pseudoNormal;

#if VERTEX_LIGHTING
	// calcul du cosinus de l'angle entre les deux vecteurs
	float NdotL = max(dot(N, L), 0.0);
	// Equation de Lambert : Intensite Reflechie = Intensite Incidente * N.L
	v_vertexColor = c_objectColor * NdotL;
#else
	v_vertexColor = c_objectColor;
	v_normal = N;
#endif

	gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * a_position;

}