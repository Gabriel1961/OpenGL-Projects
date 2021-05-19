#pragma once
#include "../../OpenGLWrapper/Common.h"
#include "../Action/Action.h"
namespace Input
{
	int Window_Height;
	int Window_Width;
	

	namespace Mouse {
		bool LeftPressed = false;
		Action<double, double> MouseMove;
		double Y = 0, X = 0, PrevX = 0, PrevY = 0, SpeedX = 0, SpeedY = 0;
		static void ButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) LeftPressed = true;
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) LeftPressed = false;
		}
		static void PositionCallBack(GLFWwindow* window, double Xpos, double Ypos) {
			PrevY = Y;
			PrevX = X;
			Y = -(Ypos / (Window_Height / 2) - 1);
			X = Xpos / (Window_Width / 2) - 1;
			SpeedX = X - PrevX;
			SpeedY = Y - PrevY;
			MouseMove.Call(SpeedX, SpeedY);
		}
	}

	namespace Keyboard
	{
		/// <summary>
		/// Activates on keypress 
		/// int key, int action
		/// </summary>
		Action<int,int> KeyDown;

		
		void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			KeyDown.Call(key, action);
		}

	}

	void Init(GLFWwindow* window, int width, int height)
	{
		Window_Height = height;
		Window_Width = width;
		glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
		glfwSetCursorPosCallback(window, Mouse::PositionCallBack);
		glfwSetMouseButtonCallback(window, Mouse::ButtonCallBack);
		glfwSetKeyCallback(window, Keyboard::KeyPressCallback);
	}
}