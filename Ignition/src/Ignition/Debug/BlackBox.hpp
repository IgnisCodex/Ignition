#pragma once

#include <imgui.h>
#include <libs/implot/implot.h>

namespace Ignition::Debug {
	class BlackBox {
	public:
		inline void Show() {

			if (ImGui::Begin("BlackBox")) {
				if (ImPlot::BeginPlot("My Plot")) {
					ImPlot::PlotLine("Test", mData, 11);
					ImPlot::EndPlot();
				}
				ImGui::End();
			}

			ImPlot::ShowDemoWindow();
		}

	private:
		int mData[11] = { 1, 3, 4, 7, 1, 5, 3, 5, 2, 65, 31 };
	};
}