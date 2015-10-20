

varying vec4 v_vertexColor;

#if !VERTEX_LIGHTING
// Direction VERS la source lumineuse exprimee dans le repere WORLD
const vec3 L = vec3(1.0, 1.0, 0.0);
varying vec3 v_normal;
#endif

void main(void)
{
#if VERTEX_LIGHTING
  	gl_FragColor = v_vertexColor;
#else
	// calcul du cosinus de l'angle entre les deux vecteurs
	float NdotL = max(dot(normalize(v_normal), L), 0.0);
	// Equation de Lambert : Intensite Reflechie = Intensite Incidente * N.L
    gl_FragColor = v_vertexColor * NdotL;
#endif
}