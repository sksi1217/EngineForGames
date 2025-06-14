#pragma once

enum class RenderLayer
{
	Default,
	Effects,
	UI
};

struct LayerRender
{
	RenderLayer Layer;
};
