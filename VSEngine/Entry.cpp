#include "../Renderer/Renderer.h"

int main()
{
	VSEngine::Renderer renderer(600,800, "My Window");
	renderer.Start();
	
	return 0;
}