# Graphics

## What I used

-   Opengl
-   glew, glut

<hr>

## Assignment 1

### Triangle Mesh Viewer

-   Read .off file
-   render by buffer
-   keyboard interaction (View Change, polygone mode change)

[See Detailed Code](https://github.com/Sunny8747/Graphics/blob/master/Assign1/main.cpp "go link")

```C++
// read .off file
// OFF
// #v #f 0 : number of vertices, faces
// vx1 vy1 vz1 : xyz coordinate for vertex1
// vx2 vy2 vz2 : xyz coordinate for vertex2
// ...
// #v_f1 f1v1 f1v2 f1v3 : each index for face1
// #v_f2 f2v1 f2v2 f2v3 : each index for face2
// vertices, index of faces, normal calculation
void off_file_read(char* arg_path){...}

void renderScene(void){
    glclear();
    Lightfunc();
    glGenBuffers(1, &array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunny_vert), bunny_vert, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunny_normal), bunny_normal, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, 0);

    //indexbuffer don't need attribarray
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bunny_ind), bunny_ind, GL_STATIC_DRAW);

    //same with number of ind not no of triangle
	glDrawElements(GL_TRIANGLES, 208418, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
}
```

-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign1/bunny_20131329_%EC%8B%A0%EC%84%A0%EC%9A%B0.GIF" width="30%" title="Bunny" alt="Bunny"></img>
-

<hr>

## Assignment 2

### GLSL Shaders and Cartoon Shading

-   Phong shading (Phong illumination, by using normal and distance between eye and object)
-   Sillhouette shading (Over lapping two object, by using glCullFace(GL_BACK and GL_FRONT))
-   Toon shading (Leveling by value of 'Lambert's cosine law')

[See Detailed Report and Code](https://github.com/Sunny8747/Graphics/tree/master/Assign2 "go link")

```C++
//toon.frag
// diffuse_level = dot(lightDir, normal) from toon.vert
	if(toon_level == 3) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > level_scale) {
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale, 2) * diffuse;
		}
	}
```

-   Keyboard interaction (Changing shader, toon level, diffuse and shiny parameter)

-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign2/images/t-d.GIF" width="30%" title="Phong Shading" alt="Phong Shading"></img>
-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign2/images/t-c-level6.GIF" width="30%" title="Toon Shading" alt="Toon Shading"></img>

<hr>

## Assignment 3

### 2D texture UV mapping, Mouse drag object rotating

-   2D static texture UV mapping
-   Mouse drag moves object like trackball
-   Failed Environment mapping, object reflection_MAP

[See Detailed Report and Code](https://github.com/Sunny8747/Graphics/tree/master/Assign4 "go link")

-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign3/2.GIF" width="30%" title="Texture mapping" alt="Texture mapping"></img>

<hr>

## Assignment 4

### Ray casting(Maximum Intensity Projection, Alpha blending for transparency)

-   Shader 부분만 직접 작업하였습니다.
-   ray-cube intersection point
-   MIP (Maximum Intensity Projection)
-   Alpha blending for transparency
-   Sampling rate can change in .frag

-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign4/MIP1.GIF" width="30%" title="MIP" alt="MIP"></img>
-   <img src="https://github.com/Sunny8747/Graphics/blob/master/Assign4/alpha2.GIF" width="30%" title="Alpha blending" alt="Alpha blending"></img>

<hr>
