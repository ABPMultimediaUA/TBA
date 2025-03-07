#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 FragPos;  
out vec3 Normal;


uniform mat4 model;
uniform mat4 modelview;


void main()
{

    gl_Position =  modelview * vec4(position, 1.0f);
    TexCoords = texCoords;
	FragPos = vec3(model * vec4(position, 1.0f));
	//NOTA: necesitamos pasar el vector normal de coordenadas del mundo a coordenadas de la vista, pero para ello no podemos multiplicarla
	//por la view directamente como hacemos con la matriz modelo ya que este vector normal no se puede trasladar o dejaria de ser perpendicular a la cara del modelo
	//entonces lo que hacemos es usar las traspuesta de la inversa del modelo y ademas la pasamos a una matriz de 3x3 para que se pierdan las coordenadas de traslacion.
	//Nota2: esto es bastante costoso, para ganar rendimiento podriamos multiplicar el vector normal por la Normal Matrix (cuya 4 coordenada es 0 para perder la traslacion)
	//pero entonces tendriamos que asegurarnos de que no hacemos ningun escalado no uniforme o sino las normales no servirian.
    Normal = mat3(transpose(inverse(model))) * normal;  
 
}

