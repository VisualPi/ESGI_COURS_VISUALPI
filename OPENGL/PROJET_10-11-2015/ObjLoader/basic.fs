 #version 330

 // Direction VERS la source lumineuse exprimee dans le repere WORLD
const vec3 L = vec3(0.0, 0.0, 1.0);

in vec3 v_normal;

uniform sampler2D u_texture;
in vec2 v_UV;

out vec4 Fragment;

void main(void)
{
    vec4 textureColor = texture(u_texture, v_UV);

	// calcul du cosinus de l'angle entre les deux vecteurs
	float NdotL = max(dot(normalize(v_normal), L), 0.0);
	// Equation de Lambert : Intensite Reflechie = Intensite Incidente * N.L
    Fragment = textureColor * NdotL;
    //Fragment = textureColor;
}