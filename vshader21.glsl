
attribute vec3 vPosition, vNormal;
varying vec3 color;
varying vec3 fN;
varying vec3 fE;
varying vec3 fL1;
varying vec3 fL2;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 diffuseproduct1;
uniform vec3 diffuseproduct2;
uniform vec3 ambientproduct1;
uniform vec3 ambientproduct2;
uniform vec3 specularproduct1;
uniform vec3 specularproduct2;
uniform vec3 lightposition1;
uniform vec3 lightposition2;
uniform float shininess;
uniform vec4 eye;
uniform int shading_model;

void
main()
{
     if (shading_model == 0)
   {
     vec3 pos = (model_view * vec4(vPosition,1.0)).xyz;
     vec3 L1 = normalize( lightposition1 - pos );
     vec3 L2 = normalize( lightposition2 - pos );
     vec3 E1 = normalize(-pos);
     vec3 H1 = normalize(L1 + E1);
     vec3 H2 = normalize(L2 + E1);
     vec3 N = normalize( model_view * vec4( vNormal, 0.0) ).xyz;

     vec3 ambient1 = ambientproduct1;
     vec3 ambient2 = ambientproduct2;
     float Kd1 = max( dot(L1, N), 0.0 );
     float Kd2 = max( dot(L2, N), 0.0 );
     float Ks1 = pow( max(dot(N, H1), 0.0), shininess );
     float Ks2 = pow( max(dot(N, H2), 0.0), shininess );
     vec3 specular1 = Ks1 * specularproduct1;
     vec3 specular2 = Ks2 * specularproduct2;
     if( dot(L1, N) < 0.0 ) 
        specular1 = vec3(0.0, 0.0, 0.0);
     if( dot(L2, N) < 0.0 ) 
        specular2 = vec3(0.0, 0.0, 0.0);
     vec3 diffuse1 = Kd1*diffuseproduct1;
     vec3 diffuse2 = Kd2*diffuseproduct2;
   
     gl_Position = projection * model_view * vec4(vPosition, 1);
     color= diffuse1 + diffuse2 + ambient1 +  ambient2 + specular1 + specular2;

    }

     else
     {

           fN = vNormal;
           fE = eye.xyz - vPosition;

           // Light defined in world coordinates
 
            fL1 = lightposition1.xyz - vPosition.xyz;
            fL2 = lightposition2.xyz - vPosition.xyz;
      
       

       gl_Position = projection * model_view * vec4(vPosition, 1);
     }
}
