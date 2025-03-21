#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

#include <imgui.h>
#include <libs/implot/implot.h>

namespace Ignition::Debug {

    struct ProfileResult {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession {
        std::string Name;
    };

    class Instrumentor {
    private:
        InstrumentationSession* mCurrentSession;
        std::ofstream mOutputStream;
        int mProfileCount;

        std::mutex mMutex;
        std::vector<std::pair<float, float>> mProfileData;  // Stores (time, duration)
        float mStartTime;

    public:
        Instrumentor() : mCurrentSession(nullptr), mProfileCount(0), mStartTime(0.0f) {}

        void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
            mOutputStream.open(filepath);
            WriteHeader();
            mCurrentSession = new InstrumentationSession{ name };
            mStartTime = GetCurrentTime();  // Mark start time
        }

        void EndSession() {
            WriteFooter();
            mOutputStream.close();
            delete mCurrentSession;
            mCurrentSession = nullptr;
            mProfileCount = 0;
            mProfileData.clear();  // Clear recorded data
        }

        void WriteProfile(const ProfileResult& result) {
            std::lock_guard<std::mutex> lock(mMutex);

            if (mProfileCount++ > 0)
                mOutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            mOutputStream << "{";
            mOutputStream << "\"cat\":\"function\",";
            mOutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            mOutputStream << "\"name\":\"" << name << "\",";
            mOutputStream << "\"ph\":\"X\",";
            mOutputStream << "\"pid\":0,";
            mOutputStream << "\"tid\":" << result.ThreadID << ",";
            mOutputStream << "\"ts\":" << result.Start;
            mOutputStream << "}";

            mOutputStream.flush();

            // Store data for real-time graph
            float timestamp = (result.Start / 1000000.0f) - mStartTime; // Convert to seconds
            float duration = (result.End - result.Start) / 1000.0f; // Convert to milliseconds
            mProfileData.emplace_back(timestamp, duration);

            // Remove old data (only keep the last 5 seconds)
            while (!mProfileData.empty() && timestamp - mProfileData.front().first > 5.0f) {
                mProfileData.erase(mProfileData.begin());
            }
        }

        void WriteHeader() {
            mOutputStream << "{\"otherData\": {},\"traceEvents\":[";
            mOutputStream.flush();
        }

        void WriteFooter() {
            mOutputStream << "]}";
            mOutputStream.flush();
        }

        void RenderImPlot() {
            std::lock_guard<std::mutex> lock(mMutex);

            static float timeWindow = 5.0f;  // 5 seconds visible window
            static float minDuration = 0.3f; // Minimum duration of 1ms

            if (ImGui::Begin("Profiler")) {
                if (ImPlot::BeginPlot("Execution Time", ImVec2(-1, 200))) {
                    ImPlot::SetupAxes("Time (s)", "Duration (ms)");

                    std::vector<float> times, durations;
                    for (const auto& [t, d] : mProfileData) {
                        times.push_back(t);
                        durations.push_back(d);
                    }

                    if (!times.empty()) {
                        float latestTime = times.back();
                        float startTime = latestTime - timeWindow;

                        // Determine Y-axis limits with a minimum of 1ms
                        float maxDuration = std::max(*std::max_element(durations.begin(), durations.end()), minDuration);

                        ImPlot::SetupAxesLimits(startTime, latestTime, 0, maxDuration, ImGuiCond_Always);

                        // Fill the space under the line
                        std::vector<float> zeroLine(durations.size(), 0.0f); // X-axis baseline
                        ImPlot::PlotShaded("Execution Time", times.data(), durations.data(), zeroLine.data(), times.size());

                        // Draw the line on top
                        ImPlot::PlotLine("Execution Time", times.data(), durations.data(), times.size());
                    }

                    ImPlot::EndPlot();
                }
            }
            ImGui::End();



        }

        static Instrumentor& Get() {
            static Instrumentor instance;
            return instance;
        }

        float GetCurrentTime() {
            return std::chrono::duration<float>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
    };

    class InstrumentationTimer {
    public:
        InstrumentationTimer(const char* name)
            : m_Name(name), m_Stopped(false) {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer() {
            if (!m_Stopped)
                Stop();
        }

        void Stop() {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}



// ---- TEMP ----

#define IG_PROFILING_ENABLE 1

#if IG_PROFILING_ENABLE
    #define IG_PROFILING_BEGIN_SESSION(name, filepath)            ::Ignition::Debug::Instrumentor::Get().BeginSession(name, filepath)
    #define IG_PROFILING_END_SESSION()                      ::Ignition::Debug::Instrumentor::Get().EndSession()
    #define IG_PROFILING_SCOPE(name)                        ::Ignition::Debug::InstrumentationTimer timer##__LINE__(name);
    #define IG_PROFILING_FUNC()                             IG_PROFILING_SCOPE(__FUNCSIG__)
#else
    #define IG_PROFILING_BEGIN_SESSION(name, filepath)
    #define IG_PROFILING_END_SESSION()
    #define IG_PROFILING_SCOPE(name)
    #define IG_PROFILING_FUNC()
#endif