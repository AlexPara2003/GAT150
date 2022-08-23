#pragma once
#include "Framework/Component.h"

namespace neu{
	class AudioComponent : public Component{
	public:
		AudioComponent() = default;
		void Update() override;
		void Play();
		void Stop();


		std::string m_soundName;
		bool m_playOnAwake = false;
		float m_volume = 1.0f;
		float m_pitch = 1.0f;
		bool m_loop = false;

		// Inherited via Component
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	};
}