#pragma once

#include "IGPCH.hpp"

#include "Ignition/Core/Layer.hpp"

#include "Ignition/API.hpp"

namespace Ignition::Core {
	class IGNITION_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
		std::vector<Layer*>::iterator end() { return mLayers.end(); }

	private:
		std::vector<Layer*> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};
}