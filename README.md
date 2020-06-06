# Graphics

## 학력

- Opengl
- glew, glut

<hr>

## Assignment 1

### Triangle Mesh Viewer

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

- Bunny Image
-

<hr>

## Assignment 2

### GLSL Shaders and Cartoon Shading

```C++

```

- Bunny Image
-

<hr>

## Assignment 3

### AASDFASDF

```C++

```

- Bunny Image
-

<hr>

## Assignment 4

### ASDFASDF

```C++

```

- Bunny Image
-

<hr>
- [PintOS](https://web.stanford.edu/class/cs140/projects/pintos/pintos.html#SEC_Contents "go link")
