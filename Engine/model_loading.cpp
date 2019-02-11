//#define GLEW_STATIC
//#include <GLEW/glew.h>
//
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <Engine/Rendering/ContextWindow.h>
//#include <Engine/Rendering/Shader.h>
//#include <Engine/Math/Vector.h>
//#include <Engine/Math/Matrix.h>
//#include <Engine/Math/Projection.h>
//#include <Engine/Math/Transform.h>
//
//#include <Engine/TEMP/model.h>
//
//#include <iostream>
//
//int main()
//{
//	ContextWindow::Instance().Init("s", 1280, 720);
//
//	Shader ourShader = Shader();
//	ourShader.Load("TEMP/1.model_loading.vs", "TEMP/1.model_loading.fs");
//
//	ModelLE ourModel("TEMP/resources/objects/planet/planet.obj");
//
//	while (!ContextWindow::Instance().ShouldClose())
//	{
//		Time::Calculate();
//
//		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		ourShader.Use();
//
//		Matrix4 persp = Projection::Perspective(80, 16.f / 9.f, 0.1f, 100);
//		Matrix4 v = Transform::LookAt(Vector3(0,0,3), 
//			Vector3(0,0,2), 
//			Vector3(0,1,0));
//
//		ourShader.SetMat4("projection", persp);
//		ourShader.SetMat4("view", v);
//		
//		Transform t = Transform();
//		t.Translate(Vector3(0, -1.75f, 0));
//		t.Scale(Vector3(0.2f, 0.2f, 0.2f));
//		
//		ourShader.SetMat4("model", t.GetTransformMatrix());
//		ourModel.Draw(ourShader.GetProgramID());
//
//		ContextWindow::Instance().SwapBuffers();
//		ContextWindow::Instance().PollEvents();
//	}
//
//	ContextWindow::Instance().Terminate();
//	return 0;
//}
