varying vec3 color;
varying vec3 fN;
varying vec3 fL1;
varying vec3 fL2;
varying vec3 fE;
uniform vec3 diffuseproduct1;
uniform vec3 diffuseproduct2;
uniform vec3 ambientproduct1;
uniform vec3 ambientproduct2;
uniform vec3 specularproduct1;
uniform vec3 specularproduct2;
uniform float shininess;
uniform int shading_model;

void
main()
{
    if(shading_model == 0)
    {
    gl_FragColor = vec4(color, 1);
    }
    else
    {
       vec3 N = normalize(fN);
       vec3 E = normalize(fE);
       vec3 L1 = normalize(fL1);
       vec3 L2 = normalize(fL2);
       vec3 H1 = normalize( L1 + E );
       vec3 H2 = normalize( L2 + E );
       vec3 ambient1 = ambientproduct1;
       vec3 ambient2 = ambientproduct2;

       float Kd1 = max(dot(L1, N), 0.0);
       vec3 diffuse1 = Kd1 * diffuseproduct1;
       float Kd2 = max(dot(L2, N), 0.0);
       vec3 diffuse2 = Kd2 * diffuseproduct2;
       float Ks1 = pow(max(dot(N, H1), 0.0), shininess);
       vec3 specular1 = Ks1 * specularproduct1;
       float Ks2 = pow(max(dot(N, H2), 0.0), shininess);
       vec3 specular2 = Ks2 * specularproduct2;
      
       // discard the specular highlight if the light's behind the vertex
       if( dot(L1, N) < 0.0 )
       specular1 = vec3(0.0, 0.0, 0.0);

       if( dot(L2, N) < 0.0 )
       specular2 = vec3(0.0, 0.0, 0.0);
       
       gl_FragColor = vec4(ambient1 + diffuse1 + specular1 + ambient2 + diffuse2 + specular2, 1.0);

    }
}
