#version 330 core

uniform mat4 NM;
uniform vec4 color;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularity;
uniform float shinyness;
uniform sampler2D tex;

in vec3 normal;
in vec3 v;
in vec3 lightDirection;
in vec2 UV;

out vec4 frag_color;

vec4 calcShading( vec3 N, vec3 L ) {
    //Ambient contribution
    vec4 intensityAmbient = ambientColor;

    //Diffuse contribution
    vec4 intensityDiffuse = diffuseColor * max(dot(N,L), 0.0);
    intensityDiffuse = clamp(intensityDiffuse, 0.0, 1.0);

    return intensityAmbient + intensityDiffuse;
}

vec4 calcSpecularShading( vec3 N, vec3 L ) {

    //Specular contribution
    vec3 v_dir = normalize(-v);
    vec3 R = normalize(reflect(-L, normalize(N)));
    const float specExp = 10.0;
    float spec = dot(R, v_dir);
    spec = (spec > 0.0) ? (1.0 * pow(spec, specularity)) : 0.0;

    vec4 intensitySpecular = specularColor * spec;

    intensitySpecular = clamp(intensitySpecular, 0.0, 1.0);

    return intensitySpecular;
}

void main() {

    v;
    lightDirection;

    frag_color = texture(tex, UV.st);

    frag_color.rgb *= calcShading(
        normalize(vec3(NM * vec4(normal, 1.0))),
        lightDirection).rgb * 1.0;
    
    frag_color.rgb += calcSpecularShading(
        normalize(vec3(NM * vec4(normal, 1.0))),
        lightDirection).rgb * shinyness;

}