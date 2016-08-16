

#include "Angel.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

//--------------------------------------------------------------------------
vec3 vertices[15000];
vec3 normals_vertices[15000];
vec3 normals_faces[15000];
vec3 points[15000];
int count = 0;
vec3 product(vec3, vec3);
int count1 = 0;
void mymenu(int);
void face(int, int, int);
void compute_normals();
void idle();
vec3 light_diffuse1 = vec3(1.0, 0.8, 1.0);
vec3 light_diffuse2 = vec3(1.0, 1.0, 1.0);
vec3 material_diffuse1 = vec3(1.0, 0.8, 1.0);
vec3 material_ambient1 = vec3(1.0, 0.0, 0.5);
vec3 material_specular1 = vec3(1.0, 0.0, 1.0);
vec3 material_diffuse2 = vec3(0.0, 0.5, 1.0);
vec3 material_ambient2 = vec3(0.3, 0.7, 0.5);
vec3 material_specular2 = vec3(0.3, 0.5, 0.8);
vec3 material_diffuse3 = vec3(0.5, 0.8, 0.2);
vec3 material_ambient3 = vec3(1.0, 0.5, 0.3);
vec3 material_specular3 = vec3(0.0, 0.8, 1.0);
vec3 light_ambient1 = vec3(1.0, 0.0, 0.0);
vec3 light_ambient2 = vec3(1.0, 0.0, 1.0);
vec3 light_specular1 = vec3(1.0, 0.0, 0.0);
vec3 light_specular2 = vec3(1.0, 0.0, 0.0);
vec3 light_position1;
vec3 light_position2 = vec3(-10.0, -5.0, 5.0);
float shininess = 100.0;
GLfloat left = -1.0;
GLfloat right = 1.0;
GLfloat bottom = -1.0;
GLfloat top = 1.0;
GLfloat near_ = -1.0;
GLfloat far_ = 1.0;
GLfloat radius_camera = 0.2;
GLfloat theta_camera = 0.0;
GLfloat radius_light = 0.2;
GLfloat theta_light = 0.0;
GLfloat phi = 0.0;
GLfloat height_light = 0.0;
GLfloat znear = 0.5;
GLfloat zfar = 5.0;
const GLfloat dr = 5.0 * DegreesToRadians;
vec4 eye;
vec4 at = vec4( 0.0, 0.0, 0.0, 1.0 );
vec4 up = vec4( 0.0, 1.0, 0.0, 0.0 );
GLuint model_view_loc;
GLuint projection_loc;
GLuint diffuseproduct1_loc;
GLuint diffuseproduct2_loc;
GLuint specularproduct1_loc;
GLuint specularproduct2_loc;
GLuint ambientproduct1_loc;
GLuint ambientproduct2_loc;
GLuint lightposition1_loc;
GLuint lightposition2_loc;
GLuint eye_loc;
GLuint shininess_loc;
GLuint shading_model_loc;
vec3 diffuseproduct1;
vec3 diffuseproduct2;
vec3 ambientproduct1;
vec3 ambientproduct2;
vec3 specularproduct1;
vec3 specularproduct2;
mat4 model_view;
mat4 projection;
float pi = 3.14;
int decision=1;
bool rotation = false;
int shading_model = 0;
int material_selection= 1;


void
init(void)
{
      
      
	std::string line;
	std::istringstream is;
	std::ifstream inFile;
	inFile.open("bound-bunny_1k.smf");

	if (inFile.fail())
	{
		std::cout << "failed to open file!!\n";
	}

	std::string type;
	float vx, vy, vz;
	int v1, v2, v3;
	getline(inFile, line);

	while (line.empty() != true)
	{
		is.str(line);
		is >> type;
		if (type.compare("v")==0)
		{
			is >> vx >> vy >> vz;
			points[count1] = vec3(vx, vy, vz);
			count1++;
	        }
                   
		else if (type.compare("f") == 0)
		{
                   
	
			is >> v1 >> v2 >> v3;
			face(v1, v2, v3);
                   
		
			/*Flat shading
			vec3 V0 = points[v1 - 1];
			vec3 V1 = points[v2 - 1];
			vec3 V2 = points[v3 - 1];
			normals_face[count_normal_face] = normalize(cross(V1 - V0, V2 - V1)); 
			count_normal_face++;
			normals_face[count_normal_face] = normals_face[count_normal_face - 1]; 
			count_normal_face++;
			normals_face[count_normal_face] = normals_face[count_normal_face - 1]; 
			count_normal_face++;
			*/
		}
		line.clear();
		is.clear();
		getline(inFile, line);
	}

        for (int i = 0; i <15000; i++)
	     {
	     normals_vertices[i] = vec3 (0.0, 0.0, 0.0);
	     }
       
          compute_normals();
	
        for (int i = 0; i <count; i++)
	    {
             normals_vertices[i] = normalize(normals_vertices[i]);

	    }

	
	
	// Create a vertex array object
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);


	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals_vertices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(vertices[0]), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, count*sizeof(vertices[0]), count * sizeof(normals_vertices[0]), normals_vertices);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(0));

	GLuint loc2 = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(count*sizeof(vertices[0])));
	
	model_view_loc = glGetUniformLocation(program, "model_view");
	projection_loc = glGetUniformLocation(program, "projection");
	diffuseproduct1_loc= glGetUniformLocation(program, "diffuseproduct1");
	diffuseproduct2_loc = glGetUniformLocation(program, "diffuseproduct2");
	ambientproduct1_loc = glGetUniformLocation(program, "ambientproduct1");
	ambientproduct2_loc = glGetUniformLocation(program, "ambientproduct2");
	specularproduct1_loc = glGetUniformLocation(program, "specularproduct1");
	specularproduct2_loc = glGetUniformLocation(program, "specularproduct2");
	lightposition1_loc = glGetUniformLocation(program, "lightposition1");
	lightposition2_loc = glGetUniformLocation(program, "lightposition2");
	shininess_loc = glGetUniformLocation(program, "shininess");
	eye_loc = glGetUniformLocation(program, "eye");
	shading_model_loc = glGetUniformLocation(program, "shading_model");
	
	

	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0); // black background
}


//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT
		| GL_DEPTH_BUFFER_BIT);

	if(material_selection == 1)
        {
        diffuseproduct1 = product(light_diffuse1, material_diffuse1);
	diffuseproduct2 = product(light_diffuse2, material_diffuse1);
	specularproduct1 = product(light_specular1, material_specular1);
	specularproduct2 = product(light_specular2, material_specular1);
	ambientproduct1 = product(light_ambient1, material_ambient1);
	ambientproduct2 = product(light_ambient2, material_ambient1);
        }

        if(material_selection == 2)
        {
        diffuseproduct1 = product(light_diffuse1, material_diffuse2);
	diffuseproduct2 = product(light_diffuse2, material_diffuse2);
	specularproduct1 = product(light_specular1, material_specular2);
	specularproduct2 = product(light_specular2, material_specular2);
	ambientproduct1 = product(light_ambient1, material_ambient2);
	ambientproduct2 = product(light_ambient2, material_ambient2);
        }
        if(material_selection == 3)
        {
        diffuseproduct1 = product(light_diffuse1, material_diffuse3);
	diffuseproduct2 = product(light_diffuse2, material_diffuse3);
	specularproduct1 = product(light_specular1, material_specular3);
	specularproduct2 = product(light_specular2, material_specular3);
	ambientproduct1 = product(light_ambient1, material_ambient3);
	ambientproduct2 = product(light_ambient2, material_ambient3);
        }
		eye[0] = radius_camera*sin(theta_camera);
		eye[1] = phi;
		eye[2] = radius_camera*cos(theta_camera);
		eye[3] = 1.0;
	
		light_position1[0] = radius_light * sin(theta_light);
		light_position1[1] = height_light;
		light_position1[2] = radius_light * cos(theta_light);
	
		
		model_view = LookAt(eye, at, up);

	if (decision == 1)
		projection = Ortho(left, right, bottom, top, near_, far_);

	else if (decision == 2)
		projection = Frustum(left, right, bottom, top, znear, zfar);

	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_view);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glUniform4fv(eye_loc, 1, eye);
	glUniform3fv(lightposition1_loc, 1, light_position1);
        glUniform3fv(diffuseproduct1_loc, 1, diffuseproduct1);
	glUniform3fv(diffuseproduct2_loc, 1, diffuseproduct2);
	glUniform3fv(ambientproduct1_loc, 1, ambientproduct1);
	glUniform3fv(ambientproduct2_loc, 1, ambientproduct2);
	glUniform3fv(specularproduct1_loc, 1, specularproduct1);
	glUniform3fv(specularproduct2_loc, 1, specularproduct2);
	glUniform3fv(diffuseproduct2_loc, 1, diffuseproduct2);
	glUniform3fv(lightposition2_loc, 1, light_position2);
	glUniform1f(shininess_loc, shininess);



	
	glDrawArrays(GL_TRIANGLES, 0, count);    // draw the points

	glutSwapBuffers();



}


//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	
	case 'r':
		radius_camera += 0.5;
		break;
	case 'c':
		radius_camera -= 0.5;
		break;
	case 'o':
		theta_camera += dr;
		break;
	case 'i':
		theta_camera -= dr;
		break;
	case 'p':
		phi += dr;
		break;
	case 'k':
		phi -= dr;
		break;
	case 't':
		theta_light += dr;
		break;
	case 'y':
		theta_light -= dr;
		break;
	case 'f':
		height_light += dr;
		break;
	case 'g':
		height_light -= dr;
		break;
	case 'v':
		radius_light += 0.2;
		break;
	case 'b':
		radius_light -= 0.2;
		break;
	}
}

//----------------------------------------------------------------------------
int mainwindow;
int menu_id;
int
main(int argc, char **argv)
{
	
        std::cout<<"Welcome to Assignment 6"<<std::endl;
        std::cout<<"Please read the following instructions to use this application."<<std::endl;
        std::cout<<"1) Right-click to access options"<<std::endl;
        std::cout<<"2) Press 'r' and 'c' to increase and decrease the radius of camera respectively"<<std::endl;
        std::cout<<"3) Press 'o' and 'i' to increase and decrease the movement of camera respectively"<<std::endl;
        std::cout<<"4) Press 'p' and 'k' to increase and decrease the height of camera respectively"<<std::endl;  
        std::cout<<"5) Press 't' and 'y' to increase and decrease the movement of light respectively"<<std::endl;  
        std::cout<<"6) Press 'f' and 'g' to increase and decrease the height of light respectively"<<std::endl;  
        std::cout<<"7) Press 'v' and 'b' to increase and decrease the radius of light respectively"<<std::endl;          
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);

	mainwindow = glutCreateWindow("Assignment 6");
	glewExperimental = GL_TRUE;
	glewInit();
	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSetWindow(mainwindow);
	menu_id = glutCreateMenu(mymenu);
	glutAddMenuEntry("PARALLEL", 1);
	glutAddMenuEntry("PERSPECTIVE", 2);
	glutAddMenuEntry("START ROTATION OF CAMERA", 3);
	glutAddMenuEntry("STOP ROTATION OF CAMERA", 4);
	glutAddMenuEntry("GOROUD SHADING", 5);
	glutAddMenuEntry("PHONG SHADING", 6);
        glutAddMenuEntry("MATERIAL 1", 7);
        glutAddMenuEntry("MATERIAL 2", 8);
        glutAddMenuEntry("MATERIAL 3", 9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}


void mymenu(int id)
{
	if (id == 1)
		decision = 1;
	if (id == 2)
		decision = 2;
	if (id == 3)
		rotation = true;
	if (id == 4)
		rotation = false;
	if (id == 5)
		shading_model = 0;
	if (id == 6)
		shading_model = 1;
        if (id == 7)
                material_selection = 1; 
        if (id == 8)
                material_selection = 2; 
        if (id == 9)
                material_selection = 3; 
	glUniform1i(shading_model_loc, shading_model);
}


void face(int a, int b, int c)
{

	vertices[count] = points[a-1];
        vec3 surface_normal = normalize(cross(points[b-1] - points[a-1], points[c-1] - points[a-1])); 
        normals_faces[count] = surface_normal;
	count++;

	vertices[count] = points[b-1];
        normals_faces[count] = surface_normal;
	count++;

	vertices[count] = points[c-1];
        normals_faces[count] = surface_normal;
	count++;
	
	         
}
void compute_normals()
{

	for (int i=0; i<count; i++)
      {
         for (int j=0; j<count; j++)
         {

          if(vertices[i].x == vertices[j].x && vertices[i].y == vertices[j].y && vertices[i].z == vertices[j].z && i != j)
              {
               normals_vertices[i] +=  normals_faces[j];
         
        	}

	
            }
       }
	
	         
}
void
idle(void)
{
	if (rotation)
	theta_camera += 0.0015;
	glutPostRedisplay();
}

vec3 product(vec3 a, vec3 b)
{
	return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
