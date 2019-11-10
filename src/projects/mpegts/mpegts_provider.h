//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2019 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "mpegts_observer.h"
#include "mpegts_server.h"

#include <stdint.h>
#include <unistd.h>
#include <algorithm>
#include <memory>
#include <thread>
#include <vector>

#include <base/media_route/media_buffer.h>
#include <base/media_route/media_type.h>
#include <base/ovlibrary/ovlibrary.h>
#include <base/provider/application.h>
#include <base/provider/provider.h>

class MpegTsProvider : public pvd::Provider, public MpegTsObserver
{
public:
	static std::shared_ptr<MpegTsProvider>
	Create(const info::Application *application_info, std::shared_ptr<MediaRouteInterface> router);

	explicit MpegTsProvider(const info::Application *application_info, std::shared_ptr<MediaRouteInterface> router);

	~MpegTsProvider() override;

	cfg::ProviderType GetProviderType() override
	{
		return cfg::ProviderType::MpegTs;
	}

	bool Start() override;

	std::shared_ptr<pvd::Application> OnCreateApplication(const info::Application *application_info) override;

	//--------------------------------------------------------------------
	// Implementation of MpegTsObserver
	//--------------------------------------------------------------------
	bool OnStreamReadyComplete(const ov::String &app_name, const ov::String &stream_name,
							   std::shared_ptr<MpegTsMediaInfo> &media_info,
							   info::application_id_t &application_id,
							   uint32_t &stream_id) override;

	bool OnVideoData(info::application_id_t application_id, uint32_t stream_id,
					 uint32_t timestamp,
					 MpegTsFrameType frame_type,
					 const std::shared_ptr<const ov::Data> &data) override;

	bool OnAudioData(info::application_id_t application_id, uint32_t stream_id,
					 uint32_t timestamp,
					 MpegTsFrameType frame_type,
					 const std::shared_ptr<const ov::Data> &data) override;

	bool OnDeleteStream(info::application_id_t application_id, uint32_t stream_id) override;

private:
	struct MpegTsStream
	{
		ov::SocketAddress address;

		ov::String app_name;
		ov::String stream_name;
	};

	const cfg::MpegTsProvider *_provider_info = nullptr;

	std::shared_ptr<MpegTsServer> _mpegts_server;

	// key: port
	// value: stream information
	std::map<int, MpegTsStream> _stream_table;
};
