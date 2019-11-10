//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "../item.h"

#include <base/ovsocket/socket.h>

namespace cfg
{
	struct Port : public Item
	{
		Port(const char *port, ov::SocketType default_type)
			: _port(port),
			  _default_type(default_type)
		{
		}

		explicit Port(const char *port)
			: _port(port)
		{
		}

		virtual int GetPort() const
		{
			return ov::Converter::ToInt32(_port);
		}

		ov::SocketType GetSocketType() const
		{
			auto tokens = _port.Split("/");

			if (tokens.size() != 2)
			{
				return _default_type;
			}

			auto type = tokens[1].UpperCaseString();

			if (type == "TCP")
			{
				return ov::SocketType::Tcp;
			}
			else if (type == "UDP")
			{
				return ov::SocketType::Udp;
			}
			else if (type == "SRT")
			{
				return ov::SocketType::Srt;
			}

			return ov::SocketType::Unknown;
		}

	protected:
		void MakeParseList() const override
		{
			RegisterValue<ValueType::Text>(nullptr, &_port);
		}

		ov::String _port;

		ov::SocketType _default_type = ov::SocketType::Tcp;
	};
}  // namespace cfg